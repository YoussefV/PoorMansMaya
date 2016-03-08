#include "character.h"
#include "scene/node.h"
#include "scene/cube.h"
#include "scene/sphere.h"

#include "shaderprogram.h"
#include <la.h>
#include <QApplication>
#include <QKeyEvent>
#include <la.h>

Character::Character(Cube *cube, Sphere *sphere)
{
    float bodyDepth = 1;

    //Creating Body
    root = new Node(0,0,0,0,0,0,2,3,bodyDepth, QString("Root - Torso"));
    root->setGeometry(cube);
    root->setGeomColor(glm::vec4(1,0.1,0.1,1));

    //Creating headJoint (neck)
    Node *headJoint =  new Node(0,0.5,0,0,0,0,(1.0/2.0),(1.0/3.0),(1.0/bodyDepth), QString("Joint - Head"));

    //Adding headJoint as Child to Body
    root->addChild(headJoint);

    //Create head
//    Node *head = new Node(0,2.5,0,0,0,0,1.75,2,1.75, QString("head"));
    Node *head = new Node(0,1,0,0,0,0,1.75,2,1.75, QString("Head"));
    head->setGeometry(sphere);
    head->setGeomColor(glm::vec4(1,0.9,0.8,1));

    //Add head as Child to the headJoint
    headJoint->addChild(head);

    /*
     *Starting to create the left arm
     */
    //Creating joint for left arm
    Node *lArmJoint =  new Node(.4,0.33,0,0,0,0,(1.0/2.0),(1.0/3.0),(1.0/bodyDepth), QString("Joint - Left Arm"));
    root->addChild(lArmJoint);

    //Creating lArm itself
    Node *lArm = new Node(.75,0,0,0,0,0,1.5,bodyDepth,bodyDepth, QString("Left Arm"));
    lArm->setGeometry(cube);
    lArm->setGeomColor(glm::vec4(1,0.1,0.1,1));
    lArmJoint->addChild(lArm);

    //Creating lForearmJoint
    Node *lForearmJoint = new Node(.6,0,0,0,0,0,1,1,1,QString("Joint - Left Forearm"));
    lArm->addChild(lForearmJoint);

    //Creating and adding lForearm
    Node *lForearm = new Node (.4,0,0,0,0,0,1,bodyDepth,bodyDepth,QString("Left Forearm"));
    lForearm-> setGeometry(cube);
    lForearm-> setGeomColor(glm::vec4(1,0.9,0.8,1));
    lForearmJoint->addChild(lForearm);

    /*
     *Starting to create the right arm
     */
    //Creating joint for right arm
    Node *rArmJoint =  new Node(-0.4,0.33,0,0,0,0,(1.0/2.0),(1.0/3.0),(1.0/bodyDepth),QString("Joint - Right Arm"));
    root->addChild(rArmJoint);

    //Creating and adding rArm
    Node *rArm = new Node(-0.75,0,0,0,0,0,1.5,bodyDepth,bodyDepth,QString("Right Arm"));
    rArm->setGeometry(cube);
    rArm->setGeomColor(glm::vec4(1,0.1,0.1,1));
    rArmJoint->addChild(rArm);

    //Creating rForearmJoint
    Node *rForearmJoint = new Node(-0.6,0,0,0,0,0,1,1,1,QString("Joint - Forearm Joint"));
    rArm->addChild(rForearmJoint);

    //Creating and adding lForearm
    Node *rForearm = new Node (-0.4,0,0,0,0,0,1,1,1,QString("Right Forearm"));
    rForearm-> setGeometry(cube);
    rForearm-> setGeomColor(glm::vec4(1,0.9,0.8,1));
    rForearmJoint->addChild(rForearm);

    /********MAKING THE LEGS*********/

    /*
     *Starting to create the left leg
     */
    //Creating lLegJoint
    Node *lLegJoint = new Node(0.275,-0.5,0,0,0,0,(1.0/2.0),(1.0/3.0),(1.0/bodyDepth),QString("Joint - Left Leg"));
    root->addChild(lLegJoint);

    //Creating left leg
    Node *lLeg = new Node (0,-1,0,0,0,0,0.9,2,bodyDepth, QString("Left Leg"));
    lLeg-> setGeometry(cube);
    lLeg-> setGeomColor(glm::vec4(0.3,0.3,1,1));
    lLegJoint->addChild(lLeg);

    //Creating left foreleg joint
    Node *lForelegJoint = new Node(0,-0.5,0,0,0,0,1,1,1,QString("Joint - Left Foreleg"));
    lLeg->addChild(lForelegJoint);

    //Creating left foreleg
    Node *lForeleg = new Node (0,-0.5,0,0,0,0,1,1,1,QString("Left Foreleg"));
    lForeleg-> setGeometry(cube);
    lForeleg-> setGeomColor(glm::vec4(0.3,0.3,1,1));
    lForelegJoint->addChild(lForeleg);

    /*
     *Starting to create the right leg
     */
    //Creating rLegJoint
    Node *rLegJoint = new Node(-0.275,-0.5,0,0,0,0,(1.0/2.0),(1.0/3.0),(1.0/bodyDepth),QString("Joint - Right Leg"));
    root->addChild(rLegJoint);

    //Creating right leg
    Node *rLeg = new Node (0,-1,0,0,0,0,0.9,2,bodyDepth,QString("Right Leg"));
    rLeg-> setGeometry(cube);
    rLeg-> setGeomColor(glm::vec4(0.3,0.3,1,1));
    rLegJoint->addChild(rLeg);

    //Creating right foreleg joint
    Node *rForelegJoint = new Node(0,-0.5,0,0,0,0,1,1,1,QString("Joint - Right Foreleg"));
    rLeg->addChild(rForelegJoint);

    //Creating left foreleg
    Node *rForeleg = new Node (0,-0.5,0,0,0,0,1,1,1,QString("Right Foreleg"));
    rForeleg-> setGeometry(cube);
    rForeleg-> setGeomColor(glm::vec4(0.3,0.3,1,1));
    rForelegJoint->addChild(rForeleg);
}

