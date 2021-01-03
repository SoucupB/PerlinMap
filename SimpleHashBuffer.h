#pragma once
#include <stdint.h>

typedef struct Hash_t *Hash;
typedef struct KeyPairValue_t *KeyPairValue;
#define seed 84352662
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

Hash hs_Init();
void hs_Set(Hash self, void *key, int64_t keySize, void *value);
void *push(void *buffer, void *value, int64_t bufferSize);
void *hs_Get(Hash self, void *key, int64_t keySize);
int32_t vecSize(void *buffer);
void vecDelete(void *buffer);
void hs_Delete(Hash self);