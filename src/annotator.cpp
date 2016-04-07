#include <QApplication>
#include <video_annotator/interface/video_annotator.h>

int main(int argc, char** argv)
{
    QApplication a(argc, argv);
    video_annotator::VideoAnnotatorWindow* window = new video_annotator::VideoAnnotatorWindow();
    window->show();

    a.exec();
    return 0;
}
