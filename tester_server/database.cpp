#include "database.h"

dataBase::dataBase()
{
    //if (instance) return;
    //instance = true;
    ///////
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

vector<QString> dataBase::getSubjects()
{
    QSqlQuery query(db);
    query.exec("SELECT name FROM subjects");
    vector<QString> ret;
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

int dataBase::getStudentCurrentExamState(int id, int &subject_id, int &question_select_type, QByteArray &question_list, int &exam_id)
{
    QSqlQuery query(db);
    query.prepare("SELECT stud_id,exam_id,pass_status FROM tester.exam_pass_status WHERE stud_id = ?");
    //ищем строку с статусом экзамена, к которому в данный момент получен допуск (такой должен быть 1)
    //препод добавляет
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
    //questions.append((char*)&q_number,4);
    stream << query.value(0).toInt() << query.value(1).toInt();
    //questions.append((char*)&(query.value(0).toInt()),4);//q_id
    //questions.append((char*)&(query.value(1).toInt(),4);//q_type
    QByteArray q_text = query.value(2).toByteArray();
    int q_textlen = query.value(2).toByteArray().size();
    //questions.append(answerlen);
    //questions.append(answer);
    stream << q_textlen << q_text;
    QByteArray adv_data = query.value(3).toByteArray();
    int adv_data_len = query.value(3).toByteArray().size();
    //questions.append(adv_data_len);
    //questions.append(adv_data);
    stream << adv_data_len << adv_data;
    }
    return byteArray;
}



