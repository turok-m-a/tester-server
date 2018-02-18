#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "testwindow.h"
#include <infowindow.h>
#include <QPushButton>
#include <network.h>
#include <constants.h>
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
    infowindow * infoWindow;
    TestWindow * testWindow;

private slots:
    void handleButton();
    void on_loginButton_clicked();
    void on_studentSwitch_toggled(bool checked);
};

#endif // MAINWINDOW_H
