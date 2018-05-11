#include "mainwindow.h"
#include <QApplication>
using namespace std;
int main(int argc, char *argv[])
{
//    QStringList paths = QCoreApplication::libraryPaths();
//    paths.append("platforms");
//    paths.append(".");
//    QCoreApplication::setLibraryPaths();
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
