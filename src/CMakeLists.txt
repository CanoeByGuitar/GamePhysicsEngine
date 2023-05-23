set(ENGINE_HEADER
        Geometry.h)

set(ENGINE_SOURCE
        Geometry.cpp)

add_library(Engine SHARED ENGINE_HEADER ENGINE_SOURCE)
target_link_libraries(Engine PUBLIC Base)