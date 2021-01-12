#include <stdio.h>
#include "PerlinMapCreator.h"
#include "Utils.h"
#include "SimpleHashBuffer.h"
#include <stdlib.h>

int main() {
  PerlinMapCreator perlinData = pc_Init(300, 300, 3, 2);
  pc_SetPlainHeight(perlinData, 12.0f);
  pc_SetRigidity(perlinData, 0.08);
  Tribuffer triBuffer = tri_GetPolygonGrid(perlinData);
  tri_ExportToObj(triBuffer, "grid.obj");
  tri_Delete(triBuffer);
  free(perlinData);
  printf("DONE!");
}