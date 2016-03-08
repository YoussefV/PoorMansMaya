#include "camera.h"

#include <QOpenGLContext>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <la.h>
#include <math.h>
#define PI 3.14159265

Camera::Camera()
{
    screenWidth = 1280;
    screenHeight = 720;

    fovy = 45.0f;

    nearClip = 1.0f;
    farClip = 200.0f;

    //Setting View Matrix
    setTheta(45.0f);
    setPhi(80.0f);
    setZoom(15.0f);

    modelMatrix = glm::mat4(1.0f);

     //Projection Matrix:        //FOVY       //AspectRatio         //NearC   //FarC
    projMatrix = glm::perspective(fovy, screenWidth / screenHeight, nearClip, farClip);

}

//Returns camera local up vector
glm::vec3 Camera::getUp() {
    return up;
}

//Returns camera local left vector
glm::vec3 Camera::getLeft() {

    float x = zoom * sin(phi * PI/180.0) * cos(theta * PI/180.0);
    float y = zoom * cos(phi * PI/180.0);
    float z = zoom * sin(phi * PI/180.0) * sin(theta * PI/180.0);

    glm::vec3 pos = glm::vec3(x,y,z);
    glm::vec3 forward = pos - target;
    glm::vec3 left = glm::cross(glm::vec3(0.0f, 1.0f, 0.0f),forward);

    return left;
}

//Sets camera's location
void Camera::setPosition(glm::vec3 pos) {

    float newZoom = sqrt(pow(pos.x,2) + pow(pos.y,2) + pow(pos.z,2));
    float newPhi = acos (pos.y/zoom) * 180.0 / PI;
    float newTheta = atan(pos.z/pos.x) * 180.0 / PI;

    zoom = newZoom;
    phi = newPhi;
    theta = newTheta;

    glm::vec3 forward = pos - target;
    glm::vec3 left = glm::cross(glm::vec3(0.0f, 1.0f, 0.0f),forward);
    up = glm::cross(forward,left);

    viewMatrix = glm::lookAt(pos, //Camera Position
                 target,  //Camera Target
                 up); //Camera Up Vector
}

glm::vec3 Camera::getPosition() {
    float x = zoom * sin(phi * PI/180.0) * cos(theta * PI/180.0);
    float y = zoom * cos(phi * PI/180.0);
    float z = zoom * sin(phi * PI/180.0) * sin(theta * PI/180.0);

    return glm::vec3(x,y,z);
}

void Camera::setTarget(glm::vec3 target) {

    this-> target = target;

    float x = zoom * sin(phi * PI/180.0) * cos(theta * PI/180.0);
    float y = zoom * cos(phi * PI/180.0);
    float z = zoom * sin(phi * PI/180.0) * sin(theta * PI/180.0);

    glm::vec3 pos = glm::vec3(x,y,z);
    glm::vec3 forward = pos - target;
    glm::vec3 left = glm::cross(glm::vec3(0.0f, 1.0f, 0.0f),forward);
    up = glm::cross(forward,left);

    viewMatrix = glm::lookAt(pos, //Camera Position
                 target,  //Camera Target
                 up); //Camera Up Vector
}

glm::vec3 Camera::getTarget() {
    return target;
}

void Camera::setTheta(float theta) {

    this->theta = theta;

    float x = zoom * sin(phi * PI/180.0) * cos(theta * PI/180.0);
    float y = zoom * cos(phi * PI/180.0);
    float z = zoom * sin(phi * PI/180.0) * sin(theta * PI/180.0);

    glm::vec3 pos = glm::vec3(x,y,z);
    glm::vec3 forward = pos - target;
    glm::vec3 left = glm::cross(glm::vec3(0.0f, 1.0f, 0.0f),forward);
    up = glm::cross(forward,left);

    viewMatrix = glm::lookAt(pos, //Camera Position
                 target,  //Camera Target
                 up); //Camera Up Vector
}

float Camera::getTheta() {
    return theta;
}

void Camera::setPhi(float phi) {

    this->phi = phi;

    float x = zoom * sin(phi * PI/180.0) * cos(theta * PI/180.0);
    float y = zoom * cos(phi * PI/180.0);
    float z = zoom * sin(phi * PI/180.0) * sin(theta * PI/180.0);

    glm::vec3 pos = glm::vec3(x,y,z);
    glm::vec3 forward = pos - target;
    glm::vec3 left = glm::cross(glm::vec3(0.0f, 1.0f, 0.0f),forward);
    up = glm::cross(forward,left);

    viewMatrix = glm::lookAt(pos, //Camera Position
               target, //Camera Target
               up); //Camera Up Vector
}

float Camera::getPhi() {
    return phi;
}

float Camera::getZoom() {
    return zoom;
}

void Camera::setZoom(float zoom) {

    this->zoom = zoom;

    float x = zoom * sin(phi * PI/180.0) * cos(theta * PI/180.0);
    float y = zoom * cos(phi * PI/180.0);
    float z = zoom * sin(phi * PI/180.0) * sin(theta * PI/180.0);

    glm::vec3 pos = glm::vec3(x,y,z);
    glm::vec3 forward = pos - target;
    glm::vec3 left = glm::cross(glm::vec3(0.0f, 1.0f, 0.0f),forward);
    up = glm::cross(forward,left);

    viewMatrix = glm::lookAt(pos, //Camera Position
               target, //Camera Target
               up); //Camera Up Vector
}

void Camera::setScreenWidth(float screenWidth) {
    this->screenWidth = screenWidth;
    projMatrix = glm::perspective(fovy, screenWidth / screenHeight, nearClip, farClip);
}

float Camera::getScreenWidth() {
    return screenWidth;
}

void Camera::setScreenHeight(float screenHeight) {
    this->screenHeight = screenHeight;
    projMatrix = glm::perspective(fovy, screenWidth / screenHeight, nearClip, farClip);
}

float Camera::getScreenHeight() {
    return screenHeight;
}

void Camera::setFOVY(float fovy) {
    this->fovy = fovy;
    projMatrix = glm::perspective(fovy, screenWidth / screenHeight, nearClip, farClip);
}

void Camera::setNearClip(float nearClip){
    this->nearClip = nearClip;
    projMatrix = glm::perspective(fovy, screenWidth / screenHeight, nearClip, farClip);
}

void Camera::setFarClip(float farClip){
    this->farClip = farClip;
    projMatrix = glm::perspective(fovy, screenWidth / screenHeight, nearClip, farClip);
}


void Camera::setViewMatrix(glm::mat4 matrix) {
    viewMatrix = matrix;
}

void Camera::setModelMatrix(glm::mat4 matrix) {
    modelMatrix = matrix;
}

void Camera::setProjMatrix(glm::mat4 matrix) {
    projMatrix = matrix;
}

glm::mat4 Camera::createViewProjMatrix() {
    return projMatrix * viewMatrix * modelMatrix;
}
