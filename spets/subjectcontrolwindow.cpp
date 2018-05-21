#include "subjectcontrolwindow.h"
#include "ui_subjectcontrolwindow.h"

SubjectControlWindow::SubjectControlWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SubjectControlWindow)
{
    ui->setupUi(this);
}

SubjectControlWindow::~SubjectControlWindow()
{
    delete ui;
}

void SubjectControlWindow::on_findButton_clicked()
{
    ui->tableWidget->setRowCount(0);

    QString subjName;
    subjName = ui->lineEdit->text();

     QByteArray byteArray;
     QDataStream stream(&byteArray, QIODevice::WriteOnly);

         stream << subjName;

     Network & network = Network::getInstance();

     QByteArray reply;
     QDataStream stream2(&reply, QIODevice::ReadOnly);
     reply = network.sendQuery(GET_SUBJECT_LIST,byteArray);
     int subjNumber;
     stream2 >> subjNumber;
     QString columnText;

     for (int i=0;i<subjNumber;i++){
         ui->tableWidget->insertRow(0);
         for (int j=0;j<2;j++){
         stream2 >> columnText;
         ui->tableWidget->setItem(0,j,new QTableWidgetItem(columnText));
         }
     }

}

void SubjectControlWindow::on_addButton_clicked()
{
    QByteArray byteArray;
    QDataStream stream(&byteArray, QIODevice::WriteOnly);
    Network & network = Network::getInstance();
    QString subjName = ui->lineEdit->text();
    stream << subjName;
    network.sendQuery(ADD_SUBJECT,byteArray);

}

void SubjectControlWindow::on_deleteButton_clicked()
{
    if (ui->tableWidget->selectedItems().isEmpty()) return;
    int selectedRow = ui->tableWidget->selectedItems().first()->row();
    QString id = ui->tableWidget->item(selectedRow,1)->text();
    Network & network = Network::getInstance();
    QByteArray request,reply;
    QDataStream stream(&request, QIODevice::WriteOnly);

    stream << id.toInt();
    reply = network.sendQuery(REMOVE_SUBJECT,request);
    ui->findButton->click();
    for (int i=0;i<ui->tableWidget->rowCount();i++){
        if (ui->tableWidget->item(i,1)->text() == id){
            infowindow * iw = new infowindow(0,"Невозможно удалить: существуют вопросы, связанные с данным предметом.");
            iw->setAttribute(Qt::WA_DeleteOnClose);
            iw->show();
            break;
        }
    }
}
