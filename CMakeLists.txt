cmake_minimum_required(VERSION 3.20)
project(GamePhysicsInOneWeekend)

set(CMAKE_CXX_STANDARD 17)


find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIRS})

include_directories(${PROJECT_SOURCE_DIR}/external/)
include_directories(${PROJECT_SOURCE_DIR}/external/spdlog/include)
include_directories(${PROJECT_SOURCE_DIR}/external/glm/)
#if(LINUX)
#include_directories(${PROJECT_SOURCE_DIR}/external/glfw/include/GLFW)
#endif ()
include_directories(${PROJECT_SOURCE_DIR}/external/imgui)
include_directories(${PROJECT_SOURCE_DIR}/external/poly2tri)
include_directories(${PROJECT_SOURCE_DIR}/src)



set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

add_subdirectory(external/glm)
#if(LINUX)
#add_subdirectory(external/glfw)
#endif()
add_subdirectory(external/imgui)
add_subdirectory(external/poly2tri)
add_subdirectory(src/Math)
add_subdirectory(src/Base)
add_subdirectory(src/Loader)
add_subdirectory(src/Physic)
add_subdirectory(src/Geometry)
add_subdirectory(src/Renderer)
add_subdirectory(src/Window)
add_subdirectory(src/GUI)

add_subdirectory(src)
add_subdirectory(test)



add_executable(main main.cpp)
target_link_libraries(main Base)

if(LINUX)
add_definitions(-DfPIC)
endif ()