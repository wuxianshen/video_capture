cmake_minimum_required(VERSION 2.8)
project(video_capture)

if ( ${CMAKE_SYSTEM_PROCESSOR} STREQUAL "aarch64" )
  message ( STATUS " - System Processor : aarch64" )
  set(AARCH64 true)
elseif(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64" )
  message ( STATUS " - System Processor : x86_64" )
  set(X86_64 true)
endif()

set(CMAKE_BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/build/bin)

if (CMAKE_SYSTEM_NAME MATCHES Linux)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -fPIC")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
endif ()

set(OpenCV_DIR "~/work/softs/opencv-3.3.1/build")

include_directories(inc)
include_directories(object_detect/inc)

#########################
##  PCIE-HDMI-CAPTURE  ##
#########################
include_directories(mw_capture_inc)
include_directories(mw_capture_inc/common)
include_directories(mw_capture_inc/LibMWCapture)
include_directories(mw_capture_inc/LibMWClosedCaption)

include_directories(/usr/include/)

FILE(GLOB VIDEO_CAPTURE_SRCS src/*.cpp

                             object_detect/src/improcess.cpp
                             object_detect/src/obj_detect.cpp
        )

if ( AARCH64 )
  set(MW_LIB_PATH ${CMAKE_SOURCE_DIR}/mw_capture_lib/arm64)
elseif( X86_64 )
  set(MW_LIB_PATH ${CMAKE_SOURCE_DIR}/mw_capture_lib/x64)
else()
  set(MW_LIB_PATH ${CMAKE_SOURCE_DIR}/mw_capture_lib/i386)
endif()

link_directories(${MW_LIB_PATH})
link_directories(/usr/lib/x86_64-linux-gnu/)
link_directories(/lib/x86_64-linux-gnu/)
link_directories(${CMAKE_SOURCE_DIR}/object_detect/lib/)

add_executable(${PROJECT_NAME} ${VIDEO_CAPTURE_SRCS})

target_link_libraries(${PROJECT_NAME} PUBLIC MWCapture)

target_link_libraries(${PROJECT_NAME} PUBLIC -lpthread)
target_link_libraries(${PROJECT_NAME} PUBLIC -ldl)
target_link_libraries(${PROJECT_NAME} PUBLIC -ludev)
target_link_libraries(${PROJECT_NAME} PUBLIC -lasound)
target_link_libraries(${PROJECT_NAME} PUBLIC -lv4l2)

target_link_libraries(${PROJECT_NAME} PUBLIC -ldarknet)


find_package( OpenCV QUIET )
if (OpenCV_FOUND)
  message( STATUS "Found OpenCV installed in the system, will use it to display image in AdvancedSensing APIs")
  message( STATUS " - Includes: ${OpenCV_INCLUDE_DIRS}")
  message( STATUS " - Libraries: ${OpenCV_LIBRARIES}")
  add_definitions(-DOPEN_CV_INSTALLED)

  target_link_libraries(${PROJECT_NAME} PUBLIC ${OpenCV_LIBRARIES})
else()
  message( STATUS "Did not find OpenCV in the system, image data is inside RecvContainer as raw data")
endif ()

