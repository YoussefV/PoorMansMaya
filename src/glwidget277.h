#ifndef GLWIDGET277_H
#define GLWIDGET277_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_2_Core>
#include <QTimer>
#include <QTreeWidgetItem>
#include <QListWidgetItem>
#include <la.h>


class GLWidget277
    : public QOpenGLWidget,
      public QOpenGLFunctions_3_2_Core
{
    Q_OBJECT

private:
    /// Timer for drawing new frames
    QTimer timer;

protected:
    /*** AUTOMATIC TESTING: DO NOT MODIFY ***/
    /*** If true, save a test image and exit */
    /***/ bool autotesting;

public:
    GLWidget277(QWidget *parent);
    ~GLWidget277();

    void debugContextVersion();
    void printGLErrorLog();
    void printLinkInfoLog(int prog);
    void printShaderInfoLog(int shader);

signals:
    void sig_sendSceneItem(QTreeWidgetItem*);
    void sig_sendVertex(QListWidgetItem*);
    void sig_sendFace(QListWidgetItem*);
    void sig_sendHalfEdge(QListWidgetItem*);

    void sig_sendVertexX(double);
    void sig_sendVertexY(double);
    void sig_sendVertexZ(double);

public slots:
    virtual void slot_skeletonButtonPressed() = 0;
    virtual void slot_meshButtonPressed() = 0;
    virtual void slot_bindAtIndex(int) = 0;
    virtual void slot_updateVertices() = 0;

    virtual void slot_halfEdgeSelectionChanged(QListWidgetItem*) = 0;
    virtual void slot_nextEdgeButtonPressed() = 0;
    virtual void slot_symEdgeButtonPressed() = 0;

    virtual void slot_faceSelectionChanged(QListWidgetItem *) = 0;

    virtual void slot_vertexSelectionChanged(QListWidgetItem *) = 0;
    virtual void slot_receiveVertexTranslateX(double) = 0;
    virtual void slot_receiveVertexTranslateY(double) = 0;
    virtual void slot_receiveVertexTranslateZ(double) = 0;

    virtual void slot_selectionID(int) = 0;

private slots:
    /*** AUTOMATIC TESTING: DO NOT MODIFY ***/
    /***/ void saveImageAndQuit();

    /// Slot that gets called ~60 times per second
    void timerUpdate();
};


#endif // GLWIDGET277_H
