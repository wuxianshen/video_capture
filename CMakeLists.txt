cmake_minimum_required(VERSION 2.8)
project(video_capture)

if ( ${CMAKE_SYSTEM_PROCESSOR} STREQUAL "aarch64" )
  message ( STATUS " - System Processor : aarch64" )
  set(AARCH64 true)
elseif(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64" )
  message ( STATUS " - System Processor : x86_64" )
  set(X86_64 true)
endif()

if (CMAKE_SYSTEM_NAME MATCHES Linux)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -fPIC")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
endif ()

include_directories(inc)

include_directories(mw_capture_inc)
include_directories(mw_capture_inc/common)
include_directories(mw_capture_inc/LibMWCapture)
include_directories(mw_capture_inc/LibMWClosedCaption)

include_directories(/usr/include/)

FILE(GLOB MW_CAPTURE_SRCS src/*.cpp)

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

add_executable(${PROJECT_NAME} ${MW_CAPTURE_SRCS} src/main.cpp src/image_handler.cpp inc/image_handler.h)

target_link_libraries(${PROJECT_NAME} PUBLIC MWCapture)

#target_link_libraries(${PROJECT_NAME} PUBLIC /lib/x86_64-linux-gnu/libdl.so.2)
#target_link_libraries(${PROJECT_NAME} PUBLIC /lib/x86_64-linux-gnu/libudev.so.1)
#target_link_libraries(${PROJECT_NAME} PUBLIC /usr/lib/x86_64-linux-gnu/libv4l2.so.0)
#target_link_libraries(${PROJECT_NAME} PUBLIC /usr/lib/x86_64-linux-gnu/libasound.so.2)

target_link_libraries(${PROJECT_NAME} PUBLIC -lpthread)
target_link_libraries(${PROJECT_NAME} PUBLIC -ldl)
target_link_libraries(${PROJECT_NAME} PUBLIC -ludev)
target_link_libraries(${PROJECT_NAME} PUBLIC -lasound)
target_link_libraries(${PROJECT_NAME} PUBLIC -lv4l2)


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

