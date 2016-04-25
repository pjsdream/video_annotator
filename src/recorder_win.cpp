#include <QApplication>
#include <video_annotator/interface/kinect_recorder.h>
#include <video_annotator/util/time.h>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    video_annotator::KinectDevice* device = new video_annotator::KinectDevice();
    video_annotator::KinectRecorderWindow* window = new video_annotator::KinectRecorderWindow(device);

    window->show();
    app.exec();
    return 0;
}