void Character::draw(GLWidget277 &f , ShaderProgram &prog_lambert) {
    drawNode2(root,f,prog_lambert);
}

void Character::drawNode2(Node* n, GLWidget277 &f , ShaderProgram &prog_lambert) {
  if (n != nullptr || (n->getGeometry() != nullptr && n->childCount() != 0)) {
    for (int i = 0; i<n->childCount();i++) {
        if (n->childAt(i)->childCount() > 0) {
              drawNode2(n->childAt(i),f,prog_lambert);
        }
        else if (n->childAt(i)->getGeometry() != nullptr) {
            prog_lambert.setModelMatrix(n->childAt(i)->getChildTransMat(n));
            n->childAt(i)->getGeometry()->setColor(n->childAt(i)->getGeometryColor());
            prog_lambert.draw(f, *(n->childAt(i)->getGeometry()));
        }
    }

    if (n->getGeometry() != nullptr) {
        if (n->getParent() != nullptr) {
            prog_lambert.setModelMatrix(n->getChildTransMat(n->getParent()));
        } else prog_lambert.setModelMatrix(n->getTransMat());
        n->getGeometry()->setColor(n->getGeometryColor());
        prog_lambert.draw(f, *(n->getGeometry()));
    }
   }
}




//void Character::drawNode(Node* n, GLWidget277 &f , ShaderProgram &prog_lambert) {
//    for (int i = 0; i<n->childCount();i++) {
//        if (n->childAt(i)->childCount() > 0) {
//              drawNode(n->childAt(i),f,prog_lambert);
//        }
//        else if (n->childAt(i)->getGeometry() != nullptr) {
//            prog_lambert.setModelMatrix(n->childAt(i)->getTransMat());
//            n->childAt(i)->getGeometry()->setColor(n->childAt(i)->getGeometryColor());
//            prog_lambert.draw(f, *(n->childAt(i)->getGeometry()));
//        }
//    }

//    if (n->getGeometry() != nullptr) {
//        prog_lambert.setModelMatrix(n->getTransMat());
//        n->getGeometry()->setColor(n->getGeometryColor());
//        prog_lambert.draw(f, *(n->getGeometry()));
//    }
//}
