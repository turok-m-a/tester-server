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
#include <QChar>
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
    void delSubject(int id);
    QVector<QVector<QString> > findSubject(QString name);
    QVector<QVector<QString> > getQuestions(int id);
    void removeQuestion(int id);
    void editQuestionSubjects(int questionId,int editOperationType,int subjId);
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
    void removeStudent(int id);
    QString textQuestionFormat(QString questionText, int type, QString answer);
    void addOnesToAnswerString(QString & answer);
};

#endif // DATABASE_H
