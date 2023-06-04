set(GEO_HEADER
        Geometry.h
        )

set(GEO_SOURCE
        Geometry.cpp
        )

add_library(Geometry SHARED ${GEO_HEADER} ${GEO_SOURCE})
target_link_libraries(Geometry PUBLIC Base)