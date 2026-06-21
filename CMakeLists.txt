cmake_minimum_required(VERSION 4.1.0)

project(raytracer LANGUAGES CXX)

if(APPLE)
  execute_process(
    COMMAND brew --prefix libomp
    OUTPUT_VARIABLE LIBOMP_PREFIX
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )
  set(OpenMP_CXX_FLAGS "-Xpreprocessor -fopenmp -I${LIBOMP_PREFIX}/include")
  set(OpenMP_CXX_LIB_NAMES "omp")
  set(OpenMP_omp_LIBRARY "${LIBOMP_PREFIX}/lib/libomp.dylib")
endif()

find_package(OpenMP REQUIRED)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} OpenMP::OpenMP_CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set ( SOURCE_ONE_WEEKEND
  main.cc
  vec3.h
  color.h
  ray.h
  sphere.h
  hittable.h
  hittable_list.h
  rtinclude.h
  camera.h
)
