#ifndef STUDCONTROLWINDOW_H
#define STUDCONTROLWINDOW_H

#include <QDialog>
#include <constants.h>
#include <network.h>
#include <infowindow.h>
#include <QCheckBox>
namespace Ui {
class StudControlWindow;
}

class StudControlWindow : public QDialog
{
    Q_OBJECT

public:
    explicit StudControlWindow(QWidget *parent = 0, bool studentSelectMode_ = false);
    ~StudControlWindow();

private slots:
    void on_findButton_clicked();

    void on_addButton_clicked();

    void on_delete_2_clicked();

    void on_addStudentToExam_clicked();
    QVector<int> * selectedStudents;
private:
    Ui::StudControlWindow *ui;
    bool studentSelectMode;
};

#endif // STUDCONTROLWINDOW_H
