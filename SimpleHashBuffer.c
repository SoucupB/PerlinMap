#include "SimpleHashBuffer.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

const int64_t maxKey = 1<<14;
const int32_t keysNumber = 1<<6;

KeyPairValue kp_Init(int64_t size) {
  KeyPairValue self = malloc(sizeof(struct KeyPairValue_t));
  self->keys = malloc(sizeof(int64_t) * size);
  memset(self->keys, 0, sizeof(int64_t) * size);
  self->hashSize = malloc(sizeof(void *) * size);
  memset(self->hashSize, 0, sizeof(void *) * size);
  return self;
}

Hash hs_Init() {
  srand(seed);
  Hash self = malloc(sizeof(struct Hash_t));
  self->hash = kp_Init(maxKey);
  self->hashKeys = malloc(sizeof(int64_t) * keysNumber);
  self->maxKey = maxKey;
  self->size = 0;
  for(int32_t i = 0; i < keysNumber; i++) {
    self->hashKeys[i] = rand() * rand();
  }
  return self;
}

void kp_Delete(KeyPairValue self) {
  free(self->keys);
  free(self->hashSize);
  free(self);
}

void hs_Delete(Hash self) {
  free(self->hashKeys);
  kp_Delete(self->hash);
  free(self);
}

void hs_AddToFloatVector(Hash self, float *buffer, int64_t size, Vector vec) {
  hs_Set(self, buffer, sizeof(float) * size, &vec, sizeof(Vector));
}

void hs_AddToFloatValue(Hash self, float *buffer, int64_t size, int32_t value) {
  hs_Set(self, buffer, sizeof(float) * size, &value, sizeof(int32_t));
}

Vector hs_GetFloatHashVector(Hash self, float *buffer, int64_t size) {
  void *response = hs_Get(self, buffer, sizeof(float) * size);
  if(!response) {
    return NULL;
  }
  return *((Vector *)response);
}

int32_t hs_GetFloatHashValue(Hash self, float *buffer, int64_t size) {
  void *response = hs_Get(self, buffer, sizeof(float) * size);
  if(!response) {
    return 0;
  }
  return *((int32_t *)response);
}

void addValueToKey(Hash self, int64_t key, void *value, int64_t valueSize) {
  KeyPairValue hsk = self->hash;
  if(self->maxKey <= self->size) {
    printf("Hashmap has reaced its limit\n");
    exit(0);
    return ;
  }
  while(hsk->keys[key % self->maxKey] && hsk->keys[key % self->maxKey] != key) {
    key++;
  }
  hsk->keys[key % self->maxKey] = key;
  if(hsk->hashSize[key % self->maxKey]) {
    free(hsk->hashSize[key % self->maxKey]);
  }
  hsk->hashSize[key % self->maxKey] = malloc(valueSize);
  memcpy(hsk->hashSize[key % self->maxKey], value, valueSize);
  self->size++;
}

Vector hs_GetValues(Hash self) {
  Vector values = vec_Init(sizeof(void **));
  for(int32_t i = 0; i < self->maxKey; i++) {
    if(self->hash->hashSize[i]) {
      vec_Push(values, &self->hash->hashSize[i]);
    }
  }
  return values;
}

int64_t getKey(Hash self, void *key, int64_t keySize) {
  unsigned char *buffer = key;
  int64_t keyValue = 0;
  for(int32_t i = 0; i < keySize; i++) {
    keyValue ^= self->hashKeys[i] * (int64_t)buffer[i];
  }
  return keyValue;
}

void hs_Set(Hash self, void *key, int64_t keySize, void *value, int64_t valueSize) {
  int64_t keyValue = getKey(self, key, keySize);
  addValueToKey(self, keyValue, value, valueSize);
}

void *hs_Get(Hash self, void *key, int64_t keySize) {
  KeyPairValue hsk = self->hash;
  int64_t keyValue = getKey(self, key, keySize);
  while(hsk->keys[keyValue % self->maxKey] && hsk->keys[keyValue % self->maxKey] != keyValue) {
    keyValue++;
  }
  if(hsk->keys[keyValue % self->maxKey] == keyValue) {
    return hsk->hashSize[keyValue % self->maxKey];
  }
  return NULL;
}

Vector vec_Init(int32_t type) {
  Vector self = malloc(sizeof(struct Vector_t));
  self->buffer = malloc(type);
  self->capacity = 1;
  self->size = 0;
  self->elemSize = type;
  return self;
}

void vec_Push(Vector self, void *element) {
  if(self->size >= self->capacity) {
    self->buffer = realloc(self->buffer, self->capacity * EXPAND * self->elemSize);
    self->capacity *= EXPAND;
  }
  memcpy(self->buffer + self->size * self->elemSize, element, self->elemSize);
  self->size++;
}

void vec_Delete(Vector self) {
  free(self->buffer);
  free(self);
}
