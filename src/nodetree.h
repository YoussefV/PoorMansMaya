#ifndef NODETREE_H
#define NODETREE_H

#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <scene/node.h>
#include <scene/joint.h>



class NodeTree : public QTreeWidget
{
    Q_OBJECT
public:
    NodeTree(QWidget* parent = NULL);
    Node* newChild = new Node(0,0,0,0,0,0,1,1,1,QString("Default"));
    ~NodeTree();

protected:
    Joint* selection = NULL;
    int getSelectionIndex();
    glm::vec4 *newChildColor = new glm::vec4(0,0,0,1);

public slots:
    void slot_receiveNode(QTreeWidgetItem*);
    void slot_selectionChanged(QTreeWidgetItem*);

    void slot_bindMeshButtonPressed();

    //Receives a spin box's new value
    void slot_translateSelectedX(double);
    void slot_translateSelectedY(double);
    void slot_translateSelectedZ(double);
    //Receives a spin box's new value
    void slot_rotateXPlus();
    void slot_rotateXMinus();
    void slot_rotateYPlus();
    void slot_rotateYMinus();
    void slot_rotateZPlus();
    void slot_rotateZMinus();


signals:

    //Sends initial Rotate Values once Node is selected
    void sig_rotateX(double);
    void sig_rotateY(double);
    void sig_rotateZ(double);

    //Sends initial Translate Values once Node is selected
    void sig_translateX(double);
    void sig_translateY(double);
    void sig_translateZ(double);

    void sig_bindAtIndex(int);
    void sig_updateVertices();

    void sig_selectionID(int);

};

#endif // NODETREE_H
