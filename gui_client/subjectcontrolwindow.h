#ifndef SUBJECTCONTROLWINDOW_H
#define SUBJECTCONTROLWINDOW_H

#include <QDialog>
#include <constants.h>
#include <network.h>
namespace Ui {
class SubjectControlWindow;
}

class SubjectControlWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SubjectControlWindow(QWidget *parent = 0);
    ~SubjectControlWindow();

private slots:
    void on_findButton_clicked();

    void on_addButton_clicked();

    void on_deleteButton_clicked();

private:
    Ui::SubjectControlWindow *ui;
};

#endif // SUBJECTCONTROLWINDOW_H
