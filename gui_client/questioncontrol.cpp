#include "questioncontrol.h"
#include "ui_questioncontrol.h"

QuestionControl::QuestionControl(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QuestionControl)
{
    subjListIsEmpty = true;
    ui->setupUi(this);
    ui->groupBox->hide();
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
     ui->tableWidget->setColumnWidth(2,100);
     ui->tableWidget->setColumnWidth(3,0);
     ui->tableWidget->setColumnWidth(4,40);
     ui->tableWidget->setColumnWidth(5,15);
     subjListIsEmpty = false;
     ui->subjectList->currentIndexChanged(ui->subjectList->currentIndex());
     ui->answersNumber->setValidator(new QIntValidator(2,10,this));
     ui->questionEdit->setColumnCount(1);
     ui->questionEdit->setColumnWidth(0,400);
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

void QuestionControl::on_addQuestion_clicked()
{
    ui->tableWidget->hide();
    ui->groupBox->show();
}

void QuestionControl::on_finishQustionAdd_clicked()
{
    const int index = ui->questionType->currentIndex();
    QString answer;
    QString questionText;
    QByteArray request,advData;
    if (index+1 == SELECT_QUESTION_TYPE){
        questionText = ui->questionEdit->item(0,0)->text();
        formatQuestionText(questionText);
        for (int i=1;i<ui->questionEdit->rowCount();i++){
            questionText.append(" $"+QString::number(i)+" ");
            QString currentAnswer = ui->questionEdit->item(i,0)->text();
            formatQuestionText(currentAnswer);
            questionText.append(currentAnswer);
            if(qobject_cast<QCheckBox*>(ui->questionEdit->cellWidget(i,1))->isChecked()){
                answer.append(QString::number(i-1)+";");
            }
        }
        answer.chop(1);
    }
    if (index+1 == INPUT_QUESTION_TYPE){
        questionText = ui->questionEdit->item(0,0)->text();
        answer = ui->questionEdit->item(1,0)->text();
    }
    if (index+1 == SEQUENCE_QUESTION_TYPE){
       questionText = ui->questionEdit->item(0,0)->text();
       for (int i=1;i<ui->questionEdit->rowCount();i++){
                QString stepNumber = QString::number(ui->questionEdit->item(i,1)->text().toInt() - 1);
               answer.append(stepNumber+";");
               advData.append(QString(ui->questionEdit->item(i,0)->text()+" "));
               advData.append('\0');
       }
       answer.chop(1);
    }
    QDataStream stream(&request, QIODevice::WriteOnly);
    stream << index+1 << subjectId[ui->subjectList->currentIndex()]
           << questionText << answer;
    if (!advData.isEmpty()){
        stream << advData;
    }
    Network & network = Network::getInstance();
    network.sendQuery(ADD_QUESTION,request);
    ui->subjectList->currentIndexChanged(ui->subjectList->currentIndex());
    ui->groupBox->hide();
    ui->tableWidget->show();
}

void QuestionControl::on_questionType_currentIndexChanged(int index)
{
    if (index+1 == SELECT_QUESTION_TYPE){
        ui->answersNumber->show();
        ui->label_2->show();
        ui->questionEdit->setRowCount(3);
        ui->questionEdit->setColumnCount(2);
        ui->questionEdit->setItem(0,0,new QTableWidgetItem("Вводите текст вопроса здесь"));
        ui->questionEdit->setItem(1,0,new QTableWidgetItem("Вариант ответа 1"));
        ui->questionEdit->setItem(2,0,new QTableWidgetItem("Вариант ответа 2"));
        ui->questionEdit->setCellWidget(1,1,new QCheckBox());
        ui->questionEdit->setCellWidget(2,1,new QCheckBox());
        ui->questionEdit->setColumnWidth(1,15);
        ui->questionEdit->setRowHeight(0,100);
        ui->answersNumber->setText("2");
    }
    if (index+1 == INPUT_QUESTION_TYPE){
        ui->answersNumber->hide();
        ui->label_2->hide();
        ui->questionEdit->setRowCount(2);
        ui->questionEdit->setColumnCount(1);
        ui->questionEdit->setItem(0,0,new QTableWidgetItem("Вводите текст вопроса здесь"));
        ui->questionEdit->setItem(1,0,new QTableWidgetItem("текст ответа"));
        ui->questionEdit->setRowHeight(0,100);
        ui->answersNumber->setText("2");
    }
    if (index+1 == SEQUENCE_QUESTION_TYPE){
        ui->answersNumber->show();
        ui->label_2->show();
        ui->questionEdit->setRowCount(2);
        ui->questionEdit->setColumnCount(2);
        ui->questionEdit->setItem(0,0,new QTableWidgetItem("Вводите текст вопроса здесь"));
        ui->questionEdit->setItem(0,1,new QTableWidgetItem("порядков ответов в вопросе"));
        ui->questionEdit->setItem(1,0,new QTableWidgetItem("шаг 1"));
        ui->questionEdit->setItem(1,1,new QTableWidgetItem("6"));
        ui->questionEdit->setRowHeight(0,100);
        ui->answersNumber->setText("1");
    }
}



void QuestionControl::on_answersNumber_editingFinished()
{
    int newRowCount = ui->answersNumber->text().toInt() + 1;
    int oldRowCount = ui->questionEdit->rowCount();
    ui->questionEdit->setRowCount(newRowCount);
    for (int i=oldRowCount;i<newRowCount;i++){
        if (ui->questionType->currentIndex() +1 == SELECT_QUESTION_TYPE){
            ui->questionEdit->setCellWidget(i,1,new QCheckBox());
        } else {
            ui->questionEdit->setItem(i,0,new QTableWidgetItem());
        }
        ui->questionEdit->setItem(i,0,new QTableWidgetItem());
    }
}

void QuestionControl::formatQuestionText(QString &text)
{
    for (int i=1;i<text.size();i++){
        if(text[i] == '$' || text[i] == '#'){
            text.insert(i,'#');
            i++;
        }
    }
}
