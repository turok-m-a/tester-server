#include "teacherwindow.h"
#include "ui_teacherwindow.h"

TeacherWindow::TeacherWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TeacherWindow)
{
    ui->setupUi(this);
}

TeacherWindow::~TeacherWindow()
{
    delete ui;
}

void TeacherWindow::on_studControl_clicked()
{
    StudControlWindow * studControlWindow = new StudControlWindow();
    studControlWindow->setAttribute(Qt::WA_DeleteOnClose);//удаление объекта по закрытию окна
    studControlWindow->show();
}
