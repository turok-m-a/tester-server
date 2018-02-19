#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    ifstream config;
    config.open("config.ini");
    if(!config.is_open()){
        infoWindow = new infowindow(0,"Файл конфигурации не найден!");
        this->close();
    }
    Network & network = Network::getInstance();
    string address;
    config >> address;
    network.setAddress(QString(address.c_str()));
    config.close();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::studLogin()
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

void MainWindow::teacherLogin()
{
    Network & network = Network::getInstance();
    int loginStatus;
    network.setUser(ui->login->text(),ui->password->text());
    loginStatus = network.checkUser();
    if (loginStatus != 1){
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
    TeacherWindow * teacherWindow = new TeacherWindow();
    teacherWindow->setAttribute(Qt::WA_DeleteOnClose);//удаление объекта по закрытию окна
    teacherWindow->show();
    }
}
void MainWindow::handleButton()
{
}

void MainWindow::on_loginButton_clicked()
{
    if (ui->studentSwitch->isChecked()){
    studLogin();
    } else {
        teacherLogin();
    }
}

void MainWindow::on_studentSwitch_toggled(bool checked)
{
    if(checked){
        ui->password->setEnabled(false);
    }
}

void MainWindow::on_teacherSwitch_toggled(bool checked)
{
    if(checked){
        ui->password->setEnabled(true);
    }
}
