cmake_minimum_required(VERSION 3.20)
project(GamePhysicsInOneWeekend)

set(CMAKE_CXX_STANDARD 17)

include_directories(${PROJECT_SOURCE_DIR}/external/)
include_directories(${PROJECT_SOURCE_DIR}/external/spdlog/include)
include_directories(${PROJECT_SOURCE_DIR}/external/glm/)
include_directories(${PROJECT_SOURCE_DIR}/external/imgui)
include_directories(${PROJECT_SOURCE_DIR}/external/glfw/)
include_directories(${PROJECT_SOURCE_DIR}/src)


add_subdirectory(external/glm)
add_subdirectory(external/imgui)
#add_subdirectory(external/glfw)

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
target_link_libraries(main Imgui)
