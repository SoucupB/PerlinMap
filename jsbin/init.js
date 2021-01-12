var initBuffer = null;
var getBufferFloatData = null;
var getTriBuffer = null;
var getTriBufferSize = null;
var getTriBufferArr = null;

function initWasm() {
  Module['onRuntimeInitialized'] = function (){
    initBuffer = Module._pc_Init;
    getBufferFloatData = Module._getElementFromBuffer;
    getTriBuffer = Module._tri_GetPolygonGrid;
    getTriBufferSize = Module._getTribufferSize;
    getTriBufferArr = Module._getTribufferBuffer;
  }
}

function isEverythingUp() {
  return initBuffer && getBufferFloatData && getTriBuffer && getTriBufferSize && getTriBufferArr;
}

function retrieveBufferDataIntoList(buffer, size) {
  let bufferData = [];
  for(let i = 0; i < size; i++) {
    bufferData.push(getBufferFloatData(buffer, i));
  }
  return bufferData;
}

initWasm();