#include "Utils.h"
#include <stdint.h>

float getElementFromBuffer(float *buffer, int32_t position) {
  return buffer[position];
}

int32_t getTribufferSize(Tribuffer buffer) {
  return buffer->size;
}

float *getTribufferBuffer(Tribuffer buffer) {
  return buffer->buffer;
}