project(spaceMiningTest)

add_executable(${PROJECT_NAME} test.cpp)
target_link_libraries(${PROJECT_NAME} spaceMiningLib)
include_directories(${CMAKE_SOURCE_DIR}/include)

add_test(NAME SpaceMiningTest1 COMMAND "spaceMiningTest" "test1")
add_test(NAME SpaceMiningTest2 COMMAND "spaceMiningTest" "test2")