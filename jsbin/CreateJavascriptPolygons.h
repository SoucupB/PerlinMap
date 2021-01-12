#pragma once
#include <stdint.h>
#include "PerlinMapCreator.h"

char *getFileBuffer(const char *script);
void populateScriptPolygon(PerlinMapCreator element, const char *script);