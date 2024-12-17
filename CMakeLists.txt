cmake_minimum_required(VERSION 3.25)
project(ogltest)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(glm CONFIG 1.0.0 REQUIRED)
find_package(SDL3 3.1 REQUIRED)
find_package(OpenGL REQUIRED)

include(FetchContent)
FetchContent_Declare(glad
    URL "${CMAKE_CURRENT_SOURCE_DIR}/glad.zip"
)
FetchContent_MakeAvailable(glad)

file(GLOB ogl_src "src/*.*")
add_executable(ogl ${ogl_src})
target_compile_definitions(ogl PUBLIC
    "GLM_FORCE_AVX2=1"
    "GLM_FORCE_SWIZZLE=1"
    "GLM_FORCE_EXPLICIT_CTOR=1"
    "GLM_FORCE_ALIGNED_GENTYPES=1"
    "GLM_ENABLE_EXPERIMENTAL=1"
)
target_link_libraries(ogl PUBLIC glad glm::glm OpenGL::GL SDL3::SDL3-static)
