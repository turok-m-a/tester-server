#include "examcontrol.h"
#include "ui_examcontrol.h"

ExamControl::ExamControl(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExamControl)
{
    subjListIsEmpty = true;
    ui->setupUi(this);
    QByteArray byteArray;   //формирование списка предметов
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
        ui->subjList->addItem(subjName);
        stream2 >> subjId;
        subjectId.push_back(subjId.toInt());
    }
    ui->examList->setColumnWidth(1,0);
    subjListIsEmpty = false;
}

ExamControl::~ExamControl()
{
    delete ui;
}

void ExamControl::on_addStudToExam_clicked()
{
    StudControlWindow * studControlWindow = new StudControlWindow(this,true);
    studControlWindow->selectedStudents = &studIds;
    studControlWindow->setAttribute(Qt::WA_DeleteOnClose);//удаление объекта по закрытию окна
    studControlWindow->show();
}

void ExamControl::on_selectExam_clicked()
{
    int selectedRow = ui->examList->selectedItems().first()->row();
    selectedExamId = ui->examList->item(selectedRow,1)->text().toInt();
    examTime = ui->examTime->text().toInt() * 60;//время укзамена в секундах
    QByteArray byteArray;
    Network & network = Network::getInstance();
     QDataStream stream(&byteArray, QIODevice::WriteOnly);
    stream << examTime << selectedExamId;
    network.sendQuery(SET_EXAM_TIME,byteArray);
    ui->examList->setRowCount(0);
    ui->subjList->setEnabled(false);
}

void ExamControl::on_examList_itemSelectionChanged()
{
    ui->selectExam->setEnabled(true);
}

void ExamControl::on_subjList_currentIndexChanged(int index)
{
    if (subjListIsEmpty) return;
    QByteArray byteArray,reply;
    QString columnText;
    int examsCount,id;
    Network & network = Network::getInstance();
     QDataStream stream(&byteArray, QIODevice::WriteOnly);
    stream << subjectId[ui->subjList->currentIndex()];
    reply = network.sendQuery(GET_EXAM_LIST,byteArray);
    QDataStream replyStream(&reply, QIODevice::ReadOnly);
    replyStream >> examsCount;
    for (int i=0;i<examsCount;i++){
        ui->examList->insertRow(0);
        replyStream >> id;
        replyStream >> columnText;
        ui->examList->setItem(0,0,new QTableWidgetItem(QString::number(id)));
        ui->examList->setItem(0,1,new QTableWidgetItem(columnText));
    }
}

void ExamControl::addStudToExam()
{
    QByteArray byteArray;
    QDataStream stream(&byteArray, QIODevice::WriteOnly);
    stream << studIds.size();
    stream << selectedExamId;
    foreach (int studId, studIds) {
        stream << studId;
    }
    studIds.clear();
    Network & network = Network::getInstance();
    network.sendQuery(START_EXAM_FOR_STUDENT,byteArray);
}
