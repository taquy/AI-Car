cmake_minimum_required(VERSION 2.8)

project (LaneDetection)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(source_files
	config.h
	config.cpp
	debug.h
	debug.cpp
	getparam.h
	getparam.cpp
	imageprocess.h
	imageprocess.cpp
	frameprocess.h
	frameprocess.cpp
	lane.h
	lane.cpp
	laneprocess.h
	laneprocess.cpp
	layer.h
	layer.cpp
	line.h
	line.cpp
	lineprocess.h
	lineprocess.cpp
	pointsprocess.h
	pointsprocess.cpp
	road.h
	road.cpp)

add_library(lanedetec ${source_files})

