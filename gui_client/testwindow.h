#ifndef TESTWINDOW_H
#define TESTWINDOW_H

#include <QDialog>
#include <question.h>
#include <QCheckBox>
#include <qlineedit.h>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <network.h>
#include <infowindow.h>
#include <sequencequestiondrawer.h>
#include <groupquestiondrawer.h>
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
    Ui::TestWindow * ui;
    QVector<Question> questions;
    int currentQuestion;
    void showQuestion(int number);
    void hideQuestion();
    void showSequenceQuestion(QVector<int> restoreSequence = QVector<int>());
    void showMatchQuestion(QVector<int> restoreSequence = QVector<int>());
    QVector<QObject *> guiObjects;
    QGraphicsScene * scene;
    SequenceQuestionDrawer * drawer;
    GroupQuestionDrawer * gDrawer;
};

#endif // TESTWINDOW_H
