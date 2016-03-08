#include "node.h"
#include "cube.h"
#include "sphere.h"
#include <shaderprogram.h>
#include <la.h>

#ifndef CHARACTER_H
#define CHARACTER_H


class Character
{
public:
    Character(Cube *cube, Sphere *sphere);
    ~Character(){delete root;}
//    void rotateJointX(QString jointName, float angle);
    virtual void draw(GLWidget277 &f, ShaderProgram &prog_lambert);
    Node* root;

private:
//    void drawNode(Node* n, GLWidget277 &f, ShaderProgram &prog_lambert);
    void drawNode2(Node* n, GLWidget277 &f, ShaderProgram &prog_lambert);

};

#endif // CHARACTER_H
