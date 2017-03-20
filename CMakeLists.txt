cmake_minimum_required(VERSION 2.6)
project(astarcpp)

list(APPEND CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS} -g -ftest-coverage -fprofile-arcs")

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_HOME_DIRECTORY}/bin)

set(ASTAR_SOURCES
	src/astar.h
	src/astar.cpp)

include_directories(src)

# BASIC ASTAR TESTS ############################################################

if(ASTARCPP_ENABLE_TESTS)
	set(ASTAR_TEST_COMMON
		src/test/common/matrix.h
		src/test/common/tilemap-problem.h)

	set(ASTAR_BASIC_SEARCH_TEST
		src/test/tilemap-search-test/main.cpp)

	add_executable(basic-search-test
		${ASTAR_SOURCES}
		${ASTAR_TEST_COMMON}
		${ASTAR_BASIC_SEARCH_TEST})


	# CAIRO TESTS

	include(FindPkgConfig)

	PKG_CHECK_MODULES(CAIRO REQUIRED cairo)
	PKG_CHECK_MODULES(SDL2 REQUIRED sdl2)

	set(ASTAR_TILEMAP_CAIRO_TEST
		src/test/tilemap-cairo-test/main.cpp
		src/test/tilemap-cairo-test/tilemap-cairo-test.h
		src/test/tilemap-cairo-test/tilemap-cairo-test.cpp)

	add_executable(tilemap-cairo-test
		${ASTAR_SOURCES}
		${ASTAR_TEST_COMMON}
		${ASTAR_TILEMAP_CAIRO_TEST})


	include_directories(tilemap-cairo-test
		${CAIRO_INCLUDE_DIRS}
		${SDL2_INCLUDE_DIRS}
	)

	link_directories(tilemap-cairo-test
		${CAIRO_LIBRARY_DIRS}
		${SDL2_LIBRARY_DIRS}
	)

	target_link_libraries(tilemap-cairo-test
		${CAIRO_LIBRARIES}
		${SDL2_LIBRARIES}
	)

endif(ASTARCPP_ENABLE_TESTS)


