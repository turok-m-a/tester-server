#ifndef EXAMCONTROL_H
#define EXAMCONTROL_H

#include <QDialog>
#include <network.h>
#include <studcontrolwindow.h>
#include <QTimer>
#include <QDate>
#include <reportview.h>
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


    void on_selectExam_clicked();

    void on_examList_itemSelectionChanged();

    void on_subjList_currentIndexChanged(int index);

    void updateStudList();
    void addStudToExam();
    void on_viewExamHistory_clicked();

    void on_viewReport_clicked();

private:
    QTimer * studentListUpdateTimer;
    Ui::ExamControl *ui;
    QVector<int> subjectId;
    QVector<int> studIds;
    QVector<QString> studNames;
    int selectedExamId;
    int examTime;
    bool subjListIsEmpty;
    QVector<QString> addedStudentsNames;
    QVector<int> addedStudentsIds;
    bool studListView;
};

#endif // EXAMCONTROL_H
