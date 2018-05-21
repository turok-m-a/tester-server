#ifndef REPORTVIEW_H
#define REPORTVIEW_H

#include <QDialog>
#include <constants.h>
#include <network.h>
#include <QDebug>
#include <loadtest.h>
namespace Ui {
class ReportView;
}

class ReportView : public QDialog
{
    Q_OBJECT

public:
    explicit ReportView(const QString & id, QWidget *parent = 0);
    ~ReportView();

private:
    Ui::ReportView *ui;
};

#endif // REPORTVIEW_H
