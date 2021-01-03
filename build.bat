emcc PerlinMapCreator.c Utils.c -Wall -s "EXPORTED_FUNCTIONS=['_pc_Init', '_getElementFromBuffer', '_getTribufferSize', '_tri_GetPolygonGrid', '_getTribufferBuffer']" -o Perlin.js -O3
