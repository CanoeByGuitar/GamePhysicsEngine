add_executable(LogTest LogTest.cpp)
target_link_libraries(LogTest Base)


#add_executable(GLMTest GLMTest.cpp)
#target_link_libraries(GLMTest Base Math)
#
#add_executable(BinReaderTest BinReaderTest.cpp)
#target_link_libraries(BinReaderTest Loader)
#
#add_executable(EncodingTest EncodingTest.cpp)
#target_link_libraries(EncodingTest Loader)
#
#add_executable(ReadHeaderTest ReadHeaderTest.cpp)
#target_link_libraries(ReadHeaderTest Loader)
#
#add_executable(GeoTest GeoTest.cpp)
#target_link_libraries(GeoTest Engine)
#
#add_executable(GLFWTest GLFWTest.cpp)
#target_link_libraries(GLFWTest Renderer)
#
#add_executable(BaseUtilTest BaseUtilTest.cpp)
#target_link_libraries(BaseUtilTest Base)

#add_executable(RendererTest RendererTest.cpp)
#target_link_libraries(RendererTest Engine)

add_executable(ActorRenderTest ActorRenderTest.cpp)
target_link_libraries(ActorRenderTest Engine)

#
#add_executable(BVH BVHTest.cpp)
#target_link_libraries(BVH Engine)
#
#add_executable(JSON JSONTest.cpp)
#target_link_libraries(JSON Loader)

add_executable(ClothData ClothDatasetTest.cpp)
target_link_libraries(ClothData Engine)

add_executable(ConvexHull ConvexHull.cpp)
target_link_libraries(ConvexHull Engine)