# RAY TRACER

## What is this?

A software ray tracer built from scratch in C++, following and extending 
"Ray Tracing in One Weekend" by Peter Shirley.

## Current Progress

- [x] Vector math utilities (vec3)  
- [x] Ray abstraction  
- [x] Sphere intersection with surface normals  
- [x] Normal visualization (colored by direction)

#### Build Steps:

- Go to root of project
- (First Time Setup ):

```
cmake -S . -B build
```

- Then (unless changing CMakeLists or generators/options):

```
cmake --build build
```

#### To execute:

- From the root

```
.\build\raytracer.exe > output.ppm
```

