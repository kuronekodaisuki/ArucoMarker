cmake_minimum_required(VERSION 3.0)

project(ArucoMarker CXX)

add_executable(ArucoMarker main.cpp)

find_package(OpenCV REQUIRED)

if(OpenCV_FOUND)
	target_include_directories(ArucoMarker
		PRIVATE
		$(OpenCV_INCLUDE_DIRS)
		)
	target_link_libraries(ArucoMarker
		$(OpenCV_LIBS)
		)
endif(OpenCV_FOUND)

