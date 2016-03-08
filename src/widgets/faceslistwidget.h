#ifndef FACESLISTWIDGET_H
#define FACESLISTWIDGET_H

#include <QListWidget>


class FacesListWidget : public QListWidget
{
    Q_OBJECT

public:
    FacesListWidget(QWidget* parent);

public slots:
    void slot_receiveFace(QListWidgetItem*);
};

#endif // FACESLISTWIDGET_H

