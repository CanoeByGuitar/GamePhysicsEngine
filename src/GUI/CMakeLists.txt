file(GLOB GUI_HEADER
        ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

file(GLOB GUI_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_library(Gui SHARED ${GUI_HEADER} ${GUI_SOURCES})
target_link_libraries(Gui PUBLIC Base Imgui)

if(APPLE)
    find_package(OpenGL REQUIRED)
    find_package(glfw3 REQUIRED)

    target_link_libraries(Gui PUBLIC OpenGL::GL glfw Geometry Loader)
elseif(Linux)
    message(STATUS "Configuring on/for Linux")
    if (POLICY CMP0072)
        set(OpenGL_GL_PREFERENCE GLVND)
    endif()
    find_package(OpenGL REQUIRED)
    find_package(glfw3 REQUIRED)
    target_link_libraries(Gui PUBLIC glfw ${OPENGL_LIBRARY} Geometry Loader)
endif()

