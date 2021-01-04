#include "PerlinMapCreator.h"
#include <stdlib.h>
#include <stdio.h>
#include "SimpleHashBuffer.h"
#include "Utils.h"

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

void dataTransformation(Tribuffer self) {
  Hash hash = hs_Init();
  int32_t edges = 1;
  Vector triBuffer = vec_Init(sizeof(float));
  Vector indexes = vec_Init(sizeof(int32_t));
  for(int32_t i = 0; i < self->size; i += 3) {
    int32_t response = hs_GetFloatHashValue(hash, (float[]){self->buffer[i], self->buffer[i + 1]}, 2);
    if(!response) {
      vec_Push(triBuffer, &self->buffer[i]);
      vec_Push(triBuffer, &self->buffer[i + 1]);
      vec_Push(triBuffer, &self->buffer[i + 2]);
      int32_t index = edges;
      vec_Push(indexes, &index);
      hs_AddToFloatValue(hash, (float[]){self->buffer[i], self->buffer[i + 1]}, 2, edges);
      edges++;
    }
    else {
      vec_Push(indexes, &response);
    }
  }
  free(self->buffer);
  self->size = triBuffer->size;
  self->buffer = triBuffer->buffer;
  int32_t *bfInt = indexes->buffer;
  self->indexesSize = indexes->size;
  self->indexes = bfInt;
}



void perlinTransform(Tribuffer self) {
  float lastNumber = func_Uniform(0.0f, 100.0f);
  for(int32_t i = 0; i < self->size; i += 3) {
    self->buffer[i + 2] = lastNumber + func_Uniform(-20.0f, 20.0f);
  }
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
  dataTransformation(triBuffer);
  perlinTransform(triBuffer);
  return triBuffer;
}

void tri_ExportToObj(Tribuffer buffer, const char *file) {
  FILE *fd = fopen(file, "w+");
  int32_t index = 1;
  for(int32_t i = 0; i < buffer->size; i += 3) {
    fprintf(fd, "v %f %f %f\n", buffer->buffer[i], buffer->buffer[i + 2], buffer->buffer[i + 1]);
  }
  for(int32_t i = 0; i < buffer->indexesSize; i += 3) {
    fprintf(fd, "f %d %d %d\n", buffer->indexes[i], buffer->indexes[i + 1], buffer->indexes[i + 2]);
    index += 3;
  }
  fclose(fd);
}