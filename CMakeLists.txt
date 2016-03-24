project(video_annotator)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_PREFIX_PATH /playpen/jaesungp/Qt/5.6/gcc_64)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
include_directories(${PROJECT_SOURCE_DIR}/include)

# Compile Settings
set(CMAKE_CXX_FLAGS "-fopenmp -fPIC -std=c++0x ${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS_RELEASE "-fopenmp -fPIC -std=c++0x ${CMAKE_CXX_FLAGS_RELEASE}")

# EIGEN3
find_package(Eigen3 QUIET REQUIRED)
if(EIGEN3_FOUND)
  include_directories(${EIGEN3_INCLUDE_DIR})
  message(STATUS "Found EIGEN3 at ${EIGEN3_INCLUDE_DIR}")
else(EIGEN3_FOUND)
  message(SEND_ERROR "Cannot find EIGEN3")
endif(EIGEN3_FOUND)

# freenect
find_package(libfreenect REQUIRED)
if(libfreenect_FOUND)
  include_directories(${FREENECT_INCLUDE_DIRS})
  message(STATUS "Found freenect at ${FREENECT_INCLUDE_DIRS}")
else(libfreenect_FOUND)
  message(SEND_ERROR "Cannot find freenect")
endif(libfreenect_FOUND)

include_directories("/usr/include/libusb-1.0/")

find_package(Threads REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

# Qt5
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
# Find the QtWidgets library
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5OpenGL REQUIRED)

# SOURCES
set(HEADERS
    include/video_annotator/kinect/kinect_device.h
    include/video_annotator/interface/visualizer.h
    include/video_annotator/util/mutex.h
)

set(SOURCES
    src/kinect/kinect_device.cpp
    src/interface/visualizer.cpp
)

add_library(video_annotator
    ${SOURCES}
    ${HEADERS}
)
target_link_libraries(video_annotator ${FREENECT_LIBRARIES} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})
qt5_use_modules(video_annotator Core Gui Widgets OpenGL)

add_executable(test_freenect src/test_freenect.cpp)
target_link_libraries(test_freenect video_annotator)

add_executable(video_recorder src/video_recorder.cpp)
target_link_libraries(video_recorder video_annotator)
