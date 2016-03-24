#include <video_annotator/interface/visualizer.h>

namespace video_annotator
{

KinectVideoWidget::KinectVideoWidget(const KinectDevice* device, QWidget *parent)
    : QOpenGLWidget(parent), device_(device)
{
    resize(1280, 480);
    move(200, 200);
}

}
