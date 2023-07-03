cmake_minimum_required (VERSION 3.26)

project ("shader-compiler")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/extern/)

IF(WIN32)
    find_package(SDL2_image REQUIRED CONFIG REQUIRED COMPONENTS SDL2_image)
ELSEIF(LINUX)
    set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
    find_package(SDL2_image REQUIRED)
ELSEIF(APPLE)

ENDIF(WIN32)

if ( NOT ("${HAS_JSON}"))
	message("[shader compiler] adding json project")
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/json ${CMAKE_SOURCE_DIR}/build/json)
endif()

if ( NOT ("${HAS_ARRAY}"))
	message("[shader compiler] adding array project")
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/array ${CMAKE_SOURCE_DIR}/build/array)
endif()

if ( NOT ("${HAS_DICT}"))
	message("[shader compiler] adding dict project")
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/dict ${CMAKE_SOURCE_DIR}/build/dict)
endif()

if ( NOT ("${HAS_UI}"))
	message("[shader compiler] adding ui project")
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/ui ${CMAKE_SOURCE_DIR}/build/ui)
endif()

find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)
find_package(SDL2 REQUIRED CONFIG COMPONENTS SDL2main)

# G10 executable
add_executable (shader-compiler "main.c" "Resource.rc") 
add_dependencies(shader-compiler json array dict ui)
target_include_directories(shader-compiler PUBLIC include ${CMAKE_SOURCE_DIR}/extern/json/include/ ${CMAKE_SOURCE_DIR}/extern/array/include/ ${CMAKE_SOURCE_DIR}/extern/dict/include/ ${CMAKE_SOURCE_DIR}/extern/ui/include/ ) 
target_link_libraries(shader-compiler PUBLIC json array dict ui ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} PRIVATE SDL2_image::SDL2_image )
IF(WIN32)
    target_link_libraries(ai-tool PUBLIC json array dict ui PRIVATE SDL2_image::SDL2_image ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})
ELSEIF(LINUX)
    target_link_libraries(ai-tool PUBLIC json array dict ui ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})
ELSEIF(APPLE)

ENDIF(WIN32)