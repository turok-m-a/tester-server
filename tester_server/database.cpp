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
    QSqlQuery query(db);
    query.exec("SELECT * FROM subjects");
    while (query.next()) {
         QString name = QString::fromUtf8(query.value(0).toByteArray());
         int salary = query.value(1).toInt();
         string a = name.toStdString();
         cout <<a << salary;
       }
}


void dataBase::addSubject(QString name)
{
    QSqlQuery query(db);
    QSqlQuery q2(db);
    q2.exec("SELECT max(subject_id) FROM tester.subjects");
    q2.next();
    int maxId = q2.value(0).toInt();

     query.prepare("INSERT INTO subjects (name, subject_id) VALUES (?, ?)");
     query.addBindValue(name);
     query.addBindValue(maxId+1);
     query.exec();
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
       query.prepare("SELECT subject_id,question_select_type,question_list FROM tester.exams WHERE exam_id = ? ");
       query.addBindValue(exam_id);
       query.exec();
       query.next();
       subject_id = query.value(0).toInt();
       question_select_type = query.value(1).toInt();
       question_list = query.value(2).toByteArray();
       int debug = question_list.size();
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
    if (! query.value(3).isNull()) {    //3 и 4 тип содержат доп. данные
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
    query.prepare("SELECT first_name,middle_name,last_name,group,stud_document_id,stud_id FROM tester.students "
                "WHERE ? ");
    QString where;
    for (int i=0;i<params.size();i++){
        switch (params[i]) {
        case FILTER_FIRST_NAME:
            where += QString(" first_name LIKE '%") + values[i] + QString("%' "); ;
            break;
        case FILTER_MIDDLE_NAME:
            where += QString(" middle_name LIKE '%") + values[i] + QString("%' "); ;
            break;
        case FILTER_LAST_NAME:
            where += QString(" last_name LIKE '%") + values[i] + QString("%' "); ;
            break;
        case FILTER_STUD_GROUP:
            where += QString(" group LIKE '%") + values[i] + QString("%' "); ;
            break;
        case FILTER_STUD_TICKET:
            where += QString(" stud_document_id LIKE '%") + values[i] + QString("%' "); ;
            break;
        default:
            break;
        }
        if (i < (params.size()-1)){
            where += QString(" AND ");
        }
    }
    query.addBindValue(where);
    query.exec();
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



