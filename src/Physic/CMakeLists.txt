file(GLOB PHY_SOURCES
        *.cpp)

file(GLOB PHY_HEADER
        *.h)

add_library(Physics SHARED ${PHY_SOURCES} ${PHY_HEADER})
target_link_libraries(Physics PUBLIC Base Geometry)

