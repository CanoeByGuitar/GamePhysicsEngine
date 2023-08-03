file(GLOB GEO_HEADER
        *.h
        )

file(GLOB GEO_SOURCE
        *.cpp
        )

add_library(Geometry SHARED ${GEO_HEADER} ${GEO_SOURCE})
target_link_libraries(Geometry PUBLIC Base poly2tri)