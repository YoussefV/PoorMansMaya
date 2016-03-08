#pragma once

#include <scene/geometry.h>
#include <la.h>

#include <QOpenGLContext>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

class Pipe : public Geometry
{
public:
    Pipe();
    virtual void create();
};
