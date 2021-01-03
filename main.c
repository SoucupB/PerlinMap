#include <stdio.h>
#include "PerlinMapCreator.h"
#include "Utils.h"
#include "SimpleHashBuffer.h"
#include <stdlib.h>

int main() {
  // PerlinMapCreator element = pc_Init(1200, 500, 3, 20);
  // Tribuffer triBuffer = tri_GetPolygonGrid(element);
  // tri_ExportToObj(triBuffer, "grid.obj");
  // printf("DONE!");
  Hash hash = hs_Init();
  int32_t buff[] = {5.4, -5.6};
  int32_t counter = 7;
  hs_Set(hash, buff, sizeof(buff), &counter);
  printf("%d", *(int32_t*)hs_Get(hash, buff, sizeof(buff)));
  // void *ana = malloc(8);
  // *((int32_t *)ana) = 15;
  // printf("\n%p %p %d", ana, (int32_t *)ana + 1, *((int32_t *)ana));
  // void *ana = NULL;
  // float cucur = 15.4325;
  // ana = push(ana, &cucur, sizeof(float));
  // ana = push(ana, &cucur, sizeof(float));
  // ana = push(ana, &cucur, sizeof(float));
  // cucur = 13;
  // ana = push(ana, &cucur, sizeof(float));
  // float *value = ana;
  // printf("\n%d\n", vecSize(ana));
  // for(int32_t i = 0; i < vecSize(ana); i++) {
  //   printf("\n%p %f", ana, value[i]);
  // }
}