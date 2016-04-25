include(../common.pri)

TEMPLATE = app
DESTDIR = $${PROJECT_DIR}/bin

HEADERS = \

SOURCES = \
    $${SOURCES_DIR}/recorder_win.cpp \

LIBS += \
    opengl32.lib \
    $${LIBS_DIR}/video_annotator.lib \

PRE_TARGETDEPS += \
    $${LIBS_DIR}/video_annotator.lib \
