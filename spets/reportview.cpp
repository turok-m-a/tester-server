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

//    loadTest a[100];
//    int pause = 1;
//    for (int i=0;i<100;i++){
//         a[i].pause = &pause;
//         a[i].opCode = VIEW_REPORT;
//         a[i].r = byteArray;
//         a[i].start();
//    }
//    qDebug() << "!";
//    pause = 0;
            reply = network.sendQuery(VIEW_REPORT,byteArray);


    //reply = network.sendQuery(VIEW_REPORT,byteArray);
    QDataStream replyStream(&reply, QIODevice::ReadOnly);
    QString report;
    replyStream >> report;
    ui->textBrowser->setText(report);
}

ReportView::~ReportView()
{
    delete ui;
}
