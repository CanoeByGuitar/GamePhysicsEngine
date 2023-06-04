file(GLOB RENDER_HEADER
        ${CMAKE_CURRENT_SOURCE_DIR}/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/Objects/*.h)

file(GLOB RENDER_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/glad.c
        ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Objects/*.cpp)

add_library(Renderer SHARED ${RENDER_HEADER} ${RENDER_SOURCES})
target_link_libraries(Renderer PUBLIC Base)

if(APPLE)
#   OPENGL
    find_package(OpenGL REQUIRED)

#   GLFW
    find_package(glfw3 REQUIRED)

    target_link_libraries(Renderer PUBLIC OpenGL::GL glfw Geometry Loader)

elseif(Linux)

endif()