TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS = \
    video_annotator \
    recorder \

recorder.depends = video_annotator
