#include <QApplication>
#include <video_annotator/interface/kinect_viewer.h>

int main(int argc, char** argv)
{
    Freenect::Freenect freenect;

    printf("setting up the device..."); fflush(stdout);
    video_annotator::KinectDevice* device = &freenect.createDevice<video_annotator::KinectDevice>(0);
    printf("complete\n"); fflush(stdout);

    QApplication a(argc, argv);
    video_annotator::KinectViewerWidget* widget = new video_annotator::KinectViewerWidget(30);
    widget->show();

    a.exec();
    return 0;
}
