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
         ui->subjectList2->addItem(subjName);
         stream2 >> subjId;
         subjectId.push_back(subjId.toInt());
     }
     ui->tableWidget->setColumnWidth(0,40);
     ui->tableWidget->setColumnWidth(1,500);
     ui->tableWidget->setColumnWidth(2,140);
     ui->tableWidget->setColumnWidth(3,0);
     ui->tableWidget->setColumnWidth(4,40);
     ui->tableWidget->setColumnWidth(5,15);
     subjListIsEmpty = false;
     ui->subjectList->currentIndexChanged(ui->subjectList->currentIndex());
}

QuestionControl::~QuestionControl()
{
    delete ui;
}

void QuestionControl::on_subjectList_currentIndexChanged(int index)
{
    if (subjListIsEmpty) return;
    ui->tableWidget->setRowCount(0);
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

void QuestionControl::on_delQuestion_clicked()
{
    if (ui->tableWidget->selectedItems().isEmpty()) return;
    const int selectedRow = ui->tableWidget->selectedItems().first()->row();
    QString questionId = ui->tableWidget->item(selectedRow,3)->text();

    Network & network = Network::getInstance();
    QByteArray request,reply;
    QDataStream stream(&request, QIODevice::WriteOnly);
    stream << questionId.toInt();
    reply = network.sendQuery(REMOVE_QUESTION,request);
    ui->subjectList->currentIndexChanged(ui->subjectList->currentIndex());
}

void QuestionControl::on_pushButton_2_clicked()
{
    if (ui->tableWidget->selectedItems().isEmpty()) return;
    const int selectedRow = ui->tableWidget->selectedItems().first()->row();
    QString questionId = ui->tableWidget->item(selectedRow,3)->text();
    const int selectedSubjId = subjectId[ui->subjectList->currentIndex()];
    Network & network = Network::getInstance();
    QByteArray request,reply;
    QDataStream stream(&request, QIODevice::WriteOnly);
    stream << questionId.toInt();
    const int editOperationType = SUBJECT_LIST_FOR_QUESTION_REMOVE;
    stream << editOperationType;
    stream << selectedSubjId;
    reply = network.sendQuery(EDIT_SUBJECT_LIST_FOR_QUESTION,request);
    ui->subjectList->currentIndexChanged(ui->subjectList->currentIndex());
}

void QuestionControl::on_addToSubject_clicked()
{
    if (ui->tableWidget->selectedItems().isEmpty()) return;
    const int selectedRow = ui->tableWidget->selectedItems().first()->row();
    QString questionId = ui->tableWidget->item(selectedRow,3)->text();
    const int selectedSubjId = subjectId[ui->subjectList2->currentIndex()];
    Network & network = Network::getInstance();
    QByteArray request,reply;
    QDataStream stream(&request, QIODevice::WriteOnly);
    stream << questionId.toInt();
    const int editOperationType = SUBJECT_LIST_FOR_QUESTION_ADD;
    stream << editOperationType;
    stream << selectedSubjId;
    reply = network.sendQuery(EDIT_SUBJECT_LIST_FOR_QUESTION,request);
    ui->subjectList->currentIndexChanged(ui->subjectList->currentIndex());
}
