#include <stdio.h>
#include "PerlinMapCreator.h"
#include "Utils.h"
#include "SimpleHashBuffer.h"
#include <stdlib.h>

int main() {
  PerlinMapCreator element = pc_Init(1200, 500, 3, 20);
  Tribuffer triBuffer = tri_GetPolygonGrid(element);
  tri_ExportToObj(triBuffer, "grid.obj");
  printf("DONE!");
 // Hash hash = hs_Init();
 // int32_t ana = 5;
 // hs_AddToFloatValue(hash, (float[]){2.4f, 5.5f}, 2, ana);
 // printf("%d", hs_GetFloatHashValue(hash, (float[]){2.4f, 5.5f}, 2));
  // Vector arb = hs_GetFloatHashValue(hash, (float[]){2.4f, 5.5f}, 2);
  // Vector allResp = hs_GetValues(hash);
  // Vector resp = *((Vector *)allResp);
  // printf("%p %I64d\n", arb, allResp->size);
  // for(int32_t i = 0; i < allResp->size; i++) {
  //   printf("AAA %p\n", ((Vector *)resp->buffer)[i]);
  // }
}