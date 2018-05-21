#include "adminwindow.h"
#include "ui_adminwindow.h"

AdminWindow::AdminWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AdminWindow)
{
    ui->setupUi(this);
}

AdminWindow::~AdminWindow()
{
    delete ui;
}

void AdminWindow::on_addUser_clicked()
{
    QVector<QString> values;
    int paramNumber = 0;
    if (!ui->firstName->text().isEmpty()){
        values.push_back(ui->firstName->text());
        paramNumber++;
    }
    if (!ui->middleName->text().isEmpty()){
        values.push_back(ui->middleName->text());
        paramNumber++;
    }
    if (!ui->lastName->text().isEmpty()){
        values.push_back(ui->lastName->text());
        paramNumber++;
    }
    if (!ui->login->text().isEmpty()){
        values.push_back(ui->login->text());
        paramNumber++;
    }
    if (!ui->password->text().isEmpty()){
        values.push_back(ui->password->text());
        paramNumber++;
    }
    values.push_back(QString::number(ui->userType->currentIndex()+1));
    if (ui->password->text() != ui->passwordConfirm->text()){
        infowindow * infoWindow = new infowindow(0,QString("Подтверждение пароля не совпадает!"));
        infoWindow->setAttribute(Qt::WA_DeleteOnClose);
        infoWindow->show();
        return;
    }
    if (paramNumber < 5){
        infowindow * infoWindow = new infowindow(0,QString("Необходимо заполнить все поля!"));
        infoWindow->setAttribute(Qt::WA_DeleteOnClose);
        infoWindow->show();
        return;
    }
    QByteArray byteArray;
    QDataStream stream(&byteArray, QIODevice::WriteOnly);
        stream << values;
    Network & network = Network::getInstance();
    QByteArray reply;
    QDataStream stream2(&reply, QIODevice::ReadOnly);
    reply = network.sendQuery(ADD_USER,byteArray);
}

void AdminWindow::on_delUser_clicked()
{
    if (ui->tableWidget->selectedItems().isEmpty()) return;
    int selectedRow = ui->tableWidget->selectedItems().first()->row();
    QString id = ui->tableWidget->item(selectedRow,5)->text();
    Network & network = Network::getInstance();
    QByteArray request,reply;
    QDataStream stream(&request, QIODevice::WriteOnly);
    stream << id.toInt();
    reply = network.sendQuery(DEL_USER,request);
    ui->findUser->click();
}

void AdminWindow::on_findUser_clicked()
{
    ui->tableWidget->setRowCount(0);
    int paramNumber = 0;
    QVector<int> params;
    QVector<QString> values;
    if (!ui->firstName->text().isEmpty()){
        params.push_back(FILTER_FIRST_NAME);
        values.push_back(ui->firstName->text());
        paramNumber++;
    }
    if (!ui->middleName->text().isEmpty()){
        params.push_back(FILTER_MIDDLE_NAME);
        values.push_back(ui->middleName->text());
        paramNumber++;
    }
    if (!ui->lastName->text().isEmpty()){
        params.push_back(FILTER_LAST_NAME);
        values.push_back(ui->lastName->text());
        paramNumber++;
    }
    if (!ui->login->text().isEmpty()){
        params.push_back(FILTER_LOGIN);
        values.push_back(ui->login->text());
        paramNumber++;
    }
     QByteArray byteArray;
     QDataStream stream(&byteArray, QIODevice::WriteOnly);
     stream << params;
     stream << values;
     Network & network = Network::getInstance();

     QByteArray reply;
     QDataStream stream2(&reply, QIODevice::ReadOnly);
     reply = network.sendQuery(FIND_USER,byteArray);
     //byteArray.clear();
     int userNumber;
     stream2 >> userNumber;
     QString columnText;

     for (int i=0;i<userNumber;i++){
         ui->tableWidget->insertRow(0);
         for (int j=0;j<6;j++){
         stream2 >> columnText;
         if(j==4){
             if (columnText.toInt() == 1){
                 ui->tableWidget->setItem(0,j,new QTableWidgetItem("Преподаватель"));
             } else {
                 ui->tableWidget->setItem(0,j,new QTableWidgetItem("Администратор"));
             }
         } else
         ui->tableWidget->setItem(0,j,new QTableWidgetItem(columnText));
         }
     }
}

void AdminWindow::on_resetPassword_clicked()
{
    if (ui->tableWidget->selectedItems().isEmpty()) return;
    int selectedRow = ui->tableWidget->selectedItems().first()->row();
    int id = ui->tableWidget->item(selectedRow,5)->text().toInt();
    if (ui->password->text() != ui->passwordConfirm->text()){
        infowindow * infoWindow = new infowindow(0,QString("Подтверждение пароля не совпадает!"));
        infoWindow->setAttribute(Qt::WA_DeleteOnClose);
        infoWindow->show();
        return;
    }
    QByteArray byteArray;
    QDataStream stream(&byteArray, QIODevice::WriteOnly);
    stream << id;
    stream << ui->password->text();
    Network & network = Network::getInstance();
    network.sendQuery(RESET_PASSWORD,byteArray);
}
