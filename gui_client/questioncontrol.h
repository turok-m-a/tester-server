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

private:
    Ui::QuestionControl *ui;
    QVector<QString> subjectName;
     QVector<int> subjectId;
     bool subjListIsEmpty;
};

#endif // QUESTIONCONTROL_H
