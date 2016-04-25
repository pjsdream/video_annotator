QT += core widgets opengl

PROJECT_DIR = $$PWD/..
SOURCES_DIR = $$PWD/../src
INCLUDE_DIR = $$PWD/../include
LIBS_DIR = $$PWD/../lib

INCLUDEPATH += $${INCLUDE_DIR}

win32 {
    # specify Kinect-installed folder
    INCLUDEPATH += $$quote(C:/Program Files/Microsoft SDKs/Kinect/v1.8/inc)
    LIBS += \
        "C:\Program Files\Microsoft SDKs\Kinect\v1.8\lib\amd64\Kinect10.lib" \
}
