class PerlinGenerator {
  constructor(h, w, dimension, triangleSize) {
    this.perlin = initBuffer(h, w, dimension, triangleSize)
    this.h = h;
    this.w = w;
    this.dimension = dimension;
    this.triangleSize = triangleSize;
  }

  getPerlinRandomMap() {
    let triBuffer = getTriBuffer(this.perlin);
    return retrieveBufferDataIntoList(getTriBufferArr(triBuffer), getTriBufferSize(triBuffer));
  }
}