#ifndef OPENGL_WIDGET_H
#define OPENGL_WIDGET_H


#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_3_Core>


namespace video_annotator
{

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_4_3_Core
{
public:

    OpenGLWidget(QWidget* parent = 0);

private:
};

}


#endif // OPENGL_WIDGET_H
