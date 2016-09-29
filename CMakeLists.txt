cmake_minimum_required(VERSION 3.6)
project(ev++)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/modules/")
find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
set(CMAKE_CXX_STANDARD 11)
set(BOX2D_BUILD_EXAMPLES false)
set(BOX2D_BUILD_UNITTESTS false)
set(BOX2D_INSTALL_DOC false)
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/vendor/json
  ${SDL2_INCLUDE_DIR}
  ${SDL2_IMAGE_INCLUDE_DIRS}
  ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glew/include
  $ENV{VULKAN_SDK}/Include
  ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glm
  ${CMAKE_CURRENT_SOURCE_DIR}/vendor/Box2D
  )
IF(MSVC)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
ENDIF(MSVC)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNOMINMAX -D_USE_MATH_DEFINES")

IF(WIN32)
  find_library(VULKAN_LIB NAMES vulkan-1 vulkan PATHS $ENV{VULKAN_SDK}/Source/lib32)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DVK_USE_PLATFORM_WIN32_KHR")
ELSEIF(LINUX)
  find_library(VULKAN_LIB NAMES libvulkan.so PATHS ${CMAKE_SOURCE_DIR}/libs/vulkan)
  find_package(ASSIMP REQUIRED)
  find_package(XCB REQUIRED)
  find_package(Threads REQUIRED)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DVK_USE_PLATFORM_XCB_KHR")
ELSE()
  add_definitions(-DNO_VULKAN)
  set(VULKAN_LIB "")
ENDIF(WIN32)


add_definitions(-D_CRT_SECURE_NO_WARNINGS)
add_subdirectory( vendor/Box2D )
add_definitions(-DGLEW_STATIC)
add_executable(main src/main.cpp
  src/application.cpp
  src/application.hpp
  src/sprite_sheet.hpp
  src/sprite_sheet.cpp
  src/sprite.hpp
  src/sprite.cpp
  src/texture.hpp
  src/texture.cpp
  src/vertex_buff.hpp
  src/vertex_buff.cpp
  src/sprite_batch.hpp
  src/sprite_batch.cpp
  src/shader_program.cpp
  src/shader_program.hpp
  src/ev.hpp
  vendor/glm/glm.hpp
  vendor/glew/src/glew.c
  )
add_definitions(-DSOURCE_DIRECTORY=${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(main ${SDL2_IMAGE_LIBRARY} ${SDL2_LIBRARY} ${CONAN_LIBS} Box2D ${OPENGL_LIBRARY} ${VULKAN_LIB})
