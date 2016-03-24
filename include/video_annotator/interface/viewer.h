#ifndef VIEWER_H
#define VIEWER_H


#include <video_annotator/interface/opengl_widget.h>


namespace video_annotator
{

class InteractiveViewerWidget : public OpenGLWidget
{
    Q_OBJECT

public:

    InteractiveViewerWidget(QWidget* parent = 0);
    InteractiveViewerWidget(int fps, QWidget* parent = 0);

private:

    int fps_;
};

}


#endif // VIEWER_H
