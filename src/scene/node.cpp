#include "node.h"

Node::Node()
{
    tx = ty = tz = rx = ry = rz = 0;
    sx = sy = sz = 1;
    geom_color = glm::vec4(1,1,1,1);

    parent = NULL;
    geometry = NULL;
}

Node::Node(float tx, float ty, float tz, float rx, float ry, float rz, float sx, float sy, float sz, QString name){
    this->tx = tx; this->ty = ty; this->tz = tz;
    this->rx = rx; this->ry = ry; this->rz = rz;
    this->sx = sx; this->sy = sy; this->sz = sz;
    geom_color = glm::vec4(1,1,1,1);

    this->name = name;
    QTreeWidgetItem::setText(0,name);

    parent = NULL;
    geometry = NULL;
}

Node::~Node()
{
//    for(int i = children.size()-1; i >=0; i--)
//    {
//       // Node* n = children[i];
//        delete children[i];
//    }

    while(children.size() > 0){
        children.erase(children.begin());
        delete children[0];
   }

    children.clear();

    if (parent != NULL) {
        for (int i=0; i<parent->children.size();i++) {
            if (this == parent->children[i]) {
                parent->children.erase(parent->children.begin()+i);
            }
        }
    }
}

int Node::childCount(){
    return children.size();
}

Node* Node::childAt(int idx){
    return children[idx];
}

void Node::addChild(Node* c){
    children.push_back(c);
    c->setParent(this);
    QTreeWidgetItem::addChild(c);
}

Node* Node::getParent(){
   return parent;
}

void Node::setParent(Node* n){
    parent = n;
}

void Node::setGeometry(ShaderProgram::Drawable* g){
    geometry = g;
}

ShaderProgram::Drawable* Node::getGeometry(){
    return geometry;
}

void Node::setGeomColor(const glm::vec4& c){
    geom_color = c;
}

const glm::vec4& Node::getGeometryColor() const {
    return geom_color;
}

glm::mat4 Node::getTransMat(){
    glm::mat4 T = glm::translate(glm::mat4(1.0f), glm::vec3(tx, ty, tz));
    glm::mat4 Rx = glm::rotate(glm::mat4(1.0f), rx, glm::vec3(1, 0, 0));
    glm::mat4 Ry = glm::rotate(glm::mat4(1.0f), ry, glm::vec3(0, 1, 0));
    glm::mat4 Rz = glm::rotate(glm::mat4(1.0f), rz, glm::vec3(0, 0, 1));
    glm::mat4 S = glm::scale(glm::mat4(1.0f), glm::vec3(sx, sy, sz));

    return T * Rx * Ry * Rz * S;
}

glm::mat4 Node::getChildTransMat(Node* parent){
    glm::mat4 Parent;
    if (parent->getParent() != nullptr) {
        Parent = parent->getChildTransMat(parent->getParent());
    } else {
        Parent = parent->getTransMat();
    }

    glm::mat4 T = glm::translate(glm::mat4(1.0f), glm::vec3(tx, ty, tz));
    glm::mat4 Rx = glm::rotate(glm::mat4(1.0f), rx, glm::vec3(1, 0, 0));
    glm::mat4 Ry = glm::rotate(glm::mat4(1.0f), ry, glm::vec3(0, 1, 0));
    glm::mat4 Rz = glm::rotate(glm::mat4(1.0f), rz, glm::vec3(0, 0, 1));
    glm::mat4 S = glm::scale(glm::mat4(1.0f), glm::vec3(sx, sy, sz));

    return Parent * T * Rx * Ry * Rz * S;
}
