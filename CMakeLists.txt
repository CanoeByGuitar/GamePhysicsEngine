cmake_minimum_required(VERSION 3.20)
project(GamePhysicsInOneWeekend)

set(CMAKE_CXX_STANDARD 17)

include_directories(${PROJECT_SOURCE_DIR}/external/)
include_directories(${PROJECT_SOURCE_DIR}/external/spdlog/include)
include_directories(${PROJECT_SOURCE_DIR}/external/glm/)
include_directories(${PROJECT_SOURCE_DIR}/src)


add_subdirectory(external/glm)
add_subdirectory(src/Math)
add_subdirectory(src/Base)
add_subdirectory(src/Loader)
add_subdirectory(test)



add_executable(GamePhysicsInOneWeekend main.cpp)
