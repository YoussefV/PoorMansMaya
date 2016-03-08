#pragma once

#include <vector>
#include <QTreeWidgetItem>
#include "shaderprogram.h"
#include <la.h>

class Node : public QTreeWidgetItem
{
private:
    Node* parent;
    std::vector<Node*> children;
    ShaderProgram::Drawable* geometry;
    glm::vec4 geom_color;

    float tx, ty, tz;
    float rx, ry, rz;
    float sx, sy, sz;
public:
    Node();
    Node(float tx, float ty, float tz, float rx, float ry, float rz, float sx, float sy, float sz, QString name);
    ~Node();
    QString name;

    float getTx(){return tx;} float getTy(){return ty;} float getTz(){return tz;}
    float getRx(){return rx;} float getRy(){return ry;} float getRz(){return rz;}
    float getSx(){return sx;} float getSy(){return sy;} float getSz(){return sz;}

    void setTx(double tx){this->tx = tx;} void setTy(double ty){this->ty = ty;} void setTz(double tz){this->tz = tz;}
    void setRx(double rx){this->rx = rx;} void setRy(double ry){this->ry = ry;} void setRz(double rz){this->rz = rz;}
    void setSx(double sx){this->sx = sx;} void setSy(double sy){this->sy = sy;} void setSz(double sz){this->sz = sz;}

    int childCount();
    Node* childAt(int idx);
    void addChild(Node* c);
    Node* getParent();
    void setParent(Node* n);
    void setGeomColor(const glm::vec4& c);
    const glm::vec4& getGeometryColor() const;

    void setGeometry(ShaderProgram::Drawable* g);
    ShaderProgram::Drawable* getGeometry();

    glm::mat4 getTransMat();
    glm::mat4 getChildTransMat(Node*);
};
