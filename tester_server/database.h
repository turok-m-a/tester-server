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
#include <QVector>
#include <QSet>
#include <iostream>
#include <QByteArray>
#include <QDataStream>
#include <constants.h>
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
    QVector<QString> getSubjects();
    int getUserId(QString name);
    bool checkUser(QString userName,QString password);
    int checkAnswer(int id,QVector<int> answers);
    int checkAnswer(int id,QString answer);
    int getStudentCurrentExamState(int id,int & subject_id,int & question_select_type,QByteArray & question_list,int & exam_id);
    int getMaxMark(int id);
    QByteArray getQuestionsForExam(QByteArray question_list);
    QVector<QVector <QString>> findStudents(QVector<int> params,QVector<QString> values);
    void addStudent(QVector<QString> values);
};

#endif // DATABASE_H
