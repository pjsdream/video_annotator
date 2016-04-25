#include <video_annotator/interface/opengl_widget.h>

namespace video_annotator
{

OpenGLWidget::OpenGLWidget(QWidget* parent)
    : QOpenGLWidget(parent)
{
    QSurfaceFormat format;
    format.setVersion(2, 1);
    format.setDepthBufferSize(24);
    //format.setProfile(QSurfaceFormat::CompatibilityProfile);
    setFormat(format);
}

}
