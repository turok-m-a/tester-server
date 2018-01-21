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
     return -1;
}

userState dataBase::getStudentState(int id)
{
    QSqlQuery query(db);
    query.prepare("SELECT stud_id FROM tester.exam_pass_status WHERE stud_document_id = ?");
    query.addBindValue(name);
    query.exec();
    if (query.next()){
       return query.value(0).toInt();
    }
    return -1;
}



