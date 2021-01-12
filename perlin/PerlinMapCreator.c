#include "PerlinMapCreator.h"
#include <stdlib.h>
#include <stdio.h>
#include "SimpleHashBuffer.h"
#include "Utils.h"
#include <string.h>
#include <math.h>

Vector getNeighbors(Tribuffer self);
PerlinMapCreator pc_Init(int32_t w, int32_t h, int8_t dimension, float triSize) {
  PerlinMapCreator self = malloc(sizeof(struct PerlinMapCreator_t));
  self->w = w;
  self->h = h;
  self->dimension = dimension;
  self->triSize = triSize;
  self->rigidity = 0.05f;
  self->plainHeight = 9.0f;
  return self;
}

void pc_Delete(PerlinMapCreator self) {
  free(self);
}

void pc_SetRigidity(PerlinMapCreator self, float rigidity) {
  self->rigidity = rigidity;
}

void pc_SetPlainHeight(PerlinMapCreator self, float plainHeight) {
  self->plainHeight = plainHeight;
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
  hs_Delete(hash);
  self->size = triBuffer->size;
  self->buffer = triBuffer->buffer;
  int32_t *bfInt = indexes->buffer;
  self->indexesSize = indexes->size;
  self->indexes = bfInt;
}

int8_t isElementPresent(Vector buffer, int32_t element) {
  for(int32_t i = 0; i < buffer->size; i++) {
    if(element == ((int32_t *)buffer->buffer)[i]) {
      return 1;
    }
  }
  return 0;
}

void addElementToBuffer(Vector line, int32_t element) {
  if(!isElementPresent(line, element)) {
    vec_Push(line, &element);
  }
}

Vector getNeighbors(Tribuffer self) {
  Vector graphMap = vec_Init(sizeof(Vector));
  for(int32_t i = 0; i < self->size / 3; i++) {
    Vector cVector = vec_Init(sizeof(int32_t));
    vec_Push(graphMap, &cVector);
  }
  for(int32_t i = 0; i < self->indexesSize; i += 3) {
    Vector firstPoint = ((Vector *)graphMap->buffer)[self->indexes[i] - 1];
    addElementToBuffer(firstPoint, self->indexes[i + 1] - 1);
    addElementToBuffer(firstPoint, self->indexes[i + 2] - 1);
    Vector secondPoint = ((Vector *)graphMap->buffer)[self->indexes[i + 1] - 1];
    addElementToBuffer(secondPoint, self->indexes[i] - 1);
    addElementToBuffer(secondPoint, self->indexes[i + 2] - 1);
    Vector thirdPoint = ((Vector *)graphMap->buffer)[self->indexes[i + 2] - 1];
    addElementToBuffer(thirdPoint, self->indexes[i] - 1);
    addElementToBuffer(thirdPoint, self->indexes[i + 1] - 1);
  }
  return graphMap;
}

float getDiscreteValueFromMap(int32_t i, int32_t j, Vector map) {
  float *buffer = map->buffer;
  int32_t w = (int32_t)buffer[0];
  int32_t h = (int32_t)buffer[1];
  return buffer[i * h + j % w + 2];
}

float getBillinearInterp(float x, float y, Vector interpMatrix) {
  int32_t nvx = (int32_t)x;
  int32_t nvy = (int32_t)y + 1;
  int32_t nex = (int32_t)x + 1;
  int32_t ney = (int32_t)y + 1;
  int32_t svx = (int32_t)x;
  int32_t svy = (int32_t)y;
  int32_t sex = (int32_t)x + 1;
  int32_t sey = (int32_t)y;
  float p1 = (getDiscreteValueFromMap(nex, ney, interpMatrix) - getDiscreteValueFromMap(sex, sey, interpMatrix)) * (y - (float)sey);
  float p2 = (getDiscreteValueFromMap(nvx, nvy, interpMatrix) - getDiscreteValueFromMap(svx, svy, interpMatrix)) * (y - (float)svy);
  float l = (x - (float)svx);
  return l * (p1 + getDiscreteValueFromMap(sex, sey, interpMatrix)) + (1.0f - l) * (getDiscreteValueFromMap(svx, svy, interpMatrix) + p2);
}

float polyInterpolation(PerlinMapCreator perlin, float x, float y, Vector interpNumbers) {
  return getBillinearInterp(x * perlin->rigidity, y * perlin->rigidity, interpNumbers);
}

Vector getRandomNumbers(float minim, float maxim, int32_t numberOfPoints) {
  Vector buffer = vec_Init(sizeof(float));
  for(int32_t i = 0; i < numberOfPoints; i++) {
    float number = func_Uniform(minim, maxim);
    vec_Push(buffer, &number);
  }
  return buffer;
}

Vector createDiscreteRandomMap(int32_t h, int32_t w, float minim, float maxim) {
  Vector interpNumbers = vec_Init(sizeof(float));
  float fw = (float)w;
  float fh = (float)h;
  vec_Push(interpNumbers, &fw);
  vec_Push(interpNumbers, &fh);
  for(int32_t i = 0; i < h * w; i++) {
    float number = func_Uniform(minim, maxim);
    vec_Push(interpNumbers, &number);
  }
  return interpNumbers;
}


void linearInterpolation(Tribuffer self, PerlinMapCreator perlin, int32_t point) {
  Vector interpMatrix = createDiscreteRandomMap(perlin->h + 1, perlin->w + 1, -1.0f, 1.0f);
  for(int32_t i = 0; i < self->size; i += 3) {
    float x = self->buffer[i] - perlin->w / 2.0;
    float y = self->buffer[i + 1] - perlin->h / 2.0;
    self->buffer[i + 2] = polyInterpolation(perlin, x + perlin->w / 2.0, y + perlin->h / 2.0, interpMatrix) * perlin->plainHeight;
  }
  vec_Delete(interpMatrix);
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
  linearInterpolation(triBuffer, self, 2);
  return triBuffer;
}

void tri_Delete(Tribuffer buffer) {
  free(buffer->indexes);
  free(buffer->buffer);
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