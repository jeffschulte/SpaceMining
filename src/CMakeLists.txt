project(spaceMining)

add_executable(${PROJECT_NAME} ${PROJECT_NAME}.cpp)
include_directories(${CMAKE_SOURCE_DIR}/include)

add_subdirectory(spaceMiningLib)
target_link_libraries(${PROJECT_NAME} spaceMiningLib)
