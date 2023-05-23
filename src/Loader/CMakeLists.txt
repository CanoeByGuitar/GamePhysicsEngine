set(
        LOAD_SOURCE
        UnifiedModel.cpp
)

set(
        LOAD_HEADER
        BinReader.h
        UnifiedModel.h
        PmxReader.h
        Utils.h
)

add_library(Loader ${LOAD_HEADER} ${LOAD_SOURCE})
target_link_libraries(Loader PUBLIC Base)