#include <video_annotator/interface/viewer.h>

#include <QTimer>

namespace video_annotator
{

InteractiveViewerWidget::InteractiveViewerWidget(QWidget *parent)
    : InteractiveViewerWidget(30, parent)
{
}

InteractiveViewerWidget::InteractiveViewerWidget(int fps, QWidget *parent)
    : OpenGLWidget(parent)
    , fps_(fps)
{
    QTimer* timer = new QTimer(this);
    timer->setInterval(1.0 / fps_);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start();
}

}
