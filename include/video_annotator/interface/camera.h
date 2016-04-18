#ifndef CAMERA_H
#define CAMERA_H


//#include <Eigen/Dense>


namespace video_annotator
{

class Camera
{
public:

    Camera();

    void rotateOnScreen(int x, int y);
    void translateOnScreen(int x, int y);
    void zoomOnScreen(int x, int y);

    //Eigen::Matrix4d projection();
    //Eigen::Matrix4d view();

private:
};

}


#endif // CAMERA_H
