cmake_minimum_required(VERSION 2.8)

project( CarControl-0.3 )

find_package(OpenCV REQUIRED)
find_package(X11 REQUIRED)
set( OPENNI2_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/2-Linux/OpenNI-Linux-Arm-2.3/Include)
set( OPENNI2_REDIST  ${CMAKE_CURRENT_SOURCE_DIR}/2-Linux/OpenNI-Linux-Arm-2.3/Redist)

include_directories( ${OPENNI2_INCLUDE} )
link_directories ( ${OPENNI2_REDIST} )


if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)

  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY
               STRINGS "Debug" "Release")
endif()
IF(MSVC)
  ADD_DEFINITIONS(-DNOMINMAX)
  # Another MSVC quirk. http://stackoverflow.com/questions/6832666/lnk2019-when-including-asio-headers-solution-generated-with-cmake
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc /MP") # For multi-threaded compilation on MSVC
ELSE()
  IF(APPLE)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++") # C++11 support on Apple clang
  ELSE()
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")# C++11 support on GCC
  ENDIF()
ENDIF()

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE})
#set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE})
 
include_directories(
      ${OpenCV_INCLUDE_DIRS}
      ${CMAKE_CURRENT_SOURCE_DIR}/LIBSCAR/stereo_vision
      ${CMAKE_CURRENT_SOURCE_DIR}/LIBSCAR/peripheral_driver/i2c
      ${CMAKE_CURRENT_SOURCE_DIR}/LIBSCAR/HAL
	${CMAKE_CURRENT_SOURCE_DIR}/LIBSCAR/openni2
      ${CMAKE_CURRENT_SOURCE_DIR}/include
      ${CMAKE_CURRENT_SOURCE_DIR}/traffic)

add_subdirectory(LIBSCAR/peripheral_driver/i2c)
add_subdirectory(LIBSCAR/stereo_vision)
add_subdirectory(LIBSCAR/HAL)
add_subdirectory(LIBSCAR/openni2)
add_subdirectory(traffic)


#link_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_subdirectory(include)

add_executable(run main.cpp)
target_link_libraries(run i2c-pwm kinect-cv2 HAL openni2 OpenNI2 traffic lanedetec ${OpenCV_LIBS} X11 )
set_property(TARGET run PROPERTY CXX_STANDARD 11)








