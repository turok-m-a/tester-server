#include "reportview.h"
#include "ui_reportview.h"

ReportView::ReportView(const QString & id,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReportView)
{
    ui->setupUi(this);
    QByteArray byteArray,reply;
    QDataStream stream(&byteArray, QIODevice::WriteOnly);
    stream << id;
    Network & network = Network::getInstance();
    reply = network.sendQuery(VIEW_REPORT,byteArray);
    QDataStream replyStream(&reply, QIODevice::ReadOnly);
    QString report;
    replyStream >> report;
    ui->textBrowser->setText(report);
}

ReportView::~ReportView()
{
    delete ui;
}
