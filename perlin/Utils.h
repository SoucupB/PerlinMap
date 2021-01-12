#pragma once
#include <stdint.h>
#include "PerlinMapCreator.h"

float getElementFromBuffer(float *buffer, int32_t position);
int32_t getTribufferSize(Tribuffer buffer);
float *getTribufferBuffer(Tribuffer buffer);
float func_Uniform(float left, float right);