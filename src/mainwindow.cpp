#include "mainwindow.h"
#include <ui_mainwindow.h>
#include <nodetree.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

void MainWindow::sendItems() {
    for (uint i = 0; i < ui->mygl->items.size(); i++) {
        emit sig_sendSceneItem(ui->mygl->items[i]->root);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionQuit_triggered()
{
    QApplication::exit();
}
