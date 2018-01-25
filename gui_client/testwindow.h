#ifndef TESTWINDOW_H
#define TESTWINDOW_H

#include <QDialog>
#include <question.h>
#include <QCheckBox>
namespace  Ui {
    class TestWindow;
}
class TestWindow : public QDialog

{
    Q_OBJECT
public:
    explicit TestWindow(QWidget *parent = 0);
    void showWindow();
    void setQuestions(QVector<Question> _questions);
private slots:
    void on_nextButton_clicked();

    void on_prevButton_clicked();

    void on_submitButton_clicked();

private:
    Ui::TestWindow * tui;
    QVector<Question> questions;
    int currentQuestion;
    void showQuestion(int number);
    QVector<QObject *> guiObjects;
};

#endif // TESTWINDOW_H
