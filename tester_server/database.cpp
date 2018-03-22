#include "database.h"

dataBase::dataBase()
{
    db = QSqlDatabase::addDatabase("QMYSQL", "mydb");
    db.setHostName("localhost");
    db.setDatabaseName("tester");
    db.setUserName("tester");
    db.setPassword("12345");
     // cout << db.lastError().number()<<endl;
    bool ok = db.open();
//    QSqlQuery query(db);
//    query.exec("SELECT * FROM subjects");
//    while (query.next()) {
//         QString name = QString::fromUtf8(query.value(0).toByteArray());
//         int salary = query.value(1).toInt();
//         string a = name.toStdString();
//         cout <<a << salary;
//       }
}


void dataBase::addSubject(QString name)
{
    QSqlQuery query(db);
    QSqlQuery q2(db);
    q2.exec("SELECT max(subject_id) FROM tester.subjects");
    q2.next();
    int maxId = q2.value(0).toInt();

     QString q("INSERT INTO subjects (name, subject_id) VALUES (");
     name.insert(0,"'");
     name.append("'");
     q.append(name+" , ");
     q.append(QString::number(maxId+1) + ")");
     query.exec(q);
}

void dataBase::delSubject(int id)
{
    QSqlQuery query(db);
     query.prepare("DELETE FROM subjects WHERE subject_id = ?");
     query.addBindValue(id);
     query.exec();
}

QVector<QVector<QString> > dataBase::findSubject(QString name)
{
    QSqlQuery query(db);
    QString q("SELECT name, subject_id FROM tester.subjects WHERE name LIKE ");
    name.insert(0,"'%");
    name.append("%'");
    q.append(name);
    query.exec(q);
    QVector<QVector<QString>> subjects;
    while(query.next()){
        QVector<QString> subject;
        for (int i=0;i<2;i++){
        subject.push_back(query.value(i).toString());
        }
        subjects.push_back(subject);
    }
    return subjects;
}
QString dataBase::textQuestionFormat(QString questionText,int type,QString answer)
{
    if (type == SELECT_QUESTION_TYPE){
        for(int i =1 ;i<questionText.size();i++){
            if(questionText[i] == QChar('$') && questionText[i-1] != QChar('#')){
                questionText[i] = '\n';
            }
        }
        questionText.append("\nправильный(е) ответ(ы):\n");
        addOnesToAnswerString(answer);
        questionText.append(answer);
    }
    if (type == INPUT_QUESTION_TYPE){
        questionText.append("\nправильный ответ:\n");
        questionText.append(answer);
    }
    if (type == MATCH_QUESTION_TYPE){
        for(int i =1 ;i<questionText.size();i++){
            if(questionText[i] == QChar('$') && questionText[i-1] != QChar('#')){
                questionText[i] = '\n';
            }
        }
    }
    return questionText;
}
void dataBase::addOnesToAnswerString(QString &answer)
{
    QStringList subjectIdList = answer.split(";",QString::SkipEmptyParts);
    answer.clear();
    foreach (QString subjectId, subjectIdList) {
        answer.append( QString::number(subjectId.toInt() + 1) + ";");
    }
    answer.chop(1);
}

void dataBase::setExamTime(int time,id)
{
    QSqlQuery query(db);
    query.prepare("UPDATE exams SET exam_time = ? WHERE exam_id = ?");
    query.addBindValue(time);
    query.addBindValue(id);
    query.exec();
}

void dataBase::deleteExam(int id)
{
    QSqlQuery query(db);
    query.prepare("DELETE FROM exams WHERE exam_id = ?");
    query.addBindValue(id);
    query.exec();
}

