project(ev++)
cmake_minimum_required(VERSION 2.8)
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
  )

add_subdirectory( vendor/Box2D )
add_definitions(-DGLEW_STATIC)
add_executable(main src/main.cpp
  src/application.cpp
  vendor/glew/src/glew.c
  )
target_link_libraries(main ${SDL2_IMAGE_LIBRARY} ${SDL2_LIBRARY} ${CONAN_LIBS} Box2D ${OPENGL_LIBRARY})
