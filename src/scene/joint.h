#ifndef JOINT_H
#define JOINT_H

#include <vector>
#include <QTreeWidgetItem>
#include "shaderprogram.h"
#include <la.h>
#include <scene/geometry.h>


class Joint : public QTreeWidgetItem, public Geometry
{
public:
    Joint();
    Joint(QString name, Joint* parent, glm::vec3* position, glm::quat* rotation);
    ~Joint();

    //The name of this joint which will be displayed in your QTreeWidget of joints.
    QString name;

    virtual void create();
    void create(bool);

    bool loadJSON(QString path);

    void setChildren(std::vector<Joint*>);
    std::vector<Joint*> getChildren();

    void addChild(Joint* c);

    int getID();

//    bool isParentOf(Joint* c);
    void setParent(Joint*);
    Joint* getParent();

    void setPosition(glm::vec3*);
    glm::vec3* getPosition();

    void setRotation(glm::quat*);
    glm::quat* getRotation();

    void setBindMatrix();
    glm::mat4* getBindMatrix();

    void getTransMats(glm::mat4[]);
    void getBindMats(glm::mat4[]);

    glm::mat4 getLocalTransformation();
    glm::mat4 getOverallTransformation();

    void draw(GLWidget277 &f, ShaderProgram &prog_lambert);
    virtual GLenum drawMode();


private:
    static int currID;
    //Helper method to automatically add children
    void initWithJSON(QJsonObject jointJSON, Joint* parent);

    //Parent: The joint that is the parent of this joint.
    Joint* parent;
    //ID: This identifies a joint, will be used with the GPU
    int id;
    //Children: The set of joints that have this joint as their parent.
    std::vector<Joint*> children;
    // Position: The position of this joint relative to its parent joint.
    glm::vec3* position;
    //Rotation: The quaternion that represents this joint's current orientation.
    glm::quat* rotation;
    //Bind Matrix: The inverse of the joint's compound transformation matrix at the time a mesh is bound to the joint's skeleton.
    glm::mat4* bindMatrix;
};

#endif // JOINT_H
