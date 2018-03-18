#include "questioncontrol.h"
#include "ui_questioncontrol.h"

QuestionControl::QuestionControl(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QuestionControl)
{
    subjListIsEmpty = true;
    ui->setupUi(this);

     QByteArray byteArray;
     QDataStream stream(&byteArray, QIODevice::WriteOnly);

     stream << QString("");

     Network & network = Network::getInstance();

     QByteArray reply;
     QDataStream stream2(&reply, QIODevice::ReadOnly);
     reply = network.sendQuery(GET_SUBJECT_LIST,byteArray);
     int subjNumber;
     stream2 >> subjNumber;

     for (int i=0;i<subjNumber;i++){
         QString subjName,subjId;
         stream2 >> subjName;
         ui->subjectList->addItem(subjName);
         stream2 >> subjId;
         subjectId.push_back(subjId.toInt());
     }
     ui->tableWidget->setColumnWidth(0,60);
     ui->tableWidget->setColumnWidth(1,600);
     ui->tableWidget->setColumnWidth(1,140);
     ui->tableWidget->setColumnWidth(3,0);
     ui->tableWidget->setColumnWidth(4,40);
     ui->tableWidget->setColumnWidth(5,15);
     subjListIsEmpty = false;
}

QuestionControl::~QuestionControl()
{
    delete ui;
}

void QuestionControl::on_subjectList_currentIndexChanged(int index)
{
    if (subjListIsEmpty) return;
    QByteArray byteArray;
    QDataStream stream(&byteArray, QIODevice::WriteOnly);
    stream << subjectId[index];
     Network & network = Network::getInstance();
     QByteArray reply;
     QDataStream stream2(&reply, QIODevice::ReadOnly);
     reply = network.sendQuery(GET_QUESTION_LIST,byteArray);
     int qNumber;
     stream2 >> qNumber;
   QVector<QVector <QString>> questions;
   stream2 >> questions;
    QString columnText;
     for (int i=0;i<qNumber;i++){
         ui->tableWidget->insertRow(0);
         for (int j=0;j<5;j++){//тип,вопрос-ответ,предмет,ID(скрыт),сложность
          columnText = questions[i][j];
          ui->tableWidget->setItem(0,j,new QTableWidgetItem(columnText));
         }
         ui->tableWidget->setCellWidget(0,5,new QCheckBox());
     }
     ui->tableWidget->resizeRowsToContents();
}
