#include "SimpleHashBuffer.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//Hashmap with exclusively heap allocated values
const int64_t maxKey = 1<<14;
const int32_t keysNumber = 1<<6;

KeyPairValue kp_Init(int64_t size) {
  KeyPairValue self = malloc(sizeof(struct KeyPairValue_t));
  self->keys = malloc(sizeof(int64_t) * size);
  memset(self->keys, 0, sizeof(int64_t) * size);
  self->hashSize = malloc(sizeof(void *) * size);
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

void addValueToKey(Hash self, int64_t key, void *value) {
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
  hsk->hashSize[key % self->maxKey] = value;
  self->size++;
}

int64_t getKey(Hash self, void *key, int64_t keySize) {
  const unsigned char *buffer = key;
  int64_t keyValue = 0;
  for(int32_t i = 0; i < keySize; i++) {
    keyValue ^= self->hashKeys[i] * (int64_t)buffer[i];
  }
  return keyValue;
}

void hs_Set(Hash self, void *key, int64_t keySize, void *value) {
  int64_t keyValue = getKey(self, key, keySize);
  addValueToKey(self, keyValue, value);
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

void *push(void *buffer, void *value, int64_t valueSize) {
  if(!buffer) {
    buffer = malloc(valueSize * EXPAND + sizeof(int32_t) * 2);
    *((int32_t *)buffer) = 1;
    *((int32_t *)buffer + 1) = 0;
    buffer += 8;
  }
  if(*((int32_t *)buffer - 2) <= *((int32_t *)buffer - 1)) {
    *((int32_t *)buffer - 2) *= EXPAND;
    buffer = realloc(buffer - 8, *((int32_t *)buffer - 2) * valueSize + sizeof(int32_t) * 2);
    buffer += 8;
  }
  memcpy(buffer + *((int32_t *)buffer - 1) * valueSize, value, valueSize);
  (*((int32_t *)buffer - 1))++;
  return buffer;
}

int32_t vecSize(void *buffer) {
  return *((int32_t *)buffer - 1);
}

void vecDelete(void *buffer) {
  free(buffer - 8);
}
