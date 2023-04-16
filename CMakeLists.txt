cmake_minimum_required (VERSION 3.8)

project ("shader-compiler")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
set(CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/extern/)

find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)
find_package(SDL2 REQUIRED CONFIG COMPONENTS SDL2main)
find_package(SDL2_image REQUIRED CONFIG REQUIRED COMPONENTS SDL2_image)

add_subdirectory(${PROJECT_SOURCE_DIR}/ui)
find_library(dict NAMES dict HINTS ${PROJECT_SOURCE_DIR}/dict/build/Debug/*)
find_library(array NAMES array HINTS ${PROJECT_SOURCE_DIR}/array/build/Debug/*)
find_library(json NAMES json HINTS ${PROJECT_SOURCE_DIR}/json/build/Debug/*)
find_library(ui NAMES ui HINTS ${PROJECT_SOURCE_DIR}/ui/build/Debug/*)

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})

# Add source to this project's executable.
add_executable (shader-compiler "main.c") 

target_link_libraries(shader-compiler PUBLIC ui json array dict ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})
target_link_libraries(shader-compiler PRIVATE ui SDL2_image::SDL2_image)

# Define targets
target_include_directories(shader-compiler PUBLIC "include/" "ui/include/" "ui/json/include")