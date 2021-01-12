# PerlinMap
A simple wasm perlin map creator
![alt text](Images/PerlinV1.png)
To build and create a randomly generated perlin noise landscape map
just run the command:

Linux:

    gcc main.c PerlinMapCreator.c Utils.c SimpleHashBuffer.c -Wall -o program -O9
  
Windows:
  
    buildC.bat
    
After the build is done, just run the 'program' executable and in 'grid.obj' map will be created.
  
 
