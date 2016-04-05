#include <QApplication>
#include <video_annotator/interface/kinect_recorder.h>
#include <video_annotator/util/time.h>

int main(int argc, char** argv)
{
    Freenect::Freenect freenect;

    printf("setting up the device..."); fflush(stdout);
    video_annotator::KinectDevice* device = &freenect.createDevice<video_annotator::KinectDevice>(0);
    printf("complete\n"); fflush(stdout);

    device->startVideo();
    device->startDepth();
    printf("kinect started\n"); fflush(stdout);

    //device->startRecord("/playpen/jaesungp/tmp.txt");

    QApplication a(argc, argv);
    video_annotator::KinectRecorderWindow* window = new video_annotator::KinectRecorderWindow(30, device);
    window->show();

    a.exec();

    device->stopVideo();
    device->stopDepth();
    return 0;
}
