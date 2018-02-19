#ifndef STUDCONTROLWINDOW_H
#define STUDCONTROLWINDOW_H

#include <QDialog>

namespace Ui {
class StudControlWindow;
}

class StudControlWindow : public QDialog
{
    Q_OBJECT

public:
    explicit StudControlWindow(QWidget *parent = 0);
    ~StudControlWindow();

private:
    Ui::StudControlWindow *ui;
};

#endif // STUDCONTROLWINDOW_H
