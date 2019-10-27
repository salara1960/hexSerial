#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settingsdialog.h"
#include "itDialog.h"



//******************************************************************************************************
//
//       hex Serial Terminal
//
//******************************************************************************************************


//const QString vers = "0.1";//10.10.2019
//const QString vers = "0.2";//11.10.2019
//const QString vers = "0.3";//12.10.2019
//const QString vers = "0.4";//14.10.2019
//const QString vers = "0.5";//16.10.2019
//const QString vers = "0.5.1";//17.10.2019
//const QString vers = "0.5.2";//17.10.2019
//const QString vers = "0.6";//18.10.2019
//const QString vers = "0.7";//24.10.2019
//const QString vers = "0.7.1";//25.10.2019
const QString vers = "0.7.2";//27.10.2019



const QString title = "hexSerialTerminal";

const QString main_pic    = "png/main.png";
const QString con_pic     = "png/conn.png";
const QString dis_pic     = "png/dis.png";
const QString salara_pic  = "png/salara.png";
const QString hide_pic    = "png/eyeHide.png";
const QString show_pic    = "png/eyeShow.png";
const QString close_pic   = "png/close.png";


//******************************************************************************************************

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), conf(new SettingsDialog)
{
    ui->setupUi(this);

    MyError = 0;

    if (!conf) {
        MyError |= 4;//create settings object error
        throw TheError(MyError);
    }

    this->setWindowIcon(QIcon(main_pic));

    this->setWindowOpacity(0.90);//set the level of transparency

    this->setTrayIconActions();
    this->showTrayIcon();

    first  = true;
    con = false;
    sdevName = "";
    sdev = nullptr;

    tmr_sec = startTimer(1000);// 1 sec.
    if (tmr_sec <= 0) {
        MyError |= 2;//start_timer error
        throw TheError(MyError);
    }

    connect(this, SIGNAL(sigWrite(QByteArray &)), this, SLOT(slotWrite(QByteArray &)));

    //Menu
    connect(ui->actionVERSION,    &QAction::triggered, this, &MainWindow::About);
    connect(ui->actionPORT,       &QAction::triggered, conf, &SettingsDialog::show);
    connect(ui->actionCONNECT,    &QAction::triggered, this, &MainWindow::on_connect_clicked);
    connect(ui->actionDISCONNECT, &QAction::triggered, this, &MainWindow::on_disconnect_clicked);
    connect(ui->actionCLEAR,      &QAction::triggered, this, &MainWindow::clrLog);


    ui->disconnect->setEnabled(false);
    ui->connect->setEnabled(true);

    ui->actionCONNECT->setEnabled(true);
    ui->actionPORT->setEnabled(true);
    ui->actionDISCONNECT->setEnabled(false);

    ui->status->clear();
    ui->pic->setPixmap(QPixmap(dis_pic));

    keyId = 0;
    keys = nullptr;
    keyAdr[keyId] = ui->ack; keyId++;
    keyAdr[keyId] = ui->nak; keyId++;
    keyAdr[keyId] = ui->enq; keyId++;
    keyAdr[keyId] = ui->eot;
    for (int i = 0; i < keyCnt; ++i) {
        keyArr[i].clear();
        keyArr[i].append(defKeys[i]);
        keyAdr[i]->setToolTip("0x" + QString::number(keyArr[i].at(0), 16));
    }
    //
    QByteArray stz; stz.append(defSendKeys, sendkeyCnt);
    ui->stx->setText(stz.toHex());
    ui->hexBox->setCheckState(Qt::Checked);
    ui->stx->setEnabled(false);
    ui->hexBox->setEnabled(false);
    //
    connect(this, &MainWindow::sigButtonData, this, &MainWindow::slotButtonData);

#ifdef SET_MOUSE_KEY
    connect(this, &MainWindow::sigRM, this, &MainWindow::slotRM);
#endif

}
//-----------------------------------------------------------------------
MainWindow::~MainWindow()
{
    deinitSerial();
    if (conf) delete conf;
    if (keys) delete keys;

    killTimer(tmr_sec);
    delete ui;
}
//--------------------------------------------------------------------------------
void MainWindow::KeyProg(QByteArray bt)
{
    if (keyId < keyCnt) {
        QByteArray tmp; tmp.clear();
        hexTobin(bt.data(), &tmp);
        if (tmp.length()) {
            keyArr[keyId] = tmp;
            keyAdr[keyId]->setToolTip(tmp.toHex(' '));
        }
    }
    if (keys) { delete keys; keys = nullptr; }
}
//--------------------------------------------------------------------------------
void MainWindow::slotButtonData()
{
    if (keyId < keyCnt) {
        QString from(keyName[keyId] + " key programming");

        if (keys) { delete keys; keys = nullptr; }
        QByteArray tmp; tmp.append(keyArr[keyId]);
        keys = new pwdDialog(nullptr, from, tmp);
        if (keys) {
            connect(keys, SIGNAL(DoneW(QByteArray)), this, SLOT(KeyProg(QByteArray)));
            keys->show();
        }
    }
}
//--------------------------------------------------------------------------------

