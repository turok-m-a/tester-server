#ifndef TESTWINDOW_H
#define TESTWINDOW_H

#include <QDialog>
namespace  Ui {
    class TestWindow;
}
class TestWindow : public QDialog

{
    Q_OBJECT
public:
    explicit TestWindow(QWidget *parent = 0);
    void showWindow();
private slots:
  //  void showWindow();
private:
    Ui::TestWindow * tui;
};

#endif // TESTWINDOW_H
