#pragma once

#include <scene/geometry.h>

#include <QOpenGLContext>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <la.h>

class Sphere : public Geometry
{
public:
    Sphere();
    void create();
};
