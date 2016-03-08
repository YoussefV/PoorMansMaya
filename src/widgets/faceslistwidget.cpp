#include "faceslistwidget.h"

FacesListWidget::FacesListWidget(QWidget* parent)  : QListWidget(parent)
{}

void FacesListWidget::slot_receiveFace(QListWidgetItem* item) {
    this->addItem(item);
}
