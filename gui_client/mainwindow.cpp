#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);

//    // создаем кнопку
//   QPushButton * m_button = new QPushButton("My Button", this);
//    // устанавливаем размер и положение кнопки
//    m_button->setGeometry(QRect(QPoint(100, 100),
//    QSize(200, 50)));

//   // подключаем сигнал к соответствующему слоту
//    connect(m_button, SIGNAL (released()), testWindow, SLOT(showWindow()));
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::handleButton()
{
// // меняем текст
// m_button->setText("Example");
// // изменяем размер кнопки
// m_button->resize(100,100);
}


void MainWindow::on_loginButton_clicked()
{
    Network & network = Network::getInstance();
    QVector<Question> questions;
    int loginStatus;
    questions = network.getQuestionsForStudent(ui->login->text(),loginStatus);
    if (loginStatus == CONN_OK){
    network.studLogin = ui->login->text();
    network.currentUserType = 1;
    testWindow = new TestWindow();
    testWindow->setQuestions(questions);
    testWindow->showWindow();
    }
}
