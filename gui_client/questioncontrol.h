#ifndef QUESTIONCONTROL_H
#define QUESTIONCONTROL_H

#include <QDialog>
#include <network.h>
#include <qcheckbox.h>
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

    void on_lineEdit_editingFinished();

    void on_answersNumber_editingFinished();

private:
    Ui::QuestionControl *ui;
    QVector<QString> subjectName;
     QVector<int> subjectId;
     bool subjListIsEmpty;
     void formatQuestionText(QString & text);
};

#endif // QUESTIONCONTROL_H
