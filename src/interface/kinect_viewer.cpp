#include <video_annotator/interface/kinect_viewer.h>

#include <video_annotator/util/time.h>

#include <QTimer>

namespace video_annotator
{

KinectViewerWidget::KinectViewerWidget(KinectDevice* device, QWidget *parent)
    : KinectViewerWidget(30, device, parent)
{
}

KinectViewerWidget::KinectViewerWidget(int fps, KinectDevice* device, QWidget *parent)
    : InteractiveViewerWidget(fps, parent)
    , device_(device)
{
    resize(1280, 480);
    move(100, 100);
}

void KinectViewerWidget::initializeGL()
{
    initializeOpenGLFunctions();

    printf("OpenGL version: %s\n", glGetString(GL_VERSION));

    glClearColor(0.0f, 1.0f, 0.0f, 0.0f);
    glClearDepth(1.0);
    glDepthFunc(GL_LESS);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glShadeModel(GL_SMOOTH);
    glGenTextures(1, &gl_depth_tex_);
    glBindTexture(GL_TEXTURE_2D, gl_depth_tex_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenTextures(1, &gl_rgb_tex_);
    glBindTexture(GL_TEXTURE_2D, gl_rgb_tex_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void KinectViewerWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 1280, 480, 0, -1.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
}

void KinectViewerWidget::paintGL()
{
    std::vector<uint8_t> rgb;
    std::vector<uint16_t> depth;

    device_->getRGBD(rgb, depth);

    std::vector<uint8_t> depth_intensity(depth.size() * 3);

    for (int i=0; i<depth.size(); i++)
    {
        const uint8_t intensity = depth[i] / (4096. / 255.);
        depth_intensity[3*i  ] = intensity;
        depth_intensity[3*i+1] = intensity;
        depth_intensity[3*i+2] = intensity;
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, gl_depth_tex_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 640, 480, 0, GL_RGB, GL_UNSIGNED_BYTE, &depth_intensity[0]);

    glBegin(GL_TRIANGLE_FAN);
    glColor4f(1, 1, 1, 1);
    glTexCoord2f(0, 0); glVertex3f(0,0,0);
    glTexCoord2f(1, 0); glVertex3f(640,0,0);
    glTexCoord2f(1, 1); glVertex3f(640,480,0);
    glTexCoord2f(0, 1); glVertex3f(0,480,0);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, gl_rgb_tex_);
    if (device_->getVideoFormat() == KinectDevice::KINECT_VIDEO_RGB || device_->getVideoFormat() == KinectDevice::KINECT_VIDEO_YUV_RGB)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 640, 480, 0, GL_RGB, GL_UNSIGNED_BYTE, &rgb[0]);
    else
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 640, 480, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, &rgb[0]);

    glBegin(GL_TRIANGLE_FAN);
    glColor4f(1, 1, 1, 1);
    glTexCoord2f(0, 0); glVertex3f(640,0,0);
    glTexCoord2f(1, 0); glVertex3f(1280,0,0);
    glTexCoord2f(1, 1); glVertex3f(1280,480,0);
    glTexCoord2f(0, 1); glVertex3f(640,480,0);
    glEnd();

    // print time
    static double last = time();
    double curr = time();
    printf("%lf\n", (curr - last) * 1000);
    last = curr;
    fflush(stdout);
}

}
