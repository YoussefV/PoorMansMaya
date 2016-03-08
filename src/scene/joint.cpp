#include "joint.h"
#include <QTreeWidgetItem>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <iostream> //404

static const int WF_IDX_COUNT = 72+2;
static const int WF_VERT_COUNT = 36+2;
static const int WF_COL_COUNT = 36+2;
int Joint::currID = 0;

//Default Constructor
Joint::Joint()
{
    name = QString("Joint");
    parent = NULL;

    position = new glm::vec3(0,0,0);
    rotation = new glm::quat(1,0,0,0);

    this->id = currID;
    currID++;

    bindMatrix = nullptr;

    setText(0,name);
}

//Constructor w/ Parameters
Joint::Joint(QString name, Joint *parent, glm::vec3* position, glm::quat* rotation) {
    this->name = name;
    this->parent = parent;

    this->position = position;
    this->rotation = rotation;

    this->id = currID;
    currID++;

    bindMatrix = nullptr;
}

//Delete Method
Joint::~Joint() {

    while(children.size() > 0) {
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

void Joint::addChild(Joint* c) {
    children.push_back(c);
    c->setParent(this);
    QTreeWidgetItem::addChild(c);
    c->create();
}

//                                 JSON Info        Parent!
void Joint::initWithJSON(QJsonObject jointJSON, Joint* parent){
    QJsonArray jointPosition = jointJSON["pos"].toArray();
    QJsonArray jointRotation = jointJSON["rot"].toArray();
    QJsonArray jointChildren = jointJSON["children"].toArray();

    QString name = jointJSON["name"].toString();

    glm::vec3* position = new glm::vec3(
                jointPosition[0].toDouble(),
                jointPosition[1].toDouble(),
                jointPosition[2].toDouble());
    double angle = jointRotation[0].toDouble();
    glm::vec3 axis = glm::vec3(
                jointRotation[1].toDouble(),
                jointRotation[2].toDouble(),
                jointRotation[3].toDouble());
    glm::quat* rotation = new glm::quat(glm::angleAxis(float(angle),axis));

    this->name = name;
    setText(0,name);
    this->position = position;
    this->rotation = rotation;
    setText(0,QString(QString::number(id) + " - " + name)); //Sets Text to "id - name"
    create();


    for (int i=0; i < jointChildren.size(); i++) {
        Joint* child = new Joint();
        child->initWithJSON(jointChildren[i].toObject(),this);
    }

    if (parent!= NULL) {
        parent->addChild(this);
    }

}

int Joint::getID() {
    return id;
}

void Joint::getTransMats(glm::mat4 matArray[100]) {
    matArray[id] = getOverallTransformation();
    for (int i = 0; i<childCount(); i++) {
        matArray[children[i]->getID()] = children[i]->getOverallTransformation();
        if (children[i]->childCount() > 0) {
            children[i]->getTransMats(matArray);
        }
    }
}

void Joint::getBindMats(glm::mat4 matArray[100]) {
    matArray[id] = *bindMatrix;
    for (int i = 0; i<childCount(); i++) {
        matArray[children[i]->getID()] = *children[i]->getBindMatrix();
        if (children[i]->childCount() > 0) {
            children[i]->getBindMats(matArray);
        }
    }
}


//int Joint::size() {
//    int size = 0;
//    for (int i=0; i<childCount(); i++) {
//        if (children[i]->childCount() > 0) {
//            size = children[i]->size() > size ? children[i]->size() : size;
//        }
//        size = children[i]->getID() > size ? children[i]->getID() : size;
//    }
//    size = id > size ? id : size;

//    return size+1;
//}

bool Joint::loadJSON(QString path) {
    QString val;
    QFile file;
    file.setFileName(path);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    val = file.readAll();
    file.close();
    QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());
    QJsonObject object = d.object();
    QJsonObject jointJSON = object["root"].toObject();
//    QString jointName = jointJSON["name"].toString();
//    QJsonArray jointPos = jointJSON["pos"].toArray();

    initWithJSON(jointJSON,NULL);

    return true;
}

//Drawing Joints
void Joint::draw(GLWidget277 &f, ShaderProgram &prog_wire) {
    prog_wire.setModelMatrix(getOverallTransformation());
    prog_wire.draw(f,*this);

    if (children.size()>0) {
        for (int i = 0; i < children.size(); i++) {
            children[i]->draw(f,prog_wire);
        }
    }
}

//Children Methods
void Joint::setChildren(std::vector<Joint*> children) {
    this->children = children;
}
std::vector<Joint*> Joint::getChildren() {
    return children;
}

//Parent Methods
void Joint::setParent(Joint* parent) {
    this->parent = parent;
}
Joint* Joint::getParent() {
    return parent;
}

//Position Methods
void Joint::setPosition(glm::vec3* position) {
    this->position = position;
}
glm::vec3* Joint::getPosition() {
    return position;
}

//Rotation Methods
void Joint::setRotation(glm::quat* rotation) {
    this->rotation = rotation;
}
glm::quat* Joint::getRotation() {
    return rotation;
}

void Joint::setBindMatrix() {
    glm::mat4* mat = new glm::mat4(glm::inverse(getOverallTransformation()));
    this->bindMatrix = mat;
}

glm::mat4* Joint::getBindMatrix() {
    return bindMatrix;
}

glm::mat4 Joint::getLocalTransformation() {
    glm::mat4 T  = glm::translate(glm::mat4(1.0f), *position);
    glm::mat4 R = glm::mat4_cast(*rotation);
    glm::mat4 S  = glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, 1));

    return T * R * S;
}


