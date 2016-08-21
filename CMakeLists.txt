cmake_minimum_required(VERSION 2.6)
project(astarcpp)

list(APPEND CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS} -g -ftest-coverage -fprofile-arcs")

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_HOME_DIRECTORY}/bin)


# DEFINE LIBRARIES ########################################################

set(ASTAR_SOURCES
	src/astar.h
	src/astar.cpp)

# DEFINE VORONOI TESTS #####################################################

set(ASTAR_BASIC_SEARCH_TEST
	src/test/tilemap-search-test/matrix.h
	src/test/tilemap-search-test/main.cpp)

add_executable(basic-search-test
	${ASTAR_SOURCES}
	${ASTAR_BASIC_SEARCH_TEST})


