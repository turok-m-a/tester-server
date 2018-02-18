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
}

void MainWindow::on_loginButton_clicked()
{
    Network & network = Network::getInstance();
    QVector<Question> questions;
    int loginStatus;
    questions = network.getQuestionsForStudent(ui->login->text(),loginStatus);
    if (questions.isEmpty()){
        infoWindow = new infowindow();
        infoWindow->setAttribute(Qt::WA_DeleteOnClose);//удаление объекта по закрытию окна
        switch (loginStatus) {
        case CONN_NOT_FOUND:
            infoWindow->setMessage("Неправильный номер ст. билета либо нет допуска к экзамену");
            break;
        case CONN_ERROR:
            infoWindow->setMessage("Нет соединеия с сервером");
            break;
        default:
            infoWindow->setMessage("Неизвестная ошибка");
            break;
        }
        infoWindow->show();
        return;
    }
    if (loginStatus == CONN_OK){
    network.studLogin = ui->login->text();
    network.currentUserType = 1;
    testWindow = new TestWindow();
    testWindow->setAttribute(Qt::WA_DeleteOnClose);//удаление объекта по закрытию окна
    testWindow->setQuestions(questions);
    testWindow->showWindow();
    }
}

void MainWindow::on_studentSwitch_toggled(bool checked)
{
    if(checked){
        ui->password->setEnabled(false);
    }
}