void dataBase::startExamForStudent(int studIds, int examId)
{
    QSqlQuery query(db);
    query.exec("SELECT MAX(exam_pass_id) FROM exam_pass_status");
    int passId = query.value(0).toInt();
    passId++;
    foreach (int studId, studIds) {
        query.prepare("INSERT INTO exam_pass_status (exam_pass_id,studId,exam_id) VALUES (?,?,?)");
        query.addBindValue(passId);
        query.addBindValue(studId);
        query.addBindValue(examId);
        query.exec();
        passId++;
    }
}
QVector<QVector<QString> > dataBase::getQuestions(int id)
{
    QSqlQuery query(db),subquery(db);
    QString idText = QString::number(id);
    idText.insert(0,"'%");
    idText.append("%'");
    QString q("SELECT q_type,q_text,subject_id,q_id,difficulty,q_answer,q_adv_data FROM questions WHERE subject_id LIKE (");
    q.append(idText+")");
    query.exec(q);
    QVector<QVector<QString> > questions;
    while(query.next()){
        QVector<QString> question;
        question.push_back(query.value(0).toString());
        const int q_type = query.value(0).toInt();
        QString questionText = query.value(1).toString();
        if (q_type == SEQUENCE_QUESTION_TYPE){
            QByteArray advData = query.value(6).toByteArray();
            QString sequenceTextNumbered;
            int currentStep = 2;
            for (int i=0; i<advData.size();i++) {
                if(advData[i] == 0){
                    advData[i] = '\n';
                }
            }
            QString sequenceText(QString::fromUtf8(advData));
            foreach (QChar c, sequenceText) {
                sequenceTextNumbered.append(c);
                if(c == QChar('\n')){
                    sequenceTextNumbered.append(QString::number(currentStep)+"\n");
                    currentStep++;
                }
            }
            sequenceTextNumbered.insert(0,"\n1\n");
            questionText.append("\nПоследовательность этапов в вопросе:"+sequenceTextNumbered);
            QString correctSequence = query.value(5).toString();
            addOnesToAnswerString(correctSequence);
            questionText.append("Правильная последовательность этапов:\n"+correctSequence);
        }
        if (q_type == MATCH_QUESTION_TYPE){
            questionText = textQuestionFormat(questionText,q_type,"");
            questionText.insert(0,"Для каждого утверждения выбрать одну из групп:");
            questionText.append("\n");
            QString groupNumbers = query.value(5).toString();
            addOnesToAnswerString(groupNumbers);
            QStringList groupNumbersList = groupNumbers.split(";",QString::SkipEmptyParts);
            int groupNumbersIndex = 0;
            QByteArray advData = query.value(6).toByteArray();
            advData.remove(0,1);//кол-во групп
            for (int i=0; i<advData.size();i++) {
                if(advData[i] == 0){
                    advData[i] = '\n';
                }
            }
            QString sequenceText(QString::fromUtf8(advData));
            foreach (QChar c, sequenceText) {
                questionText.append(c);
                if(c == QChar('\n')){
                    questionText.append(" -> "+groupNumbersList[groupNumbersIndex]+"\n");
                    groupNumbersIndex++;
                }
            }
        }
        question.push_back(textQuestionFormat(questionText,q_type,query.value(5).toString()));
        QString subjectNames;
        QStringList subjectIdList = query.value(2).toString().split(";",QString::SkipEmptyParts);
        foreach (QString subjectId, subjectIdList) {
            QString subq("SELECT name FROM subjects WHERE subject_id = ");
            subq.append(subjectId);
            subquery.exec(subq);
            subquery.next();
            subjectNames.append(subquery.value(0).toString()+" \n\r");
        }
        question.push_back(subjectNames);
        for (int i=3;i<5;i++){
        question.push_back(query.value(i).toString());
        }
        questions.push_back(question);
    }
    return questions;
}

void dataBase::removeQuestion(int id)
{
    QSqlQuery query(db);
    query.prepare("DELETE FROM tester.questions WHERE q_id = ?");
    query.addBindValue(id);
    query.exec();
}

void dataBase::editQuestionSubjects(int questionId, int editOperationType, int subjId)
{
    QSqlQuery query(db);
    query.prepare("SELECT subject_id FROM tester.questions WHERE q_id = ?");
    query.addBindValue(questionId);
    query.exec();
    query.next();
    QStringList subjects = query.value(0).toString().split(";",QString::SkipEmptyParts);

    QString newSubjList;
    if (editOperationType == SUBJECT_LIST_FOR_QUESTION_REMOVE){
        if (subjects.size() == 1){     //если этот предмет - единственный, к которому привязан вопрос
            removeQuestion(questionId);//удаляем вопрос
            return;
        }
     foreach (QString subjNumber, subjects) {
        if (subjNumber != QString::number(subjId)){
            newSubjList.append(subjNumber+";");
        }
     }
    newSubjList.chop(1);
    } else {
        newSubjList = query.value(0).toString()+";"+QString::number(subjId);
    }
    QString updateQuery("UPDATE questions SET subject_id='");
    updateQuery.append(newSubjList);
    updateQuery.append("' WHERE q_id = ");
    updateQuery.append(QString::number(questionId));
    query.exec(updateQuery);
}

