#include <stdio.h>
#include "PerlinMapCreator.h"
#include "Utils.h"
#include "SimpleHashBuffer.h"
#include <stdlib.h>
#include <time.h>

int main() {
  uint64_t seed = time(NULL);
  srand(seed);
  PerlinMapCreator perlinData = pc_Init(300, 300, 3, 2);
  pc_SetPlainHeight(perlinData, 15.0f);
  pc_SetRigidity(perlinData, 0.06);
  Tribuffer triBuffer = tri_GetPolygonGrid(perlinData);
  tri_ExportToObj(triBuffer, "grid.obj");
  tri_Delete(triBuffer);
  pc_Delete(perlinData);
  printf("DONE!");
}