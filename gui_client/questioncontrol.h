#ifndef QUESTIONCONTROL_H
#define QUESTIONCONTROL_H

#include <QDialog>
#include <QDebug>
#include <network.h>
#include <qcheckbox.h>
#include <QTableWidgetItem>
#include <QSignalMapper>
namespace Ui {
class QuestionControl;
}

class QuestionControl : public QDialog
{
    Q_OBJECT

public:
    explicit QuestionControl(QWidget *parent = 0);
    ~QuestionControl();

private slots:
    void on_subjectList_currentIndexChanged(int index);

    void on_delQuestion_clicked();

    void on_pushButton_2_clicked();

    void on_addToSubject_clicked();

    void on_addQuestion_clicked();

    void on_finishQustionAdd_clicked();

    void on_questionType_currentIndexChanged(int index);

    void on_answersNumber_editingFinished();

    void on_answersNumber2_editingFinished();

    void on_groups_itemChanged(QTableWidgetItem *item);

    void on_questionEdit_itemChanged(QTableWidgetItem *item);

    void on_pushButton_3_clicked();

    void on_addExam_clicked();
    void questionCheckStateChanged(QString id);
    void on_findQuestion_textChanged(const QString &arg1);

private:
    QVector<QVector <QString>> questions;
    QSignalMapper * signalMapper;
    Ui::QuestionControl *ui;
    QVector<QString> subjectName;
     QVector<int> subjectId;
     bool subjListIsEmpty;
     QVector<int> selectedQuestionIds;
     void formatQuestionText(QString & text);
};

#endif // QUESTIONCONTROL_H
