#pragma once
#include <stdint.h>

typedef struct Hash_t *Hash;
typedef struct KeyPairValue_t *KeyPairValue;
typedef struct Vector_t *Vector;
#define EXPAND 4

struct Hash_t {
  KeyPairValue hash;
  int64_t *hashKeys;
  int64_t maxKey;
  int32_t size;
};

struct KeyPairValue_t {
  void **hashSize;
  int64_t *keys;
};

struct Vector_t {
  void *buffer;
  int64_t size;
  int64_t capacity;
  int32_t elemSize;
};

Hash hs_Init();
void hs_Set(Hash self, void *key, int64_t keySize, void *value, int64_t valueSize);
void *hs_Get(Hash self, void *key, int64_t keySize);
void hs_AddToFloatVector(Hash self, float *buffer, int64_t size, Vector vec);
void hs_AddToFloatValue(Hash self, float *buffer, int64_t size, int32_t value);
int32_t hs_GetFloatHashValue(Hash self, float *buffer, int64_t size);
int32_t vecSize(void *buffer);
void hs_Delete(Hash self);
Vector vec_Init(int32_t type);
void vec_Push(Vector self, void *element);
Vector hs_GetValues(Hash self);
void vec_Delete(Vector self);
Vector hs_GetVectorValues(Hash self);