#include "testwindow.h"
#include "ui_testwindow.h"

TestWindow::TestWindow(QWidget *parent): QDialog(parent),  tui(new Ui::TestWindow)
{
   tui->setupUi(this);
}

void TestWindow::showWindow()
{
    this->show();
}

void TestWindow::setQuestions(QVector<Question> _questions)
{
    questions = _questions;
    currentQuestion = 0;
    showQuestion(0);
}

void TestWindow::on_nextButton_clicked()
{
    if(currentQuestion < questions.size()-1){
    currentQuestion++;
    }

    showQuestion(currentQuestion);
}

void TestWindow::on_prevButton_clicked()
{
    if(currentQuestion > 0){
    currentQuestion--;
    }
    showQuestion(currentQuestion);
}

void TestWindow::on_submitButton_clicked()
{

}

void TestWindow::showQuestion(int number)
{
    int type = questions[number].type;
    tui->label->setText(questions[number].getQuestionText());
    if (type == 1){
        int answerNumber = questions[number].getAnswerNumber();
        for (int i=0;i<answerNumber;i++){
            QCheckBox * checkBox = new QCheckBox(questions[number].getAnswerText(i), this);
            QString a = questions[number].getAnswerText(i);
            checkBox->setGeometry(50,100+i*50,500,15);
            checkBox->setText(a);
            checkBox->show();
        }
    }
}
