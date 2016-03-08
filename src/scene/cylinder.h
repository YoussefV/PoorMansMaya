#pragma once

#include <scene/geometry.h>
#include <la.h>

#include <QOpenGLContext>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

class Cylinder : public Geometry
{
public:
    Cylinder();
    virtual void create();
};
