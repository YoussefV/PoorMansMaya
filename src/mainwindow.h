#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidgetItem>
#include <QListWidgetItem>

namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void sendItems();

private slots:
    void on_actionQuit_triggered();

signals:
    void sig_sendSceneItem(QTreeWidgetItem*);
    void sig_sendSceneVertex(QListWidgetItem*);

private:
    Ui::MainWindow *ui;
};


#endif // MAINWINDOW_H
