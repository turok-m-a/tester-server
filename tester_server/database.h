#ifndef DATABASE_H
#define DATABASE_H
#include "states.h"
#include <QtSql/qsqldatabase.h>
#include <QtSql/qsqldriverplugin.h>
#include <QtSql/QSqlQuery>
#include <QVariant>
#include <QObject>
#include <QSqlError>
#include <QString>
#include <iostream>
#include <vector>
#include <QByteArray>
#include <QDataStream>
using namespace std;
class dataBase
{
private:

    dataBase();
    dataBase( const dataBase&);
    dataBase& operator=(dataBase& );
    QSqlDatabase db;
public:
   //dataBase();
    static dataBase& getInstance() {
           static dataBase  instance;
           return instance;
       }
    void addSubject(QString name);
    vector<QString> getSubjects();
    int getUserId(QString name);
    int getStudentCurrentExamState(int id,int & subject_id,int & question_select_type,QByteArray & question_list,int & exam_id);
    QByteArray getQuestionsForExam(QByteArray question_list);
};

#endif // DATABASE_H
