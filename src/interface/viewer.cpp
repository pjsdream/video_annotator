#include <video_annotator/interface/viewer.h>

#include <video_annotator/util/time.h>

#include <QMouseEvent>

namespace video_annotator
{

// ImageViewerWidget
ImageViewerWidget::ImageViewerWidget(QWidget* parent)
    : OpenGLWidget(parent)
{
}

void ImageViewerWidget::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0);
    glDepthFunc(GL_LESS);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glShadeModel(GL_SMOOTH);
    glGenTextures(1, &gl_tex_);
    glBindTexture(GL_TEXTURE_2D, gl_tex_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void ImageViewerWidget::resizeGL(int w, int h)
{
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho (0, 640, 480, 0, -1.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
}

void ImageViewerWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, gl_tex_);

    glBegin(GL_TRIANGLE_FAN);
    glColor4f(255.0f, 255.0f, 255.0f, 255.0f);
    glTexCoord2f(0, 0); glVertex3f(0,0,0);
    glTexCoord2f(1, 0); glVertex3f(640,0,0);
    glTexCoord2f(1, 1); glVertex3f(640,480,0);
    glTexCoord2f(0, 1); glVertex3f(0,480,0);
    glEnd();
}

void ImageViewerWidget::updateRGBImage(const std::vector<uint8_t>& rgb)
{
    makeCurrent();

    glBindTexture(GL_TEXTURE_2D, gl_tex_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 640, 480, 0, GL_RGB, GL_UNSIGNED_BYTE, &rgb[0]);

    doneCurrent();
    update();
}

void ImageViewerWidget::updateDepthImage(const std::vector<uint16_t>& depth)
{
    std::vector<uint8_t> depth_intensity(depth.size() * 3);
    for (int i=0; i<depth.size(); i++)
    {
        const uint8_t intensity = depth[i] / (4096. / 255.);
        depth_intensity[3*i  ] = intensity;
        depth_intensity[3*i+1] = intensity;
        depth_intensity[3*i+2] = intensity;
    }

    makeCurrent();

    glBindTexture(GL_TEXTURE_2D, gl_tex_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 640, 480, 0, GL_RGB, GL_UNSIGNED_BYTE, &depth_intensity[0]);

    doneCurrent();
    update();
}


// Viewer3DWidget
Viewer3DWidget::Viewer3DWidget(QWidget* parent)
    : OpenGLWidget(parent)
{
}

void Viewer3DWidget::mousePressEvent(QMouseEvent *event)
{
    last_mouse_ = event->pos();
}

void Viewer3DWidget::mouseMoveEvent(QMouseEvent *event)
{
    const QPoint current_mouse = event->pos();
    const QPoint mouse_move = current_mouse - last_mouse_;
    const Qt::MouseButtons buttons = event->buttons();

    if (buttons == Qt::LeftButton)
    {
        camera_->rotateOnScreen(mouse_move.x(), mouse_move.y());
        update();
    }
    else if (buttons == Qt::RightButton)
    {
        camera_->translateOnScreen(mouse_move.x(), mouse_move.y());
        update();
    }
    else if (buttons == Qt::LeftButton | Qt::RightButton)
    {
        camera_->zoomOnScreen(mouse_move.x(), mouse_move.y());
        update();
    }

    last_mouse_ = current_mouse;
}


// InteractiveViewerWidget
InteractiveViewerWidget::InteractiveViewerWidget(QWidget *parent)
    : InteractiveViewerWidget(30, parent)
{
}

InteractiveViewerWidget::InteractiveViewerWidget(int fps, QWidget *parent)
    : OpenGLWidget(parent)
    , fps_(fps)
    , timer_(new QTimer(this))
{
    timer_->setTimerType(Qt::PreciseTimer);
    timer_->setInterval(1000.0 / fps_);
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
