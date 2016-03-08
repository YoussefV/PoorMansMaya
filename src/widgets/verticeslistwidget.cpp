#include "verticeslistwidget.h"

VerticesListWidget::VerticesListWidget(QWidget* parent)  : QListWidget(parent)
{}

void VerticesListWidget::slot_receiveVertex(QListWidgetItem* item) {
     this->addItem(item);
}
