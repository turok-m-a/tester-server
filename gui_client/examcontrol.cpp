#include "examcontrol.h"
#include "ui_examcontrol.h"

ExamControl::ExamControl(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExamControl)
{
    studListView = false;
    studentListUpdateTimer = 0;
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
    //ui->examList->setColumnWidth(1,0);
    subjListIsEmpty = false;
    if(ui->subjList->count()){
        ui->subjList->currentIndexChanged(0);
    }
    ui->viewReport->hide();
}

ExamControl::~ExamControl()
{
    if(studentListUpdateTimer){
        delete studentListUpdateTimer;
    }
    delete ui;
}

void ExamControl::on_addStudToExam_clicked()
{
    StudControlWindow * studControlWindow = new StudControlWindow(this,true);
    studControlWindow->selectedStudents = &addedStudentsIds;
    studControlWindow->selectedStudentsNames = &addedStudentsNames;
    connect(studControlWindow,SIGNAL(studentsAdded()),this,SLOT(addStudToExam()));
    studControlWindow->setAttribute(Qt::WA_DeleteOnClose);//удаление объекта по закрытию окна
    studControlWindow->show();
}

void ExamControl::on_selectExam_clicked()
{

    studListView = true;
    int selectedRow = ui->examList->selectedItems().first()->row();
    selectedExamId = ui->examList->item(selectedRow,0)->text().toInt();
    examTime = ui->examTime->text().toInt() * 60;//время экзамена в секундах
    QByteArray byteArray,reply;
    Network & network = Network::getInstance();
     QDataStream stream(&byteArray, QIODevice::WriteOnly);
    stream << examTime << selectedExamId;
    reply = network.sendQuery(SET_EXAM_TIME,byteArray);

    ui->examList->setRowCount(0);
    ui->examList->setColumnCount(3);
    ui->examList->setColumnWidth(2,15);
    ui->subjList->setEnabled(false);
    ui->selectExam->setEnabled(false);
    ui->addStudToExam->setEnabled(true);
    ui->examList->setHorizontalHeaderItem(0,new QTableWidgetItem("ФИО"));
    ui->examList->setHorizontalHeaderItem(1,new QTableWidgetItem("оценка"));
    ui->examList->setHorizontalHeaderItem(2,new QTableWidgetItem("ID"));

    if (!reply.isEmpty()){
         QDataStream replyStream(&reply, QIODevice::ReadOnly);
         QVector<QVector<QString>> passStatus;
         replyStream >> passStatus;
         for(int i=0;i<passStatus.size();i++){
             ui->examList->insertRow(0);
             ui->examList->setItem(0,0,new QTableWidgetItem(passStatus[i][0]));
             studNames.push_back(passStatus[i][0]);
             ui->examList->setItem(0,2,new QTableWidgetItem(passStatus[i][1]));
             studIds.push_back(passStatus[i][2].toInt());
         }
    }
    studentListUpdateTimer = new QTimer(this);
    connect(studentListUpdateTimer,SIGNAL(timeout()),this,SLOT(updateStudList()));
    studentListUpdateTimer->start(5000);
    ui->delExam->hide();

}
void ExamControl::on_viewExamHistory_clicked()
{
    int selectedRow = ui->examList->selectedItems().first()->row();
    selectedExamId = ui->examList->item(selectedRow,0)->text().toInt();

}
void ExamControl::on_examList_itemSelectionChanged()
{
     if (studListView) return;
    ui->selectExam->setEnabled(true);
    if (ui->examList->selectedItems().isEmpty()) return;
    int selectedRow = ui->examList->selectedItems().first()->row();
    QString examDate = ui->examList->item(selectedRow,1)->text();
    QDate date = QDate::fromString(examDate,"yyyy-MM-dd");
    QDate currentDate = QDate::currentDate();
    if(currentDate > date){
        ui->selectExam->setText("Просмотреть результаты");
        ui->addStudToExam->setEnabled(false);
        ui->examTime->setEnabled(false);
        ui->addStudToExam->setEnabled(false);
        ui->viewReport->show();
    } else {
        ui->selectExam->setText("Начать/продолжить экзамен");
        ui->addStudToExam->setEnabled(true);
        ui->examTime->setEnabled(true);
        ui->addStudToExam->setEnabled(true);
        ui->viewReport->hide();
    }
}

void ExamControl::on_subjList_currentIndexChanged(int index)
{   //получить список экзаменов по предмету
    if (subjListIsEmpty) return;
    QByteArray byteArray,reply;
    Network & network = Network::getInstance();
     QDataStream stream(&byteArray, QIODevice::WriteOnly);
    stream << subjectId[ui->subjList->currentIndex()];
    reply = network.sendQuery(GET_EXAM_LIST,byteArray);
    QDataStream replyStream(&reply, QIODevice::ReadOnly);
    QVector<QVector<QString>> exams;
    replyStream >> exams;
    ui->examList->setRowCount(0);
    for (int i=0;i<exams.size();i++){
        ui->examList->insertRow(0);
        ui->examList->setItem(0,0,new QTableWidgetItem(exams[i][0]));
        ui->examList->setItem(0,1,new QTableWidgetItem(exams[i][1]));
    }
}

void ExamControl::updateStudList()
{
    QByteArray byteArray,reply;
    QVector<int> passIds;
    QVector<float> passState;
    QString columnText;
    for (int i=0;i<ui->examList->rowCount();i++){
        passIds.push_back(ui->examList->item(i,2)->text().toInt());
    }
    QDataStream stream(&byteArray, QIODevice::WriteOnly);
    stream << passIds;
    Network & network = Network::getInstance();
    reply = network.sendQuery(GET_PASS_STATE,byteArray);
    QDataStream replyStream(&reply, QIODevice::ReadOnly);
    replyStream >> passState;
    for (int i=0;i<ui->examList->rowCount();i++){
        if (passState[i] == 11){
            columnText = "--";
        } else {
            columnText = QString::number(passState[i]);
        }
        ui->examList->setItem(i,1,new QTableWidgetItem(columnText));
    }
}

void ExamControl::addStudToExam()
{
    QByteArray byteArray,reply;
    QDataStream stream(&byteArray, QIODevice::WriteOnly);


    stream << addedStudentsIds.size();
    stream << selectedExamId;
    foreach (int studId, addedStudentsIds) {
        stream << studId;
    }
    Network & network = Network::getInstance();
    reply = network.sendQuery(START_EXAM_FOR_STUDENT,byteArray);
    QDataStream replyStream(&reply, QIODevice::ReadOnly);
    QVector<int> passIds;
    replyStream >> passIds;
   // ui->examList->setRowCount(0);

    for (int i=0;i<addedStudentsIds.size();i++){
        ui->examList->insertRow(0);
        ui->examList->setItem(0,0,new QTableWidgetItem(addedStudentsNames[i]));
        ui->examList->setItem(0,2,new QTableWidgetItem(QString::number(passIds[i])));
    }
    studIds = addedStudentsIds + studIds;
    studNames = addedStudentsNames + studNames;
    addedStudentsIds.clear();
    addedStudentsNames.clear();

}



void ExamControl::on_viewReport_clicked()
{
    if (ui->examList->selectedItems().isEmpty()) return;
    int selectedRow = ui->examList->selectedItems().first()->row();
    if(studListView){
        QString debug = ui->examList->item(selectedRow,2)->text();
        ReportView * view = new ReportView(ui->examList->item(selectedRow,2)->text(),this);
        view->setAttribute(Qt::WA_DeleteOnClose);
        view->show();
    }
}
