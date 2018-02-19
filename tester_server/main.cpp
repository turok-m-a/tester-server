#include <QCoreApplication>
#include <core.h>
#ifndef linux
#include <windows.h>
#endif
using namespace std;
int main(int argc, char *argv[])
{
#ifndef linux
    //system("chcp 65001 > NULL");//перекл. консоль в utf-8
#endif
    QCoreApplication a(argc, argv);

    // Task parented to the application so that it
    // will be deleted by the application.
    core *task = new core(&a);

    // This will cause the application to exit when
    // the task signals finished.
    QObject::connect(task, SIGNAL(finished()), &a, SLOT(quit()));

    // This will run the task from the application event loop.
    QTimer::singleShot(0, task, SLOT(run()));

    return a.exec();
}
