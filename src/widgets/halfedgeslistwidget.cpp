#include "halfedgeslistwidget.h"

HalfEdgesListWidget::HalfEdgesListWidget(QWidget* parent)  : QListWidget(parent)
{}

void HalfEdgesListWidget::slot_receiveHalfEdge(QListWidgetItem* item) {
    this->addItem(item);
}
