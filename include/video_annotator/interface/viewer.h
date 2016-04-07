#ifndef VIEWER_H
#define VIEWER_H


#include <video_annotator/interface/opengl_widget.h>

#include <QTimer>


namespace video_annotator
{

class InteractiveViewerWidget : public OpenGLWidget
{
    Q_OBJECT

public:

    InteractiveViewerWidget(QWidget* parent = 0);
    InteractiveViewerWidget(int fps, QWidget* parent = 0);

protected slots:

    void pause();
    void resume();

private:

    int fps_;
    QTimer* timer_;
};

}


#endif // VIEWER_H
