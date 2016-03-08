#pragma once

#include <scene/geometry.h>

#include <QOpenGLContext>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <la.h>

class Cube : public Geometry
{
public:
    Cube();
    virtual void create();
};
