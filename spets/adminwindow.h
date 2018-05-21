#ifndef ADMINWINDOW_H
#define ADMINWINDOW_H

#include <QDialog>
#include <constants.h>
#include <network.h>
#include <infowindow.h>
namespace Ui {
class AdminWindow;
}

class AdminWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AdminWindow(QWidget *parent = 0);
    ~AdminWindow();

private slots:
    void on_addUser_clicked();

    void on_delUser_clicked();

    void on_findUser_clicked();

    void on_resetPassword_clicked();

private:
    Ui::AdminWindow *ui;
};

#endif // ADMINWINDOW_H
