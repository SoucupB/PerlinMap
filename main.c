#include <stdio.h>
#include "PerlinMapCreator.h"
#include "Utils.h"

int main() {
  PerlinMapCreator element = pc_Init(1200, 500, 3, 20);
  Tribuffer triBuffer = tri_GetPolygonGrid(element);
  tri_ExportToObj(triBuffer, "grid.obj");
  printf("DONE!");
}