glm::mat4 Joint::getOverallTransformation() {
    if (parent != NULL) {
        return parent-> getOverallTransformation() * getLocalTransformation();
    } else {
        return getLocalTransformation();
    }
}

//WIREFRAME PART
void createWireframeVertexPositions(glm::vec4 (&wf_vert_pos)[WF_VERT_COUNT]){
    for (int i= 0; i<12; i++) {
        glm::vec4 v = glm::rotate(glm::mat4(1.0f), i*30.0f, glm::vec3(0, 0, 1)) * glm::vec4(0,0.5f,0,1);
        wf_vert_pos[i] = v;
    }
    for (int i= 12; i<24; i++) {
        glm::vec4 v = glm::rotate(glm::mat4(1.0f), i*30.0f, glm::vec3(1, 0, 0)) * glm::vec4(0,0.5f,0,1);;
        wf_vert_pos[i] = v;
    }
    for (int i= 24; i<36; i++) {
        glm::vec4 v = glm::rotate(glm::mat4(1.0f), i*30.0f, glm::vec3(0, 1, 0)) * glm::vec4(0.5f,0,0,1);;
        wf_vert_pos[i] = v;
    }
    //If no parent
    wf_vert_pos[36] = glm::vec4(0,0,0,1);
    wf_vert_pos[37] = glm::vec4(0,0,0,1);
}

void createWireframeVertexPositions(glm::vec4 (&wf_vert_pos)[WF_VERT_COUNT], Joint* parent, Joint* self){
    for (int i= 0; i<12; i++) {
        glm::vec4 v = glm::rotate(glm::mat4(1.0f), i*30.0f, glm::vec3(0, 0, 1)) * glm::vec4(0,0.5f,0,1);
        wf_vert_pos[i] = v;
    }
    for (int i= 12; i<24; i++) {
        glm::vec4 v = glm::rotate(glm::mat4(1.0f), i*30.0f, glm::vec3(1, 0, 0)) * glm::vec4(0,0.5f,0,1);;
        wf_vert_pos[i] = v;
    }
    for (int i= 24; i<36; i++) {
        glm::vec4 v = glm::rotate(glm::mat4(1.0f), i*30.0f, glm::vec3(0, 1, 0)) * glm::vec4(0.5f,0,0,1);;
        wf_vert_pos[i] = v;
    }

    wf_vert_pos[36] = glm::vec4(0,0,0,1);
    wf_vert_pos[37] = glm::inverse(self->getLocalTransformation()) * glm::vec4(0,0,0,1);
}

