#ifndef CAMERA_H
#define CAMERA_H

#include <QOpenGLContext>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <la.h>
#include <QTreeWidgetItem>

class Camera
{
public:
    Camera();

    void  setPosition(glm::vec3);
    glm::vec3 getPosition();

    void setTarget(glm::vec3);
    glm::vec3 getTarget();

    glm::vec3 getUp();
    glm::vec3 getLeft();

    void  setTheta(float theta);
    float getTheta();

    void  setPhi(float phi);
    float getPhi();

    float getZoom();
    void  setZoom(float zoom);

    void  setScreenWidth(float screenWidth);
    float getScreenWidth();
    void  setScreenHeight(float screenHeight);
    float getScreenHeight();

    void  setFOVY(float fovy);

    void  setNearClip(float nearClip);
    void  setFarClip(float farClip);

    void  setViewMatrix(glm::mat4 matrix);
    void  setModelMatrix(glm::mat4 matrix);
    void  setProjMatrix(glm::mat4 matrix);
    glm::mat4 createViewProjMatrix();


private:
    glm::vec3 up;
    glm::vec3 target;

    float theta;
    float phi;
    float zoom;

    float screenWidth;
    float screenHeight;

    float fovy;

    float nearClip;
    float farClip;

    glm::mat4 viewMatrix;
    glm::mat4 modelMatrix;
    glm::mat4 projMatrix;
};

#endif // CAMERA_H
