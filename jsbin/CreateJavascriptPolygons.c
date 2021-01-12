#include "CreateJavascriptPolygons.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char *getFileBuffer(const char *script) {
  FILE *fd = fopen(script, "r+");
  int64_t bufferLen = 8096;
  char *buffer = malloc(sizeof(char) * bufferLen);
  memset(buffer, 0, sizeof(char) * bufferLen);
  int64_t bufferSize = 0;
  char chr = 0;
  int64_t counter = 0;
  fseek(fd, 0L, SEEK_END);
  int64_t sz = ftell(fd);
  fseek(fd, 0L, SEEK_SET);
  while(counter++ < sz) {
    fscanf(fd, "%c", &chr);
    buffer[bufferSize++] = chr;
    chr = 0;
  }
  fclose(fd);
  return buffer;
}

int32_t maxim(int8_t a, int8_t b) {
  return a < b ? b : a;
}

void addElementToOffset(char *buffer, float element, int64_t *offset) {
  const int32_t mat = 6;
  int32_t cnt = 0;
  int32_t elementSize = 0;
  int32_t i = 0;
  char elBuffer[128] = {0};
  int32_t cElement = element;
  while(cElement > 0) {
    cElement /= 10;
    elementSize++;
  }
  while(cnt++ < mat) {
    element *= 10;
  }
  int64_t intValue = element;
  while(intValue > 0) {
    elBuffer[i++] = intValue % 10 + '0';
    intValue /= 10;
  }
  if(elementSize == 0) {
    buffer[(*offset)++] = '0';
  }
  for(int32_t j = 0; j < elementSize; j++) {
    buffer[(*offset)++] = elBuffer[i - j - 1];
  }
  buffer[(*offset)++] = '.';
  for(int32_t j = mat - 1; j >= 0; j--) {
    buffer[(*offset)++] = maxim(48, elBuffer[j + cElement]);
  }
}

void populateScriptPolygon(PerlinMapCreator element, const char *script) {
  char *buffer = getFileBuffer(script);
  int64_t bufferSize = strlen(buffer);
  buffer[bufferSize++] = '\n';
  const char *instr = "let polygons = [\n";
  strcat(buffer, instr);
  bufferSize += strlen(instr);
  Tribuffer triBuffer = getPolygonGrid(element);
  for(int32_t i = 0; i < triBuffer->size; i += 2) {
    printf("%f\n", triBuffer->buffer[i]);
    addElementToOffset(buffer, triBuffer->buffer[i], &bufferSize);
    strcat(buffer, ",");
    bufferSize++;
    addElementToOffset(buffer, triBuffer->buffer[i + 1], &bufferSize);
    strcat(buffer, ",\n");
    bufferSize += 2;
  }
  buffer[bufferSize - 2] = ']';
  printf("%s\n", buffer);
  FILE *fd = fopen("result.js", "w+");
  fprintf(fd, "%s", buffer);
  fclose(fd);
}