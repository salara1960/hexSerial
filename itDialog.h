#pragma once

#include <QtWidgets/QDialog>
#include <QString>

class pwdDialog;

//-----------------------------------------------------------------
namespace Ui {
    class pwdDialog;
}
//-----------------------------------------------------------------

class pwdDialog : public QDialog
{
    Q_OBJECT
public:
    explicit pwdDialog(QWidget *parent = nullptr, QString = "", QString = "");
    ~pwdDialog() override;

signals:
    void DoneW(QString);

private:
    Ui::pwdDialog *uiw;
    QString buf;

public slots:
    void slotOk();

};

//-----------------------------------------------------------------
