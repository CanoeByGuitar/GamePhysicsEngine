File(GLOB IMGUI_HEADER
        *.h)

File(GLOB IMGUI_SOURCE
        *.cpp)

add_library(Imgui SHARED ${IMGUI_HEADER} ${IMGUI_SOURCE})
target_link_libraries(Imgui PUBLIC glfw)

if(APPLE)
    #   OPENGL
    find_package(OpenGL REQUIRED)

    #   GLFW
    find_package(glfw3 REQUIRED)

    target_link_libraries(Imgui PUBLIC OpenGL::GL glfw)

elseif(Linux)

endif()