#include "testwindow.h"
#include "ui_testwindow.h"

TestWindow::TestWindow(QWidget *parent): QDialog(parent),  tui(new Ui::TestWindow)
{
   tui->setupUi(this);
}

void TestWindow::showWindow()
{
   this->show();
}