void dataBase::addQuestion(int type, int subjId, QString question, QString answer, QByteArray advData, int difficulty)
{
    QSqlQuery query(db);
    query.exec("SELECT MAX(q_id) FROM tester.questions");
    query.next();
    int qId = query.value(0).toInt();
    qId++;
    QString q("INSERT INTO tester.questions (q_id,q_type,subject_id,q_text,q_answer,difficulty) VALUES (");
    q.append(QString::number(qId)+" , "+QString::number(type)+" , "+QString::number(subjId)+" , ");
    q.append("'"+question+"' , '"+answer+" , "+QString::number(difficulty)+"' )");
    query.exec(q);
    if(!advData.isEmpty()){
        query.prepare("UPDATE tester.questions SET q_adv_data = ? WHERE q_id = ?");
        query.addBindValue(advData);
        query.addBindValue(qId);
        query.exec();
    }
}

void dataBase::addExam(int subject, QDate date, QByteArray questionList)
{
    QSqlQuery query(db);
    query.exec("SELECT MAX(exam_id) FROM tester.exams");
    query.next();
    int examId = query.value(0).toInt();
    examId++;
    query.prepare("INSERT INTO tester.exams (exam_id,subject_id,date,question_list) VALUES (?,?,?,?)");
    query.addBindValue(examId);
    query.addBindValue(subject);
    query.addBindValue(date);
    query.addBindValue(questionList);
    query.exec();
    const QSqlResult * debug = query.result();
}
int dataBase::checkAnswer(int id,QVector<int> answers)
{
    QSqlQuery query(db);
    query.prepare("SELECT q_id,q_answer,difficulty,q_type FROM tester.questions WHERE q_id = ?");
    query.addBindValue(id);
    query.exec();
    query.next();
    int type = query.value(3).toInt();
    if (type == 1){
        QString _correctAnswers = query.value(1).toString();
        QStringList correctAnswers = _correctAnswers.split(";",QString::SkipEmptyParts);
        QSet<int> tmp;

        foreach(QString num, correctAnswers){
            tmp.insert( num.toInt());
        }
        if (tmp == answers.toList().toSet()){//последовательность ответов не важна
            return query.value(2).toInt();
        }
    }
    if (type == 3 ||type == 4){
        QString _correctAnswers = query.value(1).toString();
        QStringList correctAnswers = _correctAnswers.split(";",QString::SkipEmptyParts);
        QVector<int> tmp;
        foreach(QString num, correctAnswers){
            tmp.push_back(num.toInt());
        }
        if (tmp == answers){
            return query.value(2).toInt();
        }
    }
    return 0;
}
QVector<QString> dataBase::getSubjects()
{
    QSqlQuery query(db);
    query.exec("SELECT name FROM subjects");
    QVector<QString> ret;
    while (query.next()) {
         ret.push_back( QString::fromUtf8(query.value(0).toByteArray()) );
       }
    return ret;
}

int dataBase::getUserId(QString name)
{
     QSqlQuery query(db);
     query.prepare("SELECT stud_id FROM tester.students WHERE stud_document_id = ?");
     query.addBindValue(name);
     query.exec();
     if (query.next()){
        return query.value(0).toInt();
     }
     cout << "user id not found\n";
     return -1;
}

bool dataBase::checkUser(QString userName, QString password)
{
    QSqlQuery query(db);
    query.prepare("SELECT login,password FROM tester.users WHERE login = ? AND password =?");
    query.addBindValue(userName);
    query.addBindValue(password);
    query.exec();
    if (query.next()){
        return true;
    }
    return false;
}



int dataBase::checkAnswer(int id, QString answer)
{
    QSqlQuery query(db);
    query.prepare("SELECT q_id,q_answer,difficulty FROM tester.questions WHERE q_id = ?");
    query.addBindValue(id);
    query.exec();
    query.next();
    QString correctAnswer = query.value(1).toString();
    if (!QString::compare(answer,correctAnswer,Qt::CaseInsensitive)){
        return query.value(2).toInt();
    }
    return 0;
}

int dataBase::getStudentCurrentExamState(int id, int &subject_id, int &question_select_type, QByteArray &question_list, int &exam_id)
{
    QSqlQuery query(db);
    query.prepare("SELECT stud_id,exam_id,pass_status FROM tester.exam_pass_status WHERE stud_id = ?");
    //ищем строку с статусом экзамена, к которому в данный момент получен допуск (такой должен быть 1 т.к.
    //препод добавляет студента когда тот приходит на экзамен)
    query.addBindValue(id);
    query.exec();
    int pass_status;
    if (query.next()){
       cout << "getStudentState found\n";
       exam_id = query.value(1).toInt();
       pass_status = query.value(2).toInt();
       query.prepare("SELECT subject_id,question_select_type,question_list,exam_time FROM tester.exams WHERE exam_id = ? ");
       query.addBindValue(exam_id);
       query.exec();
       query.next();
       subject_id = query.value(0).toInt();
       question_select_type = query.value(1).toInt();
       question_list = query.value(2).toByteArray();
       int debug = question_list.size();
       QDateTime time_limit = QDateTime::currentDateTime();
       query.exec()
       return pass_status;
    }
    return -1;//нет студента в exam_pass_status, препод не добавил допущенного студента
}

