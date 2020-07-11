# Renderer
My own implementation of [Ray Tracing in One Weekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html). 

## Differences with the original tutorial
* Works on triangle meshes instead of only spheres
* Uses glm for 3D math

## Prerequisites
- Qt 5.12.7 (optional for PNG support)

## Build on Linux
```bash
$ mkdir build
$ cd build
$ cmake -DCMAKE_BUILD_TYPE=Release ..
$ make
```

## Author
Mathieu Gaillard
[HPCG Laboratory](http://hpcg.purdue.edu/)  
Department of Computer Graphics Technology  
Purdue University

## License
See the LICENSE file.