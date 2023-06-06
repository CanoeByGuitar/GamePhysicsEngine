file(GLOB LOAD_SOURCE
        *.cpp
)

file(GLOB LOAD_HEADER
        *.h
)

add_library(Loader SHARED ${LOAD_HEADER} ${LOAD_SOURCE})
target_link_libraries(Loader PUBLIC Base Geometry)