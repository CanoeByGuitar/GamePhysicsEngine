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
    find_package(OpenGL REQUIRED)
    find_package(glfw3 REQUIRED)
    target_link_libraries(Renderer PUBLIC OpenGL::GL glfw Geometry Loader)
elseif(Linux)
    if (POLICY CMP0072)
        set(OpenGL_GL_PREFERENCE GLVND)
    endif()
    find_package(OpenGL REQUIRED)
    find_package(glfw3 REQUIRED)
    target_link_libraries(Renderer PUBLIC ${OPENGL_LIBRARY} glfw Geometry Loader)
endif()

