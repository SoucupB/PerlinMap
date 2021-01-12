#include <stdio.h>
#include "PerlinMapCreator.h"
#include "Utils.h"
#include "SimpleHashBuffer.h"
#include <stdlib.h>

int main() {
  PerlinMapCreator element = pc_Init(300, 300, 3, 2);
  Tribuffer triBuffer = tri_GetPolygonGrid(element);
  tri_ExportToObj(triBuffer, "grid.obj");
  tri_Delete(triBuffer);
  free(element);
  printf("DONE!");
}