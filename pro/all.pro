TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS = \
    video_annotator \
    recorder \
    annotator \

recorder.depends = video_annotator
annotator.depends = video_annotator
