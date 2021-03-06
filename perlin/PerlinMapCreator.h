#pragma once
#include <stdint.h>

typedef struct PerlinMapCreator_t *PerlinMapCreator;
struct PerlinMapCreator_t {
  int32_t w;
  int32_t h;
  int8_t dimension;
  float triSize;
  float rigidity;
  float plainHeight;
};

typedef struct Tribuffer_t *Tribuffer;
struct Tribuffer_t {
  float *buffer;
  int32_t *indexes;
  int32_t offset;
  int64_t size;
  int64_t indexesSize;
};

PerlinMapCreator pc_Init(int32_t w, int32_t h, int8_t dimension, float triSize);
void pc_SetRigidity(PerlinMapCreator self, float rigidity);
void pc_SetPlainHeight(PerlinMapCreator self, float plainHeight);
void pc_Delete(PerlinMapCreator self);
Tribuffer tri_GetPolygonGrid(PerlinMapCreator self);
void tri_Delete(Tribuffer self);
void tri_ExportToObj(Tribuffer buffer, const char *file);