MainWindow::TheError::TheError(int err) { code = err; }//error class descriptor

//--------------------------------------------------------------------------------
void MainWindow::clrLog()
{
    ui->log->clear();
}
//--------------------------------------------------------------------------------
int MainWindow::initSerial()
{
    deinitSerial();

    sdev = new QSerialPort(sdevName);
    if (sdev) {
        const SettingsDialog::Settings p = conf->settings();
        sdevName = p.name;   sdev->setPortName(sdevName);
        sdev->setBaudRate(p.baudRate);
        sdev->setDataBits(p.dataBits);
        sdev->setParity(p.parity);
        sdev->setFlowControl(p.flowControl);
        sdev->setStopBits(p.stopBits);

        if (!sdev->open(QIODevice::ReadWrite)) {
            delete sdev;
            sdev = nullptr;
            return 1;
        } else {
            rxData.clear();            
            while (!sdev->atEnd()) rxData.append(sdev->readAll());
            rxData.clear();
            txData.clear();
            hex = true;

            ui->log->append("\n");

            connect(sdev, &QSerialPort::readyRead, this, &MainWindow::ReadData);
            connect(sdev, &QSerialPort::errorOccurred, this, &MainWindow::slotError);

            return 0;
        }
    } else {
        MyError |= 1;//create serial_port_object error
        throw TheError(MyError);
    }

}
//--------------------------------------------------------------------------------
void MainWindow::deinitSerial()
{
    if (sdev) {
        if (sdev->isOpen()) sdev->close();
        sdev->disconnect();
        delete sdev;
        sdev = nullptr;
    }
}
//--------------------------------------------------------------------------------
void MainWindow::About()
{
    QString st ="\nSerial port util version " + vers + "\nused : Qt v.";
    st.append(QT_VERSION_STR);

    QMessageBox box;
    box.setStyleSheet("background-color: rgb(208, 208, 208);");
    box.setIconPixmap(QPixmap(salara_pic));
    box.setText(st);
    box.setWindowTitle("About");
    box.exec();
}
//-----------------------------------------------------------------------
void MainWindow::LogSave(const char *func, const QByteArray & st, bool rxd, bool pr)
{
    QString fw;
    if (pr) {
        time_t ict = QDateTime::currentDateTime().toTime_t();
        struct tm *ct = localtime(&ict);
        fw.sprintf("%02d.%02d.%02d %02d:%02d:%02d | ", ct->tm_mday, ct->tm_mon+1, ct->tm_year+1900, ct->tm_hour, ct->tm_min, ct->tm_sec);
    }
    if (func) {
        fw.append("[");
        fw.append(func);
        fw.append("] ");
    }
    int len = st.length();
    fw.append(" (" + QString::number(len, 10) + ")");
    if (rxd) fw.append("> ");
        else fw.append("< ");
    if (hex) fw.append(st.toHex(' '));
        else fw.append(st);

    ui->log->append(fw);//to log screen
}
//-----------------------------------------------------------------------
void MainWindow::on_connect_clicked()
{
    if (con) return;


    if (!initSerial()) {
        ui->status->clear();
        ui->status->setText(tr("Connected to %1 : %2 %3%4%5 FlowControl %6")
                          .arg(sdevName)
                          .arg(conf->settings().stringBaudRate)
                          .arg(conf->settings().stringDataBits)
                          .arg(conf->settings().stringParity.at(0))
                          .arg(conf->settings().stringStopBits)
                          .arg(conf->settings().stringFlowControl));
        ui->pic->setPixmap(QPixmap(con_pic));

        ui->connect->setEnabled(false);
        ui->disconnect->setEnabled(true);

        con = true;
        ui->actionCONNECT->setEnabled(false);
        ui->actionPORT->setEnabled(false);
        ui->actionDISCONNECT->setEnabled(true);

        ui->stx->setEnabled(true);
        ui->hexBox->setEnabled(true);
    } else {
        ui->status->clear();
        ui->status->setText("Serial port " + sdevName + " open ERROR");
        ui->pic->setPixmap(QPixmap(dis_pic));

        deinitSerial();
    }
}
//-----------------------------------------------------------------------
void MainWindow::on_disconnect_clicked()
{
    if (!con) return;

    deinitSerial();

    ui->pic->setPixmap(QPixmap(dis_pic));
    ui->status->clear();

    ui->connect->setEnabled(true);
    ui->actionPORT->setEnabled(true);
    ui->disconnect->setEnabled(false);

    con = false;
    ui->actionCONNECT->setEnabled(true);
    ui->actionDISCONNECT->setEnabled(false);

    ui->stx->setEnabled(false);
    ui->hexBox->setEnabled(false);
}
//-----------------------------------------------------------------------
void MainWindow::slotWrite(QByteArray & mas)
{
    if (!sdev) return;

    if (sdev->isOpen()) {
        if (hex) {
            sdev->write(mas);
        } else {
            QString m(mas);
            sdev->write(m.toLocal8Bit());
        }

        LogSave(nullptr, mas, false, true);
    }
}
//-----------------------------------------------------------------------
void MainWindow::on_ack_clicked()
{
    if (sdev) {
        QByteArray m(keyArr[KEY_ACK]);
        hex = true;
        emit sigWrite(m);
    } else {
        keyId = KEY_ACK;
        emit sigButtonData();
    }
}
//-----------------------------------------------------------------------
void MainWindow::on_nak_clicked()
{
    if (sdev) {
        QByteArray m(keyArr[KEY_NAK]);
        hex = true;
        emit sigWrite(m);
    } else {
        keyId = KEY_NAK;
        emit sigButtonData();
    }
}
//-----------------------------------------------------------------------
void MainWindow::on_enq_clicked()
{
    if (sdev) {
        QByteArray m(keyArr[KEY_ENQ]);
        hex = true;
        emit sigWrite(m);
    } else {
        keyId = KEY_ENQ;
        emit sigButtonData();
    }
}
//-----------------------------------------------------------------------
void MainWindow::on_eot_clicked()
{
    if (sdev) {
        QByteArray m(keyArr[KEY_EOT]);
        hex = true;
        emit sigWrite(m);
    } else {
        keyId = KEY_EOT;
        emit sigButtonData();
    }
}
//-----------------------------------------------------------------------
void MainWindow::on_answer_clicked()
{
//02 55 00 00 03 56
//02 55 00 a0 03 f6

    QByteArray sb(ui->stx->text().toLocal8Bit());
    QByteArray tmp; tmp.clear();

    if (ui->hexBox->checkState() == Qt::Checked) {
        hex = true;
        sb = sb.toUpper();
        hexTobin(sb.data(), &tmp);
    } else {
        hex = false;
        tmp = sb;
    }

    if (tmp.length()) emit sigWrite(tmp);

}
//------------------------------------------------------------------------------------
unsigned char MainWindow::myhextobin(const char *uk)
{
char line[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
unsigned char a = 0, b = 0, c = 0, i;


    for	(i = 0; i < 16; i++) { if (*uk == line[i]) { b = i; break; } else b = 0xff; }
    for	(i = 0; i < 16; i++) { if (*(uk + 1) == line[i]) { c = i; break; } else c = 0xff; }
    if ((b == 0xff) || (c == 0xff)) a = 0xff; else { b <<= 4;   a = b | c; }

    return a;
}
//-----------------------------------------------------------------------
void MainWindow::hexTobin(const char *in, QByteArray *out)
{
size_t i = 0, len = strlen(in) / 2;
const char *uk = in;

    while (i < len) {
        out->append( static_cast<char>(myhextobin(uk)) );
        i++; uk += 2;
    }
}
//-----------------------------------------------------------------------
void MainWindow::timerEvent(QTimerEvent *event)
{ 
    if (tmr_sec == event->timerId()) {
        time_t it_ct = QDateTime::currentDateTime().toTime_t();
        struct tm *ctimka = localtime(&it_ct);
        QString dt;
        dt.sprintf("%02d.%02d.%04d %02d:%02d:%02d",
                    ctimka->tm_mday,
                    ctimka->tm_mon + 1,
                    ctimka->tm_year + 1900,
                    ctimka->tm_hour,
                    ctimka->tm_min,
                    ctimka->tm_sec);
        setWindowTitle(title +" ver. " + vers + "  |  " + dt);
    }
}
//-----------------------------------------------------------------------
void MainWindow::ReadData()
{
    while (!sdev->atEnd()) rxData.append(sdev->read(1));

    if (rxData.length()) {
        hex = true;
        LogSave(nullptr, rxData, true, true);
        rxData.clear();
    }

}
//-----------------------------------------------------------------------
void MainWindow::slotError(QSerialPort::SerialPortError serialPortError)
{
    if (serialPortError == QSerialPort::ReadError) {
        MyError |= 8;//error reading from serial port
        throw TheError(MyError);
    }
}

//**************************************************************************************
//                            Tray
//**************************************************************************************
void MainWindow::showTrayIcon()
{
    trayIcon = new QSystemTrayIcon(this);
    QIcon trayImage(main_pic);
    trayIcon->setIcon(trayImage);
    trayIcon->setContextMenu(trayIconMenu);

    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
                this, SLOT(trayIconActivated(QSystemTrayIcon::ActivationReason)));

    trayIcon->show();
}
//-------------------------------------------------------------------------------------
void MainWindow::trayActionExecute()
{
    //QMessageBox::information(this, "TrayIcon", "Тестовое сообщение. Замените вызов этого сообщения своим кодом.");
}
//-------------------------------------------------------------------------------------
void MainWindow::trayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (static_cast<int>(reason)) {
        case QSystemTrayIcon::Trigger:
        case QSystemTrayIcon::DoubleClick:
            this->showNormal();
        break;
    }
}
//-------------------------------------------------------------------------------------
void MainWindow::setTrayIconActions()
{
    minA  = new QAction(QIcon(hide_pic), "Hide", this);
    maxA  = new QAction(QIcon(show_pic), "Show", this);
    quitA = new QAction(QIcon(close_pic),"Quit", this);


    connect(minA, SIGNAL(triggered()),  this, SLOT(hide()));
    connect(maxA, SIGNAL(triggered()),  this, SLOT(showNormal()));
    connect(quitA, SIGNAL(triggered()), qApp, SLOT(quit()));

    trayIconMenu = new QMenu(this);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(minA);
    trayIconMenu->addAction(maxA);
    trayIconMenu->addAction(quitA);
    //trayIconMenu->setStyleSheet(QString::fromUtf8("background-color: rgb(100, 100, 100);"));
}
//-------------------------------------------------------------------------------------
void MainWindow::changeEvent(QEvent *event)
{
    QMainWindow::changeEvent(event);
    if (event->type() == QEvent::WindowStateChange) {
        if (isMinimized()) this->hide();
    }
}
//-------------------------------------------------------------------------------------
void MainWindow::closeEvent(QCloseEvent *evt)
{
    if (trayIcon->isVisible()) {
        this->hide();
        evt->ignore();
    }
}
//**************************************************************************************

#ifdef SET_MOUSE_KEY
//-------------------------------------------------------------------------------------
void MainWindow::slotRM(int x, int y)
{
    QMessageBox::information(this, "Mouse key", "\nX,Y = " + QString::number(x, 10) + "," + QString::number(y, 10) + "\n");
}
//-------------------------------------------------------------------------------------
void MainWindow::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::RightButton) {
        e->accept();
        //emit sigRM(e->pos().x(), e->pos().y());
        QMessageBox::information(this, "Right key", "\nX,Y = " + QString::number(e->pos().x(), 10) + "," + QString::number(e->pos().y(), 10) + "\n");
    } else if (e->button() == Qt::MiddleButton) {
        e->accept();
        QMessageBox::information(this, "Middle key", "\nX,Y = " + QString::number(e->pos().x(), 10) + "," + QString::number(e->pos().y(), 10) + "\n");
        //emit sigRM(e->pos().x(), e->pos().y());
    }
}
#endif



