#ifndef OPENGL_WIDGET_H
#define OPENGL_WIDGET_H


#include <QOpenGLWidget>
#include <QOpenGLFunctions_2_1>


namespace video_annotator
{

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_2_1
{
public:

    OpenGLWidget(QWidget* parent = 0);

private:
};

}


#endif // OPENGL_WIDGET_H
