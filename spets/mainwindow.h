#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "testwindow.h"
#include <infowindow.h>
#include <QPushButton>
#include <network.h>
#include <constants.h>
#include <fstream>
#include <QString>
#include <teacherwindow.h>
#include <adminwindow.h>
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
    //TestWindow * testWindow;
    void studLogin();
    void teacherLogin();
private slots:
    void handleButton();
    void on_loginButton_clicked();
    void on_studentSwitch_toggled(bool checked);
    void on_teacherSwitch_toggled(bool checked);
};

#endif // MAINWINDOW_H
