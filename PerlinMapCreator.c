#include "PerlinMapCreator.h"
#include <stdlib.h>
#include <stdio.h>

PerlinMapCreator pc_Init(int32_t w, int32_t h, int8_t dimension, float triSize) {
  PerlinMapCreator self = malloc(sizeof(struct PerlinMapCreator_t));
  self->w = w;
  self->h = h;
  self->dimension = dimension;
  self->triSize = triSize;
  return self;
}

Tribuffer pc_getTriangleMesh(PerlinMapCreator self) {
  Tribuffer triBuffer = malloc(sizeof(struct Tribuffer_t));
  triBuffer->offset = self->dimension;
  triBuffer->size = 0;
  return triBuffer;
}

Tribuffer tri_GetPolygonGrid(PerlinMapCreator self) {
  Tribuffer triBuffer = malloc(sizeof(struct Tribuffer_t));
  triBuffer->offset = self->dimension;
  triBuffer->size = 0;
  int32_t h = self->h / (float)self->triSize;
  int32_t w = self->w / (float)self->triSize;
  triBuffer->buffer = malloc(sizeof(float) * h * w * self->dimension * 6);
  for(int32_t i = 0; i < h; i++) {
    for(int32_t j = 0; j < w; j++) {
      float fi = i;
      float fj = j;
      triBuffer->buffer[triBuffer->size++] = fj * self->triSize;
      triBuffer->buffer[triBuffer->size++] = fi * self->triSize;
      if(self->dimension == 3) {
        triBuffer->buffer[triBuffer->size++] = 0.0f;
      }
      triBuffer->buffer[triBuffer->size++] = fj * self->triSize;
      triBuffer->buffer[triBuffer->size++] = (fi + 1.0f) * self->triSize;
      if(self->dimension == 3) {
        triBuffer->buffer[triBuffer->size++] = 0.0f;
      }
      triBuffer->buffer[triBuffer->size++] = (fj + 1.0f) * self->triSize;
      triBuffer->buffer[triBuffer->size++] = fi * self->triSize;
      if(self->dimension == 3) {
        triBuffer->buffer[triBuffer->size++] = 0.0f;
      }
      triBuffer->buffer[triBuffer->size++] = fj * self->triSize;
      triBuffer->buffer[triBuffer->size++] = (fi + 1.0f) * self->triSize;
      if(self->dimension == 3) {
        triBuffer->buffer[triBuffer->size++] = 0.0f;
      }
      triBuffer->buffer[triBuffer->size++] = (fj + 1.0f) * self->triSize;
      triBuffer->buffer[triBuffer->size++] = (fi + 1.0f) * self->triSize;
      if(self->dimension == 3) {
        triBuffer->buffer[triBuffer->size++] = 0.0f;
      }
      triBuffer->buffer[triBuffer->size++] = (fj + 1.0f) * self->triSize;
      triBuffer->buffer[triBuffer->size++] = fi * self->triSize;
      if(self->dimension == 3) {
        triBuffer->buffer[triBuffer->size++] = 0.0f;
      }
    }
  }
  return triBuffer;
}

void tri_ExportToObj(Tribuffer buffer, const char *file) {
  FILE *fd = fopen(file, "w+");
  int32_t index = 1;
  for(int32_t i = 0; i < buffer->size; i += 3) {
    fprintf(fd, "v %f %f %f\n", buffer->buffer[i], buffer->buffer[i + 2], buffer->buffer[i + 1]);
  }
  for(int32_t i = 0; i < buffer->size; i += 9) {
    fprintf(fd, "f %d %d %d\n", index, index + 1, index + 2);
    index += 3;
  }
  fclose(fd);
}