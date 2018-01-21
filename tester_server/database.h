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
using namespace std;
class dataBase
{
private:

    dataBase();
    dataBase( const Singleton&);
    dataBase& operator=(dataBase& );
    QSqlDatabase db;
public:
   //dataBase();
    static dataBase& getInstance() {
           static Singleton  instance;
           return instance;
       }
    void addSubject(QString name);
    vector<QString> getSubjects();
    int getUserId(QString name);
    userState gSetStudentState(int id);
};

#endif // DATABASE_H
