#include "studcontrolwindow.h"
#include "ui_studcontrolwindow.h"

StudControlWindow::StudControlWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StudControlWindow)
{
    ui->setupUi(this);
}

StudControlWindow::~StudControlWindow()
{
    delete ui;
}
