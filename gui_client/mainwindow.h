#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "testwindow.h"
#include <QPushButton>
#include <network.h>
#include <connectionerror.h>
class TestWindow;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow * ui;
    TestWindow * testWindow;

private slots:
    void handleButton();
    void on_loginButton_clicked();
};

#endif // MAINWINDOW_H
