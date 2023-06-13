File(GLOB IMGUI_HEADER
        *.h)

File(GLOB IMGUI_SOURCE
        *.cpp
        ../../src/Renderer/glad.c
        )

add_library(Imgui SHARED ${IMGUI_HEADER} ${IMGUI_SOURCE})
target_link_libraries(Imgui PUBLIC glfw)

if(APPLE)
    find_package(OpenGL REQUIRED)
    find_package(glfw3 REQUIRED)

    target_link_libraries(Imgui PUBLIC OpenGL::GL glfw)
elseif(Linux)
    if (POLICY CMP0072)
        set(OpenGL_GL_PREFERENCE GLVND)
    endif()
    find_package(OpenGL REQUIRED)
    find_package(glfw3 REQUIRED)
    target_link_libraries(Imgui PUBLIC glfw ${OPENGL_LIBRARY} )

endif()


