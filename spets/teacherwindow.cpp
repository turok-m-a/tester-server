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

void TeacherWindow::on_subjectControl_clicked()
{
    SubjectControlWindow * subjectControlWindow = new SubjectControlWindow();
    subjectControlWindow->setAttribute(Qt::WA_DeleteOnClose);
    subjectControlWindow->show();
}

void TeacherWindow::on_pushButton_clicked()
{
    QuestionControl * questionControl = new QuestionControl();
    questionControl->setAttribute(Qt::WA_DeleteOnClose);
    questionControl->show();
}

void TeacherWindow::on_examControl_clicked()
{
    ExamControl * examControl = new ExamControl();
    examControl->setAttribute(Qt::WA_DeleteOnClose);
    examControl->show();
}
