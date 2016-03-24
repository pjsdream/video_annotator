#include <video_annotator/interface/kinect_viewer.h>

#include <QTimer>

namespace video_annotator
{

KinectViewerWidget::KinectViewerWidget(QWidget *parent)
    : KinectViewerWidget(30, parent)
{
}

KinectViewerWidget::KinectViewerWidget(int fps, QWidget *parent)
    : InteractiveViewerWidget(fps, parent)
{
    resize(640, 480);
    move(100, 100);
}

void KinectViewerWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0, 1, 0, 1);
}

void KinectViewerWidget::resizeGL(int w, int h)
{
}

void KinectViewerWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

}
