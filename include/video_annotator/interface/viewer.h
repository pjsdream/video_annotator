#ifndef VIEWER_H
#define VIEWER_H


#include <video_annotator/interface/opengl_widget.h>
#include <video_annotator/interface/camera.h>

#include <QTimer>


namespace video_annotator
{

class ImageViewerWidget : public OpenGLWidget
{
    Q_OBJECT

public:

    ImageViewerWidget(QWidget* parent = 0);

protected:

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

public slots:

    void updateRGBImage(const std::vector<uint8_t>& rgb);
    void updateDepthImage(const std::vector<uint16_t>& depth);
    void updatePoints(const std::vector<double>& points);

private:

    GLuint gl_tex_;

    std::vector<double> points_;
};


class Viewer3DWidget : public OpenGLWidget
{
    Q_OBJECT

public:

    Viewer3DWidget(QWidget* parent = 0);

protected:

    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);

private:

    Camera* camera_;

    QPoint last_mouse_;
};


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
