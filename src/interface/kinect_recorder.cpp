#include <video_annotator/interface/kinect_recorder.h>


namespace video_annotator
{

KinectRecorderWindow::KinectRecorderWindow(KinectDevice* device)
    : KinectRecorderWindow(30, device)
{
}

KinectRecorderWindow::KinectRecorderWindow(int fps, KinectDevice* device)
    : QMainWindow()
    , fps_(fps)
    , device_(device)
    , widget_viewer_(new KinectViewerWidget(fps, device, this))
{
    resize(1280, 480);
    move(100, 100);
    setCentralWidget(widget_viewer_);
}

}
