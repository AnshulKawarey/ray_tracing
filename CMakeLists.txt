cmake_minimum_required(VERSION 4.1.0)

project(raytracer)

add_executable(${PROJECT_NAME} main.cpp)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set ( SOURCE_ONE_WEEKEND
  main.cc
  vec3.h
  color.h
  ray.h
)
