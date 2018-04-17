#include "database.h"

dataBase::dataBase()
{
    db = QSqlDatabase::addDatabase("QMYSQL", "mydb");
    db.setHostName("localhost");
    db.setDatabaseName("tester");
    db.setUserName("tester");
    db.setPassword("12345");
    bool ok = db.open();
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

void dataBase::setExamTime(int time, int id,bool & alreadySet)
{
    QSqlQuery query(db);
    query.prepare("SELECT exam_time FROM exams WHERE exam_id = ?");
    query.addBindValue(id);
    query.exec();
    query.next();
    if (!query.value(0).isNull()){
        alreadySet = true;
        return;
    }
    query.prepare("UPDATE exams SET exam_time = ? WHERE exam_id = ?");
    query.addBindValue(time);
    query.addBindValue(id);
    query.exec();
    alreadySet = false;
}

void dataBase::deleteExam(int id)
{
    QSqlQuery query(db);
    query.prepare("DELETE FROM exams WHERE exam_id = ?");
    query.addBindValue(id);
    query.exec();
}

void dataBase::startExamForStudent(QVector<int> studIds, int examId, QVector<int> &passIds)
{
    QSqlQuery query(db);
    query.exec("SELECT MAX(exam_pass_id) FROM exam_pass_status");
    query.next();
    int passId = query.value(0).toInt();
    passId++;
    foreach (int studId, studIds) {
        query.prepare("INSERT INTO exam_pass_status (exam_pass_id,stud_id,exam_id,pass_status) VALUES (?,?,?,11.0)");
        query.addBindValue(passId);
        query.addBindValue(studId);
        query.addBindValue(examId);
        query.exec();
        passIds.push_back(passId);
        passId++;
    }
}

bool dataBase::timeLimitExceed(int studId)
{
    QSqlQuery query(db);
    query.prepare("SELECT time_limit FROM exam_pass_status WHERE pass_status = 11 AND stud_id = ?");
    query.addBindValue(studId);
    query.exec();
    query.next();
    QDateTime current = QDateTime::currentDateTime();
    if (current > query.value(0).toDateTime()){
        return true;
    }
    return false;
}

QVector<QVector<QString> > dataBase::getExamList(int subjId)
{
    QSqlQuery query(db);
    query.prepare("SELECT exam_id,date FROM exams WHERE subject_id = ?");
    query.addBindValue(subjId);
    query.exec();
    QVector<QVector<QString>> exams;
    while (query.next()){
        QVector<QString> exam;
        exam.push_back(query.value(0).toString());
        exam.push_back(query.value(1).toString());
        exams.push_back(exam);
    }
    return exams;
}

QVector<float> dataBase::getStudentPassStatus(QVector<int> passIds, int examId)
{
    QSqlQuery query(db);
    QVector<float> state;

    foreach (int id, passIds) {
        query.prepare("SELECT pass_status FROM exam_pass_status WHERE exam_pass_id = ?");
        query.addBindValue(id);
        query.exec();
        query.next();
        state.push_back(query.value(0).toFloat());
    }
    return state;
}

void dataBase::setStudentMark(int studId, float mark)
{
    QSqlQuery query(db);
    query.prepare("UPDATE exam_pass_status SET pass_status = ? WHERE stud_id = ? AND pass_status = 11");
    query.addBindValue(mark);
    query.addBindValue(studId);
    query.exec();
}

QVector<QVector<QString> > dataBase::getExamStudList(int examId)
{
    QSqlQuery query(db);
    QVector<QVector<QString>> studList;
    QVector<int> passIds;
    QVector<int> studIds;
        query.prepare("SELECT exam_pass_id,stud_id FROM exam_pass_status WHERE exam_id = ?");
        query.addBindValue(examId);
        query.exec();
        while(query.next()){
            passIds.push_back(query.value(0).toInt());
            studIds.push_back(query.value(1).toInt());
        }
        for (int i=0;i<studIds.size();i++) {
            QVector<QString> status;
            query.prepare("SELECT last_name,first_name FROM students WHERE stud_id = ?");
            query.addBindValue(studIds[i]);
            query.exec();
            query.next();
            status.push_back(query.value(0).toString()+" "+query.value(1).toString());
            status.push_back(QString::number(passIds[i]));
            status.push_back(QString::number(studIds[i]));
            studList.push_back(status);
        }
        return studList;
}

int dataBase::getExamTime(int exam_id)
{
     QSqlQuery query(db);
     query.prepare("SELECT exam_time FROM exams WHERE exam_id = ?");
     query.addBindValue(exam_id);
     query.exec();
     query.next();
     return query.value(0).toInt();
}

void dataBase::addTextNote(QString studAnswer, int id, int studId)
{
    QString q;
    QSqlQuery checkIfNull(db);
    checkIfNull.exec("SELECT text_note FROM exam_pass_status WHERE stud_id = "+QString::number(studId)+" AND pass_status = 11");
    checkIfNull.next();
    if (checkIfNull.value(0).isNull()){
        q = "UPDATE exam_pass_status SET text_note = \"";
    } else {
        q = "UPDATE exam_pass_status SET text_note = CONCAT(text_note,\"";
    }
    QVector<QVector<QString>> questions = getQuestions(-1,id); //тип, вопрос,...,id,diff,ответ
    q.append("\n"+questions[0][1]);
    q.append("\nответ студента:\n"+studAnswer+"\n\"");
    if (!checkIfNull.value(0).isNull()){
        q.append(")");
    }
    q.append("WHERE stud_id = "+QString::number(studId)+" AND pass_status = 11");
    QSqlQuery query(db);
    query.exec(q);

}

void dataBase::addTextNote(QVector<int> studAnswers, int id, int studId)
{
    QString q;
    QSqlQuery checkIfNull(db);
    checkIfNull.exec("SELECT text_note FROM exam_pass_status WHERE stud_id = "+QString::number(studId)+" AND pass_status = 11");
    checkIfNull.next();
    if (checkIfNull.value(0).isNull()){
        q = "UPDATE exam_pass_status SET text_note = \"";
    } else {
        q = "UPDATE exam_pass_status SET text_note = CONCAT(text_note,\"";
    }
    QVector<QVector<QString>> questions = getQuestions(-1,id); //тип, вопрос,...,id,diff,ответ
    q.append(questions[0][1]+"\nответ студента\n");
    QString studAnswer;
    foreach (int answer, studAnswers) {
        studAnswer.append(QString::number(answer+1)+" ;");
    }
    q.append(studAnswer+"\"");
    if (!checkIfNull.value(0).isNull()){
        q.append(")");
    }
    q.append("WHERE stud_id = "+QString::number(studId)+" AND pass_status = 11");
    QSqlQuery query(db);
    query.exec(q);
}

QString dataBase::getReport(int id)
{
    QSqlQuery query(db);
    query.prepare("SELECT text_note FROM exam_pass_status WHERE exam_pass_id = ?");
    query.addBindValue(id);
    query.exec();
    query.next();
    return query.value(0).toString();
}
QVector<QVector<QString> > dataBase::getQuestions(int id,int questionId)
{
    QSqlQuery query(db),subquery(db);
    QString idText = QString::number(id);
    idText.insert(0,"'%");
    idText.append("%'");
    QString q("SELECT q_type,q_text,subject_id,q_id,difficulty,q_answer,q_adv_data FROM questions WHERE subject_id LIKE (");
    q.append(idText+")");
    if (questionId != -1){
        q = "SELECT q_type,q_text,subject_id,q_id,difficulty,q_answer,q_adv_data FROM questions WHERE";
        q.append(" q_id = "+QString::number(questionId));
    }
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
    q.append("'"+question+"' , '"+answer+"' , "+QString::number(difficulty)+" )");
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
int dataBase::checkAnswer(int id, QVector<int> answers, QString *correctAnswer)
{
    QSqlQuery query(db);
    query.prepare("SELECT q_id,q_answer,difficulty,q_type FROM tester.questions WHERE q_id = ?");
    query.addBindValue(id);
    query.exec();
    query.next();
    int type = query.value(3).toInt();
    QString _correctAnswers = query.value(1).toString();
    if (correctAnswer != NULL)
        *correctAnswer = _correctAnswers;
    if (type == 1){

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

bool dataBase::checkUser(QString userName, QString password, int &userType)
{
    QSqlQuery query(db);
    query.prepare("SELECT login,password,user_type FROM tester.users WHERE login = ? AND password =?");
    query.addBindValue(userName);
    query.addBindValue(password);
    query.exec();
    if (query.next()){
        userType = query.value(2).toInt();
        return true;
    }
    return false;
}



int dataBase::checkAnswer(int id, QString answer, QString *correctAnswer)
{
    QSqlQuery query(db);
    query.prepare("SELECT q_id,q_answer,difficulty FROM tester.questions WHERE q_id = ?");
    query.addBindValue(id);
    query.exec();
    query.next();
    if (correctAnswer != NULL){
        *correctAnswer = query.value(1).toString();
    }
    if (!QString::compare(answer,correctAnswer,Qt::CaseInsensitive)){
        return query.value(2).toInt();
    }
    return 0;
}

int dataBase::getStudentCurrentExamState(int id, int &subject_id, int &question_select_type, QByteArray &question_list, int &exam_id)
{
    QSqlQuery query(db);
    query.prepare("SELECT stud_id,exam_id,pass_status FROM tester.exam_pass_status WHERE stud_id = ? AND pass_status = 11");
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
       QDateTime time_limit = QDateTime::currentDateTime();
       qint64 seconds = query.value(3).toInt();
       time_limit = time_limit.addSecs(seconds);
       query.prepare("UPDATE exam_pass_status SET time_limit = ? WHERE stud_id = ?");
       query.addBindValue(time_limit);
       query.addBindValue(id);
       query.exec();
       return pass_status;
    } else {
        return -1; //нет прохождения экзамена в exam_pass_status, в котором есть допуск
    }
    return -1;
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
void dataBase::addUser(QVector<QString> values){
    QSqlQuery query(db),query2(db);
     query.exec("SELECT MAX(teacher_id) FROM tester.users");
     query.next();
     int maxId = query.value(0).toInt() +1;

    QString queryString("INSERT INTO tester.users (first_name,middle_name,last_name, login ,password,user_type,teacher_id) VALUES ( ");
    for  (int i=0;i<6;i++){
       queryString+=(QString("'") + values[i] + QString("'"));
       queryString+= ", ";
    }
    queryString+=( QString::number(maxId));
    queryString+= ")";
    query2.exec(queryString);
}
void dataBase::deleteUser(int id){
    QSqlQuery query(db);
    query.prepare("DELETE FROM tester.users WHERE teacher_id = ?");
    query.addBindValue(id);
    query.exec();
}
void dataBase::resetPassword(int id,QString password){
    QSqlQuery query(db);
    query.prepare("UPDATE tester.users SET password = ? WHERE teacher_id = ?");
    query.addBindValue(password);
    query.addBindValue(id);
    query.exec();
}
QVector<QVector<QString> > dataBase::findUsers(QVector<int> params, QVector<QString> values)
{
    QSqlQuery query(db);
    QString queryString("SELECT first_name,middle_name,last_name, login ,user_type,teacher_id FROM tester.users WHERE ");
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
        case FILTER_LOGIN:
            where += QString("login LIKE '%") + values[i] + QString("%' "); ;
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
         query.prepare("SELECT first_name,middle_name,last_name, login ,user_type,teacher_id FROM tester.users");
        query.exec();
         //нет параметров фильтра
    }

    QVector<QVector<QString>> users;
    while(query.next()){
        QVector<QString> user;
        for (int i=0;i<6;i++){
        user.push_back(query.value(i).toString());
        }
        users.push_back(user);
    }
    return users;
}



