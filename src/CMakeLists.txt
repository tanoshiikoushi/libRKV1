file(GLOB CPP_SOURCES "*.cpp")
file(GLOB H_HEADERS "../include/*.h")

add_library(RKV1 STATIC ${CPP_SOURCES} ${H_HEADERS})
target_include_directories(RKV1 PUBLIC ../include)
target_compile_features(RKV1 PUBLIC cxx_std_17)
set_target_properties(RKV1 PROPERTIES CXX_EXTENSIONS OFF)

source_group (
	TREE "${PROJECT_SOURCE_DIR}/include"
	PREFIX "Header Files"
	FILES ${H_HEADERS})