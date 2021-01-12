#include "Utils.h"
#include <stdint.h>
#include <math.h>
#include <stdlib.h>

float getElementFromBuffer(float *buffer, int32_t position) {
  return buffer[position];
}

int32_t getTribufferSize(Tribuffer buffer) {
  return buffer->size;
}

float *getTribufferBuffer(Tribuffer buffer) {
  return buffer->buffer;
}

float func_Uniform(float left, float right) {
  float randomNumber = rand() / (float)RAND_MAX;
  return left + (right - left) * fabs(randomNumber);
}