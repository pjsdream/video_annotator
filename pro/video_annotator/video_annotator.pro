include(../common.pri)

TEMPLATE = lib
CONFIG += staticlib
DESTDIR = $${PROJECT_DIR}/lib

HEADERS = \
    $${INCLUDE_DIR}/video_annotator/kinect/kinect_device.h \
    $${INCLUDE_DIR}/video_annotator/interface/camera.h \
    $${INCLUDE_DIR}/video_annotator/interface/viewer.h \
    $${INCLUDE_DIR}/video_annotator/interface/point_cloud_viewer.h \
    $${INCLUDE_DIR}/video_annotator/interface/kinect_viewer.h \
    $${INCLUDE_DIR}/video_annotator/interface/kinect_recorder.h \
    $${INCLUDE_DIR}/video_annotator/interface/video_annotator.h \
    $${INCLUDE_DIR}/video_annotator/interface/opengl_widget.h \
    $${INCLUDE_DIR}/video_annotator/data/rgbd_frames.h \
    $${INCLUDE_DIR}/video_annotator/util/mutex.h \
    $${INCLUDE_DIR}/video_annotator/util/time.h \

SOURCES = \
    $${SOURCES_DIR}/kinect/kinect_device.cpp \
    $${SOURCES_DIR}/interface/camera.cpp \
    $${SOURCES_DIR}/interface/viewer.cpp \
    $${SOURCES_DIR}/interface/point_cloud_viewer.cpp \
    $${SOURCES_DIR}/interface/kinect_viewer.cpp \
    $${SOURCES_DIR}/interface/kinect_recorder.cpp \
    $${SOURCES_DIR}/interface/video_annotator.cpp \
    $${SOURCES_DIR}/interface/opengl_widget.cpp \
    $${SOURCES_DIR}/data/rgbd_frames.cpp \

LIBS += \
    opengl32.lib \
