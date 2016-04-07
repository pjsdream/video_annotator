#include <video_annotator/interface/viewer.h>

namespace video_annotator
{

InteractiveViewerWidget::InteractiveViewerWidget(QWidget *parent)
    : InteractiveViewerWidget(30, parent)
{
}

InteractiveViewerWidget::InteractiveViewerWidget(int fps, QWidget *parent)
    : OpenGLWidget(parent)
    , fps_(fps)
    , timer_(new QTimer(this))
{
    timer_->setInterval(1.0 / fps_);
    connect(timer_, SIGNAL(timeout()), this, SLOT(update()));
    timer_->start();
}

void InteractiveViewerWidget::pause()
{
    timer_->blockSignals(true);
}

void InteractiveViewerWidget::resume()
{
    timer_->blockSignals(false);
}

}
