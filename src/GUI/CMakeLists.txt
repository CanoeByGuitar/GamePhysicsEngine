file(GLOB GUI_HEADER
        ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

file(GLOB GUI_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_library(Gui SHARED ${GUI_HEADER} ${GUI_SOURCES})
target_link_libraries(Gui PUBLIC Base Imgui)

if(APPLE)
    #   OPENGL
    find_package(OpenGL REQUIRED)

    #   GLFW
    find_package(glfw3 REQUIRED)

    target_link_libraries(Gui PUBLIC OpenGL::GL glfw Geometry Loader)

elseif(Linux)

endif()