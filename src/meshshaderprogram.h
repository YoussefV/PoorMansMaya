#ifndef MESHSHADERPROGRAM_H
#define MESHSHADERPROGRAM_H

#include <shaderprogram.h>
#include <scene/mesh.h>

class Mesh;

class MeshShaderProgram : public ShaderProgram
{
public:
    int unifSelectedID;

    int attrInfluences;
    int attrJointIDs;

    int unifTransMats[100];
    int unifBindMats[100];

public:
    void create(const char *vertfile, const char *fragfile);
    void setSelectedID(int);
    void setBindMatrices(glm::mat4 bindMats[100]);
    void setTransMatrices(glm::mat4 transMats[100]);
    virtual void draw(GLWidget277 &f, Mesh &d);

};

#endif // MESHSHADERPROGRAM_H
