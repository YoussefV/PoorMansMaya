#ifndef HALFEDGESLISTWIDGET_H
#define HALFEDGESLISTWIDGET_H

#include <QListWidget>

class HalfEdgesListWidget : public QListWidget
{
     Q_OBJECT

public:
    HalfEdgesListWidget(QWidget* parent);

public slots:
    void slot_receiveHalfEdge(QListWidgetItem*);
};

#endif // HALFEDGESLISTWIDGET_H
