#include "questioncontrol.h"
#include "ui_questioncontrol.h"

QuestionControl::QuestionControl(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QuestionControl)
{
    signalMapper = new QSignalMapper(this);
    connect(signalMapper,SIGNAL(mapped(QString)),this,SLOT(questionCheckStateChanged(QString)));
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
     ui->answersNumber2->setValidator(new QIntValidator(2,6,this));
     ui->difficulty->setValidator(new QIntValidator(1,9,this));
     ui->questionEdit->setColumnCount(1);
     ui->questionEdit->setColumnWidth(0,360);
     ui->chooseExamDate->hide();
     ui->addExam->hide();
     ui->calendarWidget->hide();

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

     loadTest a[100];
     int pause = 1;
     for (int i=0;i<100;i++){
          a[i].pause = &pause;
          a[i].opCode = GET_QUESTION_LIST;
          a[i].r = byteArray;
          a[i].start();
     }
     qDebug() << "!";
     pause = 0;

     reply = network.sendQuery(GET_QUESTION_LIST,byteArray);
     int qNumber;
     stream2 >> qNumber;
   stream2 >> questions;
    QString columnText;
     for (int i=0;i<qNumber;i++){
         ui->tableWidget->insertRow(0);
         for (int j=0;j<5;j++){//тип,вопрос-ответ,предмет,ID(скрыт),сложность
             columnText = questions[i][j];
             if(j==0){
                 if (columnText.toInt() == 1){
                     ui->tableWidget->setItem(0,j,new QTableWidgetItem("Выб.\nвар."));
                 }
                 if (columnText.toInt() == 2){
                     ui->tableWidget->setItem(0,j,new QTableWidgetItem("Ввод"));
                 }
                 if (columnText.toInt() == 3){
                     ui->tableWidget->setItem(0,j,new QTableWidgetItem("Посл."));
                 }
                 if (columnText.toInt() == 4){
                     ui->tableWidget->setItem(0,j,new QTableWidgetItem("Груп."));
                 }
             } else {

          ui->tableWidget->setItem(0,j,new QTableWidgetItem(columnText));
             }
         }
         QCheckBox * chkBox = new QCheckBox();
         connect(chkBox,SIGNAL(toggled(bool)),signalMapper,SLOT(map()));
         signalMapper->setMapping(chkBox,questions[i][3]);
         ui->tableWidget->setCellWidget(0,5,chkBox);
     }
     ui->tableWidget->resizeRowsToContents();

     Sleep(1000);
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
    ui->nRandButton->hide();
    ui->nRandNum->hide();
    ui->pushButton_3->hide();

    ui->tableWidget->hide();
    ui->groupBox->show();
    ui->questionType->currentIndexChanged(0);
}

void QuestionControl::on_finishQustionAdd_clicked()
{
    ui->nRandButton->show();
    ui->nRandNum->show();
    ui->pushButton_3->show();

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
               advData.append(" "+QString(ui->questionEdit->item(i,0)->text()+" "));
               advData.append('\0');
       }
       answer.chop(1);
    }
    if (index+1 == MATCH_QUESTION_TYPE){
       advData.append((byte)(ui->groups->rowCount()));
       for (int i=1;i<ui->questionEdit->rowCount();i++){
               advData.append(" "+ui->questionEdit->item(i,0)->text());
               advData.append('\0');
               QString answerNum = QString::number(ui->questionEdit->item(i,1)->text().toInt() - 1);
               answer.append(answerNum+";");
       }
       for (int i=0;i<ui->groups->rowCount();i++){
               questionText.append(" $"+QString::number(i+1)
                                   +ui->groups->item(i,0)->text());;
       }
       answer.chop(1);
    }
    QDataStream stream(&request, QIODevice::WriteOnly);
    stream << index+1 << subjectId[ui->subjectList->currentIndex()]
           << questionText << answer;
    stream << ui->difficulty->text().toInt();
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
    ui->answersNumber->show();
    ui->label_2->show();
    ui->label_3->hide();
    ui->answersNumber2->hide();
    ui->groups->hide();
    if (index+1 == SELECT_QUESTION_TYPE){
        ui->label_2->setText("Вариантов ответа:");
        ui->questionEdit->setRowCount(0);
        ui->questionEdit->setRowCount(3);
        ui->questionEdit->setColumnCount(0);//очистить содержимое
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
        ui->questionEdit->setRowCount(0);
        ui->questionEdit->setRowCount(2);
        ui->questionEdit->setColumnCount(0);//очистить содержимое
        ui->questionEdit->setColumnCount(1);
        ui->questionEdit->setItem(0,0,new QTableWidgetItem("Вводите текст вопроса здесь"));
        ui->questionEdit->setItem(1,0,new QTableWidgetItem("текст ответа"));
        ui->questionEdit->setRowHeight(0,100);
        ui->answersNumber->setText("2");
    }
    if (index+1 == SEQUENCE_QUESTION_TYPE){
        ui->label_2->setText("Этапов:");
        ui->questionEdit->setRowCount(0);
        ui->questionEdit->setRowCount(2);
        ui->questionEdit->setColumnCount(0);//очистить содержимое
        ui->questionEdit->setColumnCount(2);
        ui->questionEdit->setItem(0,0,new QTableWidgetItem("Вводите текст вопроса здесь"));
        ui->questionEdit->setItem(0,1,new QTableWidgetItem("правильный\nпорядок\nответов"));
        ui->questionEdit->setItem(1,0,new QTableWidgetItem("шаг 6"));
        ui->questionEdit->setItem(1,1,new QTableWidgetItem("6"));
        ui->questionEdit->setRowHeight(0,100);
        ui->questionEdit->setColumnWidth(1,60);
        ui->questionEdit->resizeColumnsToContents();
        ui->answersNumber->setText("1");

    }
    if (index+1 == MATCH_QUESTION_TYPE){
        ui->questionEdit->clearContents();
        ui->label_2->setText("Кол-во ответов:");
        ui->label_3->setText("Групп:");
        ui->label_3->show();
        ui->groups->show();
        ui->answersNumber2->show();
        ui->questionEdit->setColumnCount(0);//очистить содержимое
        ui->questionEdit->setColumnCount(2);
        ui->questionEdit->setRowCount(0);
        ui->questionEdit->setRowCount(2);
        ui->questionEdit->setItem(0,0,new QTableWidgetItem("Ответы для группировки:"));
        ui->questionEdit->setItem(0,1,new QTableWidgetItem("Номера групп:"));
        ui->questionEdit->setItem(1,0,new QTableWidgetItem("элемент первой группы"));
        ui->questionEdit->setItem(1,1,new QTableWidgetItem("1"));
        ui->groups->setItem(0,0,new QTableWidgetItem("Группа 1"));
        ui->answersNumber->setText("1");
    }
    ui->questionEdit->setColumnWidth(0,360);
}