int dataBase::getMaxMark(int id)
{
    QSqlQuery query(db);
    query.prepare("SELECT difficulty FROM tester.questions WHERE q_id = ?");
    query.addBindValue(id);
    query.exec();
    query.next();
    return query.value(0).toInt();
}

QByteArray dataBase::getQuestionsForExam(QByteArray question_list)
{
    QSqlQuery query(db);
    const int * q_ids =(const int *) question_list.constData();
    const int q_number = question_list.size()/sizeof(int);
    QByteArray byteArray;
    QDataStream stream(&byteArray, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::LittleEndian);
    stream << q_number ;
    for (int i=0;i<q_number;i++){
    int currentQuestion = q_ids[i];
    query.prepare("SELECT q_id,q_type,q_text,q_adv_data FROM tester.questions WHERE q_id = ?");
    query.addBindValue(currentQuestion);
    query.exec();
    query.next();
    stream << query.value(0).toInt() << query.value(1).toInt();
    QByteArray q_text = query.value(2).toByteArray();
    int q_textlen = query.value(2).toByteArray().size();
    stream << q_text;
    int adv_data_len = 0;
    QByteArray adv_data = query.value(3).toByteArray();
    if (! query.value(3).isNull()) {    //есть доп. данные
    adv_data_len = query.value(3).toByteArray().size();
    stream << adv_data;
    } else {
        stream << adv_data_len;
    }
    }
    return byteArray;
}

QVector<QVector<QString> > dataBase::findStudents(QVector<int> params, QVector<QString> values)
{
    QSqlQuery query(db);
    QString queryString("SELECT first_name,middle_name,last_name, students.group ,stud_document_id,stud_id FROM tester.students WHERE ");
    QString where;
    for (int i=0;i<params.size();i++){
        switch (params[i]) {
        case FILTER_FIRST_NAME:
            where += QString("first_name LIKE '%") + values[i] + QString("%' "); ;
            break;
        case FILTER_MIDDLE_NAME:
            where += QString("middle_name LIKE '%") + values[i] + QString("%' "); ;
            break;
        case FILTER_LAST_NAME:
            where += QString("last_name LIKE '%") + values[i] + QString("%' "); ;
            break;
        case FILTER_STUD_GROUP:
            where += QString("students.group LIKE '%") + values[i] + QString("%' "); ;
            break;
        case FILTER_STUD_TICKET:
            where += QString("stud_document_id LIKE '%") + values[i] + QString("%' "); ;
            break;
        default:
            break;
        }
        if (i < (params.size()-1)){
            where += QString(" AND ");
        }
    }
    queryString += where;
    query.exec(queryString);
    if(params.isEmpty()){
        query.clear();
         query.prepare("SELECT first_name,middle_name,last_name, students.group ,stud_document_id,stud_id FROM tester.students ");
        query.exec();
         //нет параметров фильтра
    }

    QVector<QVector<QString>> students;
    while(query.next()){
        QVector<QString> student;
        for (int i=0;i<6;i++){
        student.push_back(query.value(i).toString());
        }
        students.push_back(student);
    }
    return students;
}

void dataBase::addStudent(QVector<QString> values)
{
    QSqlQuery query(db),query2(db);
     query.exec("SELECT MAX(stud_id) FROM tester.students");
     query.next();
     int maxId = query.value(0).toInt() +1;

    QString queryString("INSERT INTO tester.students (first_name,middle_name,last_name, `students`.`group` ,stud_document_id,stud_id) VALUES ( ");
    for  (int i=0;i<5;i++){
       queryString+=(QString("'") + values[i] + QString("'"));
       queryString+= ", ";
    }
    queryString+=( QString::number(maxId));
    queryString+= ")";
    query2.exec(queryString);
    cout << query2.lastError().text().toStdString();
//    QString a(query2.executedQuery());

}

void dataBase::removeStudent(int id)
{
    QSqlQuery query(db);
    query.prepare("DELETE FROM tester.students WHERE stud_id = ?");
    query.addBindValue(id);
    query.exec();
}