void createWireframeIndices(GLuint (&wf_idx)[WF_IDX_COUNT]) {
    for (int i= 0; i<22; i+=2) {
        wf_idx[i] = i/2;
        wf_idx[i+1] = (i/2) + 1;
    }
    wf_idx[22] = 11;
    wf_idx[23] = 0;

    for (int i= 24; i<46; i+=2) {
        wf_idx[i] = i/2;
        wf_idx[i+1] = (i/2) + 1;
    }
    wf_idx[46] = 23;
    wf_idx[47] = 12;

    for (int i= 48; i<70; i+=2) {
        wf_idx[i] = i/2;
        wf_idx[i+1] = (i/2) + 1;
    }
    wf_idx[70] = 35;
    wf_idx[71] = 24;

    //Last two
    wf_idx[72] = 36;
    wf_idx[73] = 37;

}

void createWireframeColors(glm::vec4 (&wf_col)[WF_COL_COUNT]) {
    for (int i= 0; i<12; i++) {
        wf_col[i] = glm::vec4(0,0,0.8,1);
    }
    for (int i= 12; i<24; i++) {
        wf_col[i] = glm::vec4(0.8,0,0,1);
    }
    for (int i= 24; i<WF_COL_COUNT; i++) {
        wf_col[i] = glm::vec4(0,0.8,0,1);
    }
    wf_col[36] = glm::vec4(0.75f,0.65f,0,1);
    wf_col[37] = glm::vec4(1,1,1,1);
}

void createWireframeColors(glm::vec4 (&wf_col)[WF_COL_COUNT], bool selected) { 
    if (!selected) {
        for (int i= 0; i<12; i++) {
            wf_col[i] = glm::vec4(0,0,0.8,1);
        }
        for (int i= 12; i<24; i++) {
            wf_col[i] = glm::vec4(0.8,0,0,1);
        }
        for (int i= 24; i<WF_COL_COUNT; i++) {
            wf_col[i] = glm::vec4(0,0.8,0,1);
        }
        wf_col[36] = glm::vec4(0.5f,0.5f,0,1);
        wf_col[37] = glm::vec4(1,1,1,1);

    } else {
        for (int i = 0; i<WF_COL_COUNT;i++) {
            wf_col[i]=glm::vec4(0.25,1,0.5,1);
        }
    }
}

void Joint::create() {
    GLuint wf_idx[WF_IDX_COUNT];
    glm::vec4 wf_vert_pos[WF_VERT_COUNT];
    glm::vec4 wf_col[WF_COL_COUNT];

    if (parent != NULL) {
        createWireframeVertexPositions(wf_vert_pos, parent,this);
    } else {
        createWireframeVertexPositions(wf_vert_pos);
    }

    createWireframeIndices(wf_idx);
    createWireframeColors(wf_col);

    count = WF_IDX_COUNT;

    bufIdx.create();
    bufIdx.bind();
    bufIdx.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufIdx.allocate(wf_idx, WF_IDX_COUNT * sizeof(GLuint));

    bufPos.create();
    bufPos.bind();
    bufPos.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufPos.allocate(wf_vert_pos,WF_VERT_COUNT * sizeof(glm::vec4));

    bufCol.create();
    bufCol.bind();
    bufCol.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufCol.allocate(wf_col, WF_COL_COUNT * sizeof(glm::vec4));
}

void Joint::create(bool selected) {
    GLuint wf_idx[WF_IDX_COUNT];
    glm::vec4 wf_vert_pos[WF_VERT_COUNT];
    glm::vec4 wf_col[WF_COL_COUNT];

    if (parent != NULL) {
        createWireframeVertexPositions(wf_vert_pos, parent,this);
    } else {
        createWireframeVertexPositions(wf_vert_pos);
    }

    createWireframeIndices(wf_idx);
    createWireframeColors(wf_col,selected);

    count = WF_IDX_COUNT;

    bufIdx.create();
    bufIdx.bind();
    bufIdx.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufIdx.allocate(wf_idx, WF_IDX_COUNT * sizeof(GLuint));

    bufPos.create();
    bufPos.bind();
    bufPos.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufPos.allocate(wf_vert_pos,WF_VERT_COUNT * sizeof(glm::vec4));

    bufCol.create();
    bufCol.bind();
    bufCol.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufCol.allocate(wf_col, WF_COL_COUNT * sizeof(glm::vec4));
}

GLenum Joint::drawMode() {
    return GL_LINES;
}
