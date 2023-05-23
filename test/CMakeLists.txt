
add_executable(LogTest LogTest.cpp)
target_link_libraries(LogTest Base)


add_executable(GLMTest GLMTest.cpp)
target_link_libraries(GLMTest Base Math)

add_executable(BinReaderTest BinReaderTest.cpp)
target_link_libraries(BinReaderTest Loader)

add_executable(EncodingTest EncodingTest.cpp)
target_link_libraries(EncodingTest Loader)

add_executable(ReadHeaderTest ReadHeaderTest.cpp)
target_link_libraries(ReadHeaderTest Loader)