void QuestionControl::on_answersNumber_editingFinished()
{
    int newRowCount = ui->answersNumber->text().toInt() + 1;
    int oldRowCount = ui->questionEdit->rowCount();
    ui->questionEdit->setRowCount(newRowCount);
    for (int i=oldRowCount;i<newRowCount;i++){
        if (ui->questionType->currentIndex() +1 == SELECT_QUESTION_TYPE){
            ui->questionEdit->setCellWidget(i,1,new QCheckBox());
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



void QuestionControl::on_answersNumber2_editingFinished()
{
    int newRowCount = ui->answersNumber2->text().toInt();
    int oldRowCount = ui->groups->rowCount();
    ui->groups->setRowCount(newRowCount);
    for (int i=oldRowCount;i<newRowCount;i++){
        ui->groups->setItem(i,0,new QTableWidgetItem());
    }
}

void QuestionControl::on_groups_itemChanged(QTableWidgetItem *item)
{
    ui->groups->resizeRowsToContents();
}

void QuestionControl::on_questionEdit_itemChanged(QTableWidgetItem *item)
{
    ui->questionEdit->resizeRowsToContents();
}

void QuestionControl::on_pushButton_3_clicked()
{
    ui->calendarWidget->show();
    ui->chooseExamDate->show();
    ui->addExam->show();
    ui->groupBox->hide();
    ui->tableWidget->hide();
}

void QuestionControl::on_addExam_clicked()
{
    const int currentSubjId = subjectId[ui->subjectList->currentIndex()];
    QByteArray request,questionList;
    QDataStream stream(&request, QIODevice::WriteOnly);
    stream << currentSubjId;
    QDataStream qStream(&questionList, QIODevice::WriteOnly);
    qStream.setByteOrder(QDataStream::LittleEndian);
//    for (int i=0;i<ui->tableWidget->rowCount();i++){
//        if(qobject_cast<QCheckBox*>(ui->tableWidget->cellWidget(i,5))->isChecked()){
//            int qId = ui->tableWidget->item(i,3)->text().toInt();
//            qStream << qId;
//        }
//    }   //не то
    foreach (int qId, selectedQuestionIds) {
        qStream << qId;
    }
    QDate date = ui->calendarWidget->selectedDate();
    //QString dateStr = date.toString(Qt::ISODate);
    stream << date;
    stream << questionList;
    Network & network = Network::getInstance();
    network.sendQuery(ADD_EXAM,request);
    ui->calendarWidget->hide();
    ui->chooseExamDate->hide();
    ui->addExam->hide();

    ui->tableWidget->show();
}

void QuestionControl::questionCheckStateChanged(QString id)
{
    int qId = id.toInt();
    int index = selectedQuestionIds.indexOf(qId);
    if (index == -1){
        selectedQuestionIds.push_back(qId);
        qDebug() << qId;
    } else {
        selectedQuestionIds.remove(index);
    }
}

void QuestionControl::on_findQuestion_textChanged(const QString &arg1)
{
    ui->tableWidget->setRowCount(0);
    bool ok;
    int  belowDifficulty = ui->below->text().toInt(&ok);
    if (!ok && !ui->below->text().isEmpty()) return;
    if (ui->below->text().isEmpty()){
        belowDifficulty = 10;
    }
    int  aboveDifficulty = ui->above->text().toInt(&ok);
    if (!ok && !ui->above->text().isEmpty()) return;
    if (ui->above->text().isEmpty()){
        aboveDifficulty = 0;
    }
    for (int i=0;i<questions.size();i++){
        if (questions[i][4].toInt() > belowDifficulty || questions[i][4].toInt() < aboveDifficulty || !questions[i][1].contains(
                    ui->findQuestion->text(),Qt::CaseInsensitive)){
            continue;
        }
        ui->tableWidget->insertRow(0);
        for (int j=0;j<5;j++){//тип,вопрос-ответ,предмет,ID(скрыт),сложность
         QString columnText = questions[i][j];
         ui->tableWidget->setItem(0,j,new QTableWidgetItem(columnText));
        }
        QCheckBox * chkBox = new QCheckBox();
        if (selectedQuestionIds.contains(questions[i][3].toInt())){
            chkBox->setChecked(true);
        }
        connect(chkBox,SIGNAL(toggled(bool)),signalMapper,SLOT(map()));
        signalMapper->setMapping(chkBox,questions[i][3]);
        ui->tableWidget->setCellWidget(0,5,chkBox);
    }
    ui->tableWidget->resizeRowsToContents();
}



void QuestionControl::on_above_textChanged(const QString &arg1)
{
    bool ok;
    int aboveDifficulty = arg1.toInt(&ok);
    if (!ok && !arg1.isEmpty()) return;
    if (arg1.isEmpty()){
        aboveDifficulty = 0;
    }
    int  belowDifficulty = ui->below->text().toInt(&ok);
    if (!ok && !ui->below->text().isEmpty()) return;
    if (ui->below->text().isEmpty()){
        belowDifficulty = 10;
    }
    ui->tableWidget->setRowCount(0);
    for (int i=0;i<questions.size();i++){
        if (questions[i][4].toInt() > belowDifficulty || questions[i][4].toInt() < aboveDifficulty || !questions[i][1].contains(
                    ui->findQuestion->text(),Qt::CaseInsensitive)){
            continue;
        }
        ui->tableWidget->insertRow(0);
        for (int j=0;j<5;j++){//тип,вопрос-ответ,предмет,ID(скрыт),сложность
         QString columnText = questions[i][j];
         ui->tableWidget->setItem(0,j,new QTableWidgetItem(columnText));
        }
        QCheckBox * chkBox = new QCheckBox();
        if (selectedQuestionIds.contains(questions[i][3].toInt())){
            chkBox->setChecked(true);
        }
        connect(chkBox,SIGNAL(toggled(bool)),signalMapper,SLOT(map()));
        signalMapper->setMapping(chkBox,questions[i][3]);
        ui->tableWidget->setCellWidget(0,5,chkBox);
    }
    ui->tableWidget->resizeRowsToContents();
}

void QuestionControl::on_below_textChanged(const QString &arg1)
{
    bool ok;
    int  belowDifficulty = arg1.toInt(&ok);
    if (!ok && !arg1.isEmpty()) return;
    if (arg1.isEmpty()){
        belowDifficulty = 10;
    }
    int  aboveDifficulty = ui->above->text().toInt(&ok);
    if (!ok && !ui->above->text().isEmpty()) return;
    if (ui->above->text().isEmpty()){
        aboveDifficulty = 0;
    }
    ui->tableWidget->setRowCount(0);

    for (int i=0;i<questions.size();i++){
        if (questions[i][4].toInt() > belowDifficulty || questions[i][4].toInt() < aboveDifficulty || !questions[i][1].contains(
                    ui->findQuestion->text(),Qt::CaseInsensitive)){
            continue;
        }
        ui->tableWidget->insertRow(0);
        for (int j=0;j<5;j++){//тип,вопрос-ответ,предмет,ID(скрыт),сложность
         QString columnText = questions[i][j];
         ui->tableWidget->setItem(0,j,new QTableWidgetItem(columnText));
        }
        QCheckBox * chkBox = new QCheckBox();
        if (selectedQuestionIds.contains(questions[i][3].toInt())){
            chkBox->setChecked(true);
        }
        connect(chkBox,SIGNAL(toggled(bool)),signalMapper,SLOT(map()));
        signalMapper->setMapping(chkBox,questions[i][3]);
        ui->tableWidget->setCellWidget(0,5,chkBox);
    }
    ui->tableWidget->resizeRowsToContents();
}

void QuestionControl::on_nRandButton_clicked()
{
    QTime midnight(0,0,0);
    qsrand(midnight.secsTo(QTime::currentTime()));
    bool ok;
    QVector<int> randQuestionsIds;
    int n = ui->nRandNum->text().toInt(&ok);
    if (!ok) return;
    for(int i=0;i<n;i++){
        int randRow= qrand() % ui->tableWidget->rowCount();
        int qID = ui->tableWidget->item(randRow,3)->text().toInt();
        if (randQuestionsIds.contains(qID)){
            i--;
        } else {
            randQuestionsIds.push_back(qID);
        }
    }
    selectedQuestionIds = randQuestionsIds;
    on_below_textChanged(ui->below->text());
}
