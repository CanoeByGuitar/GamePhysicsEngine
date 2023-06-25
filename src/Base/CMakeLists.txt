file(GLOB
        BASE_SOURCE
        *.cpp
)

file(GLOB
        BASE_HEADER
        *.h
)



add_library(Base STATIC ${BASE_SOURCE} ${BASE_HEADER})