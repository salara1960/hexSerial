#include "itDialog.h"
#include "ui_itDialog.h"

//**************************************************************************************
pwdDialog::pwdDialog(QWidget *parent, QString title, QByteArray arr) : QDialog(parent), uiw(new Ui::pwdDialog)
{
    uiw->setupUi(this);

    this->setWindowIcon(QIcon("png/button.png"));
    this->setFixedSize(this->size());

    setWindowTitle(title);

    buf.clear();
    uiw->line->setText(arr.toHex(' '));

    connect(uiw->but_ok, SIGNAL(pressed()), this, SLOT(slotOk()));

}
//-----------------------------------------------
pwdDialog::~pwdDialog()
{
    this->disconnect();
    delete uiw;
}
//-----------------------------------------------
void pwdDialog::slotOk()
{
    buf.append(uiw->line->text().toLocal8Bit());

    emit DoneW(buf.toUpper());
}
//**************************************************************************************

