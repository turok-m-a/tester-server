#ifndef TEACHERWINDOW_H
#define TEACHERWINDOW_H

#include <QDialog>
#include <studcontrolwindow.h>
namespace Ui {
class TeacherWindow;
}

class TeacherWindow : public QDialog
{
    Q_OBJECT

public:
    explicit TeacherWindow(QWidget *parent = 0);
    ~TeacherWindow();

private slots:
    void on_studControl_clicked();

private:
    Ui::TeacherWindow *ui;
};

#endif // TEACHERWINDOW_H
