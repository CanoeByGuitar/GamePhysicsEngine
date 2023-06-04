file(GLOB WINDOW_HEADER
        ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

file(GLOB WINDOW_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_library(Window SHARED ${WINDOW_HEADER} ${WINDOW_SOURCES})
target_link_libraries(Window PUBLIC Base)

if(APPLE)
    #   OPENGL
    find_package(OpenGL REQUIRED)

    #   GLFW
    find_package(glfw3 REQUIRED)

    target_link_libraries(Window PUBLIC OpenGL::GL glfw Geometry Loader)

elseif(Linux)

endif()