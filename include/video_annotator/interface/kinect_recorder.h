#ifndef KINECT_RECORDER_H
#define KINECT_RECORDER_H


#include <QMainWindow>

#include <video_annotator/interface/kinect_viewer.h>
#include <video_annotator/kinect/kinect_device.h>


namespace video_annotator
{

class KinectRecorderWindow : public QMainWindow
{
    Q_OBJECT

public:

    KinectRecorderWindow(KinectDevice* device);
    KinectRecorderWindow(int fps, KinectDevice* device);

private:

    int fps_;
    KinectDevice* device_;

    KinectViewerWidget* widget_viewer_;
};

}


#endif // KINECT_RECORDER_H
