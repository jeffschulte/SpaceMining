project(spaceMiningLib)

add_library(${PROJECT_NAME} STATIC spaceMiningLib.cpp)
include_directories(${CMAKE_SOURCE_DIR}/include)
