#ifndef STUDCONTROLWINDOW_H
#define STUDCONTROLWINDOW_H

#include <QDialog>
#include <constants.h>
#include <network.h>
#include <infowindow.h>
namespace Ui {
class StudControlWindow;
}

class StudControlWindow : public QDialog
{
    Q_OBJECT

public:
    explicit StudControlWindow(QWidget *parent = 0);
    ~StudControlWindow();

private slots:
    void on_findButton_clicked();

    void on_addButton_clicked();

private:
    Ui::StudControlWindow *ui;
};

#endif // STUDCONTROLWINDOW_H
