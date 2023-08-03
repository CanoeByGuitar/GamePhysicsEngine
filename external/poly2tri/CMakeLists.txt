file(GLOB SOURCES
        common/*.cc
        sweep/*.cc)

file(GLOB HEADERS
        *.h
        common/*.h
        sweep/*.h)

add_library(poly2tri ${SOURCES} ${HEADERS})
