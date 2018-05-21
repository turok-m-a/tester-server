#include "infowindow.h"
#include "ui_infowindow.h"

infowindow::infowindow(QWidget *parent, QString text) :
    QDialog(parent),
    ui(new Ui::infowindow)
{
    ui->setupUi(this);
    if(text != ""){
        ui->infolabel->setText(text);
    }
    this->setAttribute(Qt::WA_DeleteOnClose);
}

infowindow::~infowindow()
{
    delete ui;
}

void infowindow::setMessage(QString msg)
{
    ui->infolabel->setText(msg);
}

void infowindow::on_pushButton_clicked()
{
    this->close();
}
