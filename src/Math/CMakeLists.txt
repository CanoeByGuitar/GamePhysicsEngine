set(
        MATH_SOURCE
#        Log.cpp
        GC.cpp
)

set(
        MATH_HEADER
        Bounds.h
        GC.h
)

add_library(Math SHARED ${MATH_SOURCE} ${MATH_HEADER})