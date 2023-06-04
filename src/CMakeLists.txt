File(GLOB ENGINE_SOURCE
        ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

FILE(GLOB ENGINE_HEAD
        ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

add_library(Engine SHARED ${ENGINE_HEAD} ${ENGINE_SOURCE})
target_link_libraries(Engine PUBLIC Renderer Base Geometry Loader Window)