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
#include <QDate>
using namespace std;
class dataBase
{
private:

    dataBase();
    dataBase( const dataBase&);
    dataBase& operator=(dataBase& );
    QSqlDatabase db;
public:
    static dataBase& getInstance() {
           static dataBase  instance;
           return instance;
       }
    void addSubject(QString name);
    void delSubject(int id);
    QVector<QVector<QString> > findSubject(QString name);
    QVector<QVector<QString> > getQuestions(int id=-1, int questionId = -1);
    void removeQuestion(int id);
    void editQuestionSubjects(int questionId,int editOperationType,int subjId);
    void addQuestion(int type,int subjId,QString question,QString answer,QByteArray advData,int difficulty);
    void addExam(int subject,QDate date,QByteArray questionList);
    QVector<QString> getSubjects();
    int getUserId(QString name);
    bool checkUser(QString userName,QString password);
    int checkAnswer(int id,QVector<int> answers,QString * correctAnswer = NULL);
    int checkAnswer(int id,QString answer,QString * correctAnswer = NULL);
    int getStudentCurrentExamState(int id,int & subject_id,int & question_select_type,QByteArray & question_list,int & exam_id);
    int getMaxMark(int id);
    QByteArray getQuestionsForExam(QByteArray question_list);
    QVector<QVector <QString>> findStudents(QVector<int> params,QVector<QString> values);
    void addStudent(QVector<QString> values);
    void removeStudent(int id);
    QString textQuestionFormat(QString questionText, int type, QString answer);
    void addOnesToAnswerString(QString & answer);
    void setExamTime(int time, int id, bool &alreadySet);
    void deleteExam(int id);
    void startExamForStudent(QVector<int> studIds, int examId,QVector<int> & passIds);
    bool timeLimitExceed(int studId);
    QVector<QVector <QString>> getExamList(int subjId);
    QVector<float> getStudentPassStatus(QVector<int> passIds,int examId = -1);
    void setStudentMark(int studId,float mark);
    QVector<QVector <QString>> getExamStudList(int examId);
    int getExamTime(int exam_id);
    void addTextNote(QString studAnswer,int id,int studId);
    void addTextNote(QVector<int> studAnswers,int id,int studId);
    QString getReport(int id);
};

#endif // DATABASE_H
