#pragma once

#include <scene/geometry.h>

#include <QOpenGLContext>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <la.h>

#ifndef CONE_H
#define CONE_H


class Cone : public Geometry
{
public:
    Cone();
    virtual void create();
};

#endif // CONE_H
