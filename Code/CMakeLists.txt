cmake_minimum_required(VERSION 3.25)
project(MyRenderer)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenMP)
if(OPENMP_FOUND)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

file(GLOB SOURCES *.h *.cpp)

add_executable(${PROJECT_NAME} ${SOURCES} ThirdParties/geometry.h model.cpp model.h renderer.cpp renderer.h utilities.cpp utilities.h Texture.cpp Texture.h Shaders/Shader.cpp Shaders/Shader.h rasterizer.cpp rasterizer.h Buffer.cpp Buffer.h ThirdParties/geometry.cpp ThirdParties/tgaimage.cpp Camera.cpp Camera.h)
