#include "studcontrolwindow.h"
#include "ui_studcontrolwindow.h"

StudControlWindow::StudControlWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StudControlWindow)
{
    ui->setupUi(this);
}

StudControlWindow::~StudControlWindow()
{
    delete ui;
}

void StudControlWindow::on_findButton_clicked()
{
    ui->tableWidget->setRowCount(0);
    int paramNumber = 0;
    QVector<int> params;
    QVector<QString> values;
    if (!ui->first->text().isEmpty()){
        params.push_back(FILTER_FIRST_NAME);
        values.push_back(ui->first->text());
        paramNumber++;
    }
    if (!ui->middle->text().isEmpty()){
        params.push_back(FILTER_MIDDLE_NAME);
        values.push_back(ui->middle->text());
        paramNumber++;
    }
    if (!ui->last->text().isEmpty()){
        params.push_back(FILTER_LAST_NAME);
        values.push_back(ui->last->text());
        paramNumber++;
    }
    if (!ui->group->text().isEmpty()){
        params.push_back(FILTER_STUD_GROUP);
        values.push_back(ui->group->text());
        paramNumber++;
    }
    if (!ui->ticket->text().isEmpty()){
        params.push_back(FILTER_STUD_TICKET);
        values.push_back(ui->ticket->text());
        paramNumber++;
    }
     QByteArray byteArray;
     QDataStream stream(&byteArray, QIODevice::WriteOnly);
     stream.setByteOrder(QDataStream::LittleEndian);
     stream << paramNumber;
     for (int i=0;i<paramNumber;i++){
         stream << params[i];
         stream << values[i];
     }
     Network & network = Network::getInstance();

     QByteArray reply;
     QDataStream stream2(&reply, QIODevice::ReadOnly);
     reply = network.sendQuery(GET_STUDENT_LIST,byteArray);
     //byteArray.clear();
     int studNumber;
     stream2 >> studNumber;
     QString columnText;

     for (int i=0;i<studNumber;i++){
         ui->tableWidget->insertRow(0);
         for (int j=0;j<6;j++){
         stream2 >> columnText;
         ui->tableWidget->setItem(0,j,new QTableWidgetItem(columnText));
         }
     }

}

void StudControlWindow::on_addButton_clicked()
{
    QVector<int> params;
    QVector<QString> values;
    int paramNumber = 0;
    if (!ui->first->text().isEmpty()){
        params.push_back(FILTER_FIRST_NAME);
        values.push_back(ui->first->text());
        paramNumber++;
    }
    if (!ui->middle->text().isEmpty()){
        params.push_back(FILTER_MIDDLE_NAME);
        values.push_back(ui->middle->text());
        paramNumber++;
    }
    if (!ui->last->text().isEmpty()){
        params.push_back(FILTER_LAST_NAME);
        values.push_back(ui->last->text());
        paramNumber++;
    }
    if (!ui->group->text().isEmpty()){
        params.push_back(FILTER_STUD_GROUP);
        values.push_back(ui->group->text());
        paramNumber++;
    }
    if (!ui->ticket->text().isEmpty()){
        params.push_back(FILTER_STUD_TICKET);
        values.push_back(ui->ticket->text());
        paramNumber++;
    }
    if (paramNumber < 5){
        infowindow * infoWindow = new infowindow(0,QString("Необходимо заполнить все поля!"));
        infoWindow->setAttribute(Qt::WA_DeleteOnClose);
        infoWindow->show();
        return;
    }
    QByteArray byteArray;
    QDataStream stream(&byteArray, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::LittleEndian);
    //stream << paramNumber;
    for (int i=0;i<paramNumber;i++){
        stream << params[i];
        stream << values[i];
    }
    Network & network = Network::getInstance();
    QByteArray reply;
    QDataStream stream2(&reply, QIODevice::ReadOnly);
    reply = network.sendQuery(ADD_STUDENT,byteArray);

}

void StudControlWindow::on_delete_2_clicked()
{
    int selectedRow = ui->tableWidget->selectedItems().first()->row();
    QString id = ui->tableWidget->item(selectedRow,5)->text();
    Network & network = Network::getInstance();
    QByteArray request,reply;
    QDataStream stream(&request, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::LittleEndian);
    stream << id.toInt();
    reply = network.sendQuery(REMOVE_STUDENT,request);
    ui->findButton->click();
}
