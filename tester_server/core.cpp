#include <core.h>
#include <database.h>
void core::run()
{
   // qDebug() << QSqlDatabase::drivers();
    dataBase db;
    db.addSubject("АИЛОВТ");
    emit finished();
}


