#ifndef VERTICESLISTWIDGET_H
#define VERTICESLISTWIDGET_H

#include <QListWidget>

class VerticesListWidget : public QListWidget
{
     Q_OBJECT

public:
    VerticesListWidget(QWidget* parent);

public slots:
    void slot_receiveVertex(QListWidgetItem*);
};

#endif // VERTICESLISTWIDGET_H


