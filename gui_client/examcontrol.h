#ifndef EXAMCONTROL_H
#define EXAMCONTROL_H

#include <QDialog>
#include <network.h>
#include <studcontrolwindow.h>
namespace Ui {
class ExamControl;
}

class ExamControl : public QDialog
{
    Q_OBJECT

public:
    explicit ExamControl(QWidget *parent = 0);
    ~ExamControl();

private slots:
    void on_addStudToExam_clicked();

    void on_pushButton_clicked();

    void on_selectExam_clicked();

    void on_examList_itemSelectionChanged();

    void on_subjList_currentIndexChanged(int index);

private:
    Ui::ExamControl *ui;
    QVector<int> subjectId;
    QVector<int> studIds;
    int selectedExamId;
    int examTime;
    bool subjListIsEmpty;
    void addStudToExam();
};

#endif // EXAMCONTROL_H
