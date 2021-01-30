cmake_minimum_required(VERSION 3.12)

include(CMakePackageConfigHelpers)

SET(lib_version 1.0)

project(libRKV1 VERSION ${lib_version} 
				DESCRIPTION "A library for editing RKV1 files"
				LANGUAGES CXX)
				
add_subdirectory(src)

configure_package_config_file(
	libRKV1Config.cmake.in
	libRKV1Config.cmake
	INSTALL_DESTINATION lib/cmake/libRKV1)

write_basic_package_version_file(
	libRKV1ConfigVersion.cmake
	VERSION ${lib_version} 
	COMPATIBILITY AnyNewerVersion)
	
install(TARGETS RKV1 
		ARCHIVE DESTINATION lib 
		INCLUDES DESTINATION include
		FILE libRKV1Config.cmake
		NAMESPACE libRKV1::
		DESTINATION lib/cmake/libRKV1)
	
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/libRKV1Config.cmake"
			  "${CMAKE_CURRENT_BINARY_DIR}/libRKV1ConfigVersion.cmake"
		DESTINATION lib/cmake/libKIni)