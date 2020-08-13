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
//const QString vers = "0.7.2";//27.10.2019
//const QString vers = "0.8";//29.10.2019
//const QString vers = "0.9";//08.03.2020
//const QString vers = "1.0";//08.03.2020
//const QString vers = "1.1";//18.06.2020
//const QString vers = "1.2";//10.07.2020
//const QString vers = "1.3";//07.08.2020
//const QString vers = "1.4";//09.08.2020  !!! +++ !!!
//const QString vers = "1.5";//10.08.2020
const QString vers = "1.6";//13.08.2020



const QString title = "SerialTerminal";

const QString main_pic    = "png/main.png";
const QString con_pic     = "png/conn.png";
const QString dis_pic     = "png/dis.png";
const QString salara_pic  = "png/salara.png";
const QString hide_pic    = "png/eyeHide.png";
const QString show_pic    = "png/eyeShow.png";
const QString close_pic   = "png/close.png";

const QByteArray cr_lf = "\r\n";




//  Макрос для промежуточного подсчета контрольной суммы CRC32
#define CRC32(crc, ch) ((crc >> 8) ^ crc32_tab[(crc ^ (ch)) & 0xff])

//-----------------------------------------------------------------------
// Таблица для подсчета контрольной суммы CRC32 (polynomial 0xedb88320)
//
//CRC-32-IEEE 802.3  (x^{32} + x^{26} + x^{23} + x^{22} + x^{16} + x^{12} + x^{11} + x^{10} + x^8 + x^7 + x^5 + x^4 + x^2 + x + 1)
//
static const uint32_t crc32_tab[256] = {
0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f, 0xe963a535, 0x9e6495a3,
0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988, 0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91,
0x1db71064, 0x6ab020f2, 0xf3b97148, 0x84be41de, 0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec, 0x14015c4f, 0x63066cd9, 0xfa0f3d63, 0x8d080df5,
0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172, 0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b,
0x35b5a8fa, 0x42b2986c, 0xdbbbc9d6, 0xacbcf940, 0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423, 0xcfba9599, 0xb8bda50f,
0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924, 0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d,
0x76dc4190, 0x01db7106, 0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x086d3d2d, 0x91646c97, 0xe6635c01,
0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e, 0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457,
0x65b0d9c6, 0x12b7e950, 0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2, 0x4adfa541, 0x3dd895d7, 0xa4d1c46d, 0xd3d6f4fb,
0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0, 0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9,
0x5005713c, 0x270241aa, 0xbe0b1010, 0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81, 0xb7bd5c3b, 0xc0ba6cad,
0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a, 0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683,
0xe3630b12, 0x94643b84, 0x0d6d6a3e, 0x7a6a5aa8, 0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb, 0x196c3671, 0x6e6b06e7,
0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc, 0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5,
0xd6d6a3e8, 0xa1d1937e, 0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55, 0x316e8eef, 0x4669be79,
0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236, 0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f,
0xc5ba3bbe, 0xb2bd0b28, 0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a, 0x9c0906a9, 0xeb0e363f, 0x72076785, 0x05005713,
0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38, 0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21,
0x86d3d2d4, 0xf1d4e242, 0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 0x8f659eff, 0xf862ae69, 0x616bffd3, 0x166ccf45,
0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2, 0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db,
0xaed16a4a, 0xd9d65adc, 0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693, 0x54de5729, 0x23d967bf,
0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94, 0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d
};




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

    /*
    this->setTrayIconActions();
    this->showTrayIcon();
    */

    first  = true;
    con = false;
    sdevName = "";
    sdev = nullptr;
    crcFile = 0;
    fileSize = 0;
    pTmp = nullptr;
    Tik = 0;
    ms10 = 0;
    txData.clear();
    rxData.clear();

    //-----   for cmds  -------

    cmd_list.clear();
    chap.clear();
    apiBuf.data = nullptr;
    tmr_cmd = 0;
    getrec = goCmd = false;
    apiSnd = 0;
    cmd = noneCmd;

    //-------------------------

    tmr_sec = startTimer(10);// 10 msec.
    if (tmr_sec <= 0) {
        MyError |= 2;//start_timer error
        throw TheError(MyError);
    }

    connect(this, SIGNAL(sigWrite(QByteArray &)), this, SLOT(slotWrite(QByteArray &)));

    //Menu
    connect(ui->actionVERSION,    &QAction::triggered, this, &MainWindow::About);
    connect(ui->actionPORT,       &QAction::triggered, conf, &SettingsDialog::show);
    connect(ui->actionCONNECT,    &QAction::triggered, this, &MainWindow::on_connect);
    connect(ui->actionDISCONNECT, &QAction::triggered, this, &MainWindow::on_disconnect);
    connect(ui->actionCLEAR,      &QAction::triggered, this, &MainWindow::clrLog);
    connect(ui->actionGetApi,     &QAction::triggered, this, &MainWindow::getApi);
    connect(ui->actionProgApi,    &QAction::triggered, this, &MainWindow::progApi);
    connect(ui->actionCompApi,    &QAction::triggered, this, &MainWindow::compApi);
    connect(ui->actionCRC32,      &QAction::triggered, this, &MainWindow::crc32File);
    ui->actionVERSION->setToolTip("About " + title);
    ui->actionPORT->setToolTip("configure serial port");
    ui->actionCLEAR->setToolTip("clear Log window");
    ui->actionGetApi->setToolTip("get API");
    ui->actionProgApi->setToolTip("prog API");
    ui->actionCompApi->setToolTip("compare API");
    ui->actionCRC32->setToolTip("calc CRC32");

    connect(this, &MainWindow::sigConn, this, &MainWindow::on_connect);
    connect(this, &MainWindow::sigDisc, this, &MainWindow::on_disconnect);

    ui->actionCONNECT->setEnabled(true);
    ui->actionPORT->setEnabled(true);
    ui->actionDISCONNECT->setEnabled(false);

    ui->status->clear();

    keyId = 0;
    keys = nullptr;
    keyAdr[keyId] = ui->ack; keyId++;
    keyAdr[keyId] = ui->nak; keyId++;
    keyAdr[keyId] = ui->enq; keyId++;
    keyAdr[keyId] = ui->eot; keyId++;
    keyAdr[keyId] = ui->any; keyId++;
    keyAdr[keyId] = ui->info; keyId++;
    keyAdr[keyId] = ui->deep; keyId++;
    keyAdr[keyId] = ui->crc;  keyId++;
    keyAdr[keyId] = ui->one;
    for (int i = 0; i < keyCnt; ++i) {
        keyArr[i].clear();
        keyArr[i].append(keyData[i]);
        keyAdr[i]->setText(keyName[i]);
        QString tp(keyData[i]);
        if (tp.indexOf("\r\n", 0) != -1) tp.truncate(tp.length() - 2);
        keyAdr[i]->setToolTip(tp);
        //keyAdr[i]->setEnabled(false);
        connect(keyAdr[i], &QPushButton::clicked, this, &MainWindow::keyPrs);
    }
    //
    ui->stx->setText("#");
    ui->stx->setEnabled(false);
    ui->crlfBox->setCheckState(Qt::Checked);
    ui->crlfBox->setEnabled(false);
    ui->log->setEnabled(false);
    //
    connect(this, &MainWindow::sigButtonData, this, &MainWindow::slotButtonData);
    connect(this, &MainWindow::sig_mkList, this, &MainWindow::slot_mkList);
    connect(this, &MainWindow::sig_goProc, this, &MainWindow::slot_goProc);
    connect(this, &MainWindow::sig_Ready, this, &MainWindow::slot_Ready);
    connect(this, &MainWindow::sig_timeOutAck, this, &MainWindow::slot_timeOutAck);


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
    if (pTmp) free(pTmp);

    if (apiBuf.data) delete apiBuf.data;

    killTimer(tmr_sec);
    delete ui;
}
//--------------------------------------------------------------------------------
void MainWindow::KeyProg(QString str)
{
    if (keyId < keyCnt) {
        if (str.length()) {
            if (keyAdr[keyId]->text() != str) {
                keyArr[keyId].clear();
                keyArr[keyId].append(str + cr_lf);
                keyAdr[keyId]->setText(str);
                keyAdr[keyId]->setToolTip(str);
            }
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
        QString tp;
        tp.append(keyArr[keyId]);
        if (tp.indexOf("\r\n", 0) != -1) tp.truncate(tp.length() - 2);
        keys = new pwdDialog(nullptr, from, tp);
        if (keys) {
            connect(keys, SIGNAL(DoneW(QString)), this, SLOT(KeyProg(QString)));
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
//   Функция возвращает контрольную сумму CRC32 массива байт
//
uint32_t MainWindow::crc32(const uint32_t crc_origin, const uint8_t *buf, const uint32_t size)
{
uint32_t crc = ~crc_origin;
const uint8_t *p = buf;

    if (!buf || !size) return ~crc;

    for (uint32_t i = 0; i < size; i++) crc = CRC32(crc, *p++);

    return ~crc;

}
//--------------------------------------------------------------------------------
void MainWindow::getFile()
{
    QString *nm = new QString();
    *nm = QFileDialog::getOpenFileName(this, tr("Open file"), nullptr, tr("Files (*.*)"));
    if (!nm->size()) {
        ui->status->clear();
        ui->status->setText("No files selected");
        delete nm;
        return;
    }

    QFile fil(*nm);
    fileSize = 0;
    bool flag = fil.open(QIODevice::ReadOnly);
    if (flag) {
        fileSize = fil.size();
        if (fileSize > 0) {
            if (pTmp) { free(pTmp); pTmp = nullptr; }
            pTmp = (uint8_t *)calloc(1, fileSize);
            qint64 dl = fil.read((char *)pTmp, fileSize);
            if (dl != fileSize) {
                ui->status->clear();
                ui->status->setText("Error readinf file " + *nm);
            } else {
                crcFile = crc32(0, pTmp, fileSize);
                //ui->status->clear();
                //ui->status->setText("File " + *nm + " Size:" + QString::number(fileSize, 10) + ", CRC:0x" + QString::number(crcFile, 16).toUpper());
                QMessageBox::information(this, "calc CRC32",
                                         "File : " + *nm + cr_lf +
                                         "Size: " + QString::number(fileSize, 10) + " bytes" + cr_lf +
                                         "CRC32: 0x" + QString::number(crcFile, 16).toUpper() + cr_lf);
            }
        } else {
            ui->status->clear();
            ui->status->setText("File " + *nm + " is empty !");
        }
        fil.close();
    } else {
        ui->status->clear();
        ui->status->setText("Error open file " + *nm);
    }
    delete nm;
    nm = nullptr;

}
//--------------------------------------------------------------------------------
int MainWindow::initSerial()
{
    //deinitSerial();

    sdev = new QSerialPort(sdevName);
    if (sdev) {
        SettingsDialog::Settings p = conf->settings();
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
            txData.clear();
            while (!sdev->atEnd()) rxData = sdev->readAll();
            rxData.clear();
            txData.clear();

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
        rxData.clear();
        txData.clear();
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
void MainWindow::LogSave(const char *func, QByteArray & st, bool pr)
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
    fw.append(st);

    ui->log->append(fw);//to log screen
}
//-----------------------------------------------------------------------
void MainWindow::LogSave(const char *func, const QString & st, bool pr)
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
    fw.append(st);

    ui->log->append(fw);//to log screen
}
//-----------------------------------------------------------------------
void MainWindow::on_connect()
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

        con = true;
        ui->actionCONNECT->setEnabled(false);
        ui->actionPORT->setEnabled(false);
        ui->actionDISCONNECT->setEnabled(true);

        //for (int i = 0; i < keyCnt; i++) keyAdr[i]->setEnabled(true);

        ui->stx->setEnabled(true);
        ui->crlfBox->setEnabled(true);
        ui->log->setEnabled(true);

    } else {
        ui->status->clear();
        ui->status->setText("Serial port " + sdevName + " open ERROR");

        deinitSerial();
    }
}
//-----------------------------------------------------------------------
void MainWindow::on_disconnect()
{
    if (!con) return;

    deinitSerial();

    ui->status->clear();
    ui->status->setText("Disconnect from serial port " + sdevName);

    ui->actionPORT->setEnabled(true);

    con = false;
    ui->actionCONNECT->setEnabled(true);
    ui->actionDISCONNECT->setEnabled(false);

    ui->stx->setEnabled(false);
    ui->crlfBox->setEnabled(false);
    ui->log->setEnabled(false);

    //for (int i = 0; i < keyCnt; i++) keyAdr[i]->setEnabled(false);

}
//-----------------------------------------------------------------------
void MainWindow::slotWrite(QByteArray & mas)
{
    if (!sdev) return;

    if (sdev->isOpen()) {
        QString m(mas);
        sdev->write(m.toLocal8Bit());
        LogSave(nullptr, mas, false);
    }
}
//-----------------------------------------------------------------------
void MainWindow::on_answer_clicked()
{
//02 55 00 00 03 56
//02 55 00 a0 03 f6

    QByteArray sb(ui->stx->text().toLocal8Bit());
    QByteArray tmp(sb);

    if (ui->crlfBox->checkState() == Qt::Checked) tmp.append(cr_lf);
    if (tmp.length()) emit sigWrite(tmp);

}
//-----------------------------------------------------------------------
void MainWindow::keyPrs()
{
    int key = -1;
    for (int i = 0; i < keyCnt; i++) {
        if (sender() == keyAdr[i]) {
            key = i;
            break;
        }
    }
    if (key != -1) {
        if (sdev) {
            cmd = noneCmd;
            QByteArray m; m.append(keyArr[key]);
            emit sigWrite(m);
        } else {
            keyId = key;
            emit sigButtonData();
        }
    }
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
        ms10++;
        if (!(ms10 % 100)) {
            Tik++;
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
        if (wait_ack) {
            if (check_tmr(wait_ack)) {
                wait_ack = 0;
                goCmd = false;
                emit sig_timeOutAck();
            }
        }
    }
}
//-----------------------------------------------------------------------
int MainWindow::chkDone(QByteArray *buf)
{
int ret = -1;

    if (!goCmd) {
        ret = buf->indexOf("\r\n>", 0);
        if (ret != -1) {
            return ret + 3;
        } else {
            ret = buf->indexOf("\r\n", 0);
            if (ret != -1) {
               return ret + 2;
            } else {
                ret = buf->indexOf(">", 0);
                if (ret != -1) return ++ret;
            }
        }
    } else {
        if (buf->length() == ackLen) {
            ret = 0;
        } else {
            switch (cmd) {
                case binCmd:
                    if (buf->indexOf("BIN\r\n>", 0) != -1) {
                        ret = 0;
                        rec.err = rec.cnt = 0;
                    }
                break;
                case stopCmd:
                    if ( (buf->indexOf("ERROR!\r\n>", 0) != -1) ||
                         (buf->indexOf("BAD STRING.\r\n>", 0) != -1) ||
                            (buf->indexOf("\r\n>", 0) != -1) ) {
                        ret = 0;
                        rec.err = rec.cnt = 0;
                    }
                break;
                case startCmd:
                    if (buf->indexOf("m1\r\n", 0) != -1) {
                        ret = 0;
                        rec.err = rec.cnt = 0;
                    }
                break;
            }
        }
    }

    return ret;
}
//-----------------------------------------------------------------------
void MainWindow::ReadData()
{
int ix = -1;

    while (!sdev->atEnd()) {
        rxData += sdev->readAll();
        ix = chkDone(&rxData);
        if (ix != -1) break;
    }
    if (ix != -1) {
        if (cmd >= stopCmd) {
            int pos = 0;
            QByteArray dat, line = rxData.mid(0, ix);
            if ((pos = line.indexOf("\r\n>", 0)) == -1) {
                if ((pos = line.indexOf("\r\n", 0)) != -1) line.remove(pos, 2);
            }
            LogSave(nullptr, line, false);
            if (rxData.length() > (ix + 1)) rxData.remove(0, ix);
                                       else rxData.clear();
        //} else {
            if (cmd != noneCmd) {
                memcpy(ibuff, rxData.data(), rxData.length());
                LogSave(nullptr, rxData, false);
                rxData.clear();
                emit sig_Ready();
            }
        }
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

//**************************************************************************************
//                            Tray
//**************************************************************************************
/*
void MainWindow::showTrayIcon()
{
    trayIcon = new QSystemTrayIcon(this);
    QIcon trayImage(main_pic);
    trayIcon->setIcon(trayImage);
    trayIcon->setContextMenu(trayIconMenu);

    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
                this, SLOT(trayIconActivated(QSystemTrayIcon::ActivationReason)));

    trayIcon->show();
    trayIcon->setToolTip("hexSerial");
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
*/
//**************************************************************************************
//-----------------------------------------------------------------------
//  Функции для установки временных интервалов , а также их проверки
//
uint32_t MainWindow::get10ms()
{
    return ms10;
}
//
uint32_t MainWindow::get_tmr(uint32_t tm)
{
    return (get10ms() + tm);
}
//
int MainWindow::check_tmr(uint32_t tm)
{
    return (get10ms() >= tm ? 1 : 0);
}
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//  Функция записывает заданное количество байт в файл (в асинхронном режиме для win32)
//
int MainWindow::writes(const char *data, int len)
{
int ret = -1;

    if (!sdev) return ret;

    if (sdev->isOpen()) {
        ret = (int)sdev->write(data, len);
        QByteArray mas; mas.append(data, len);
        //LogSave(nullptr, mas, false);
    }

    return ret;
}
//----------------------------------------------------------------------
//   Функция возвращает контрольную сумму CRC8 массива байт
//
uint8_t MainWindow::crc8(const uint8_t *uk, uint16_t bytes)
{
uint8_t crc = 0;

    for (uint16_t i = 0; i < bytes; i++) crc += *(uk + i);

    return crc;
}
//--------------------------------------------------------------------------------
void MainWindow::initList()
{
    cmd_list_ind = 0;
    total_cmd_list = 0;
    all_cmd_in_list = 0;
    cmd_list.clear();
    mode = noneCmd;
    if (apiBuf.data) delete apiBuf.data;
    apiBuf.len = 0;
}
//--------------------------------------------------------------------------------
void MainWindow::getApi()
{
    initList();
    mode = stopCmd;//downCmd;
    emit sig_mkList(mode);
}
//--------------------------------------------------------------------------------
void MainWindow::progApi()
{
    initList();
    mode = binCmd;//progCmd;
    emit sig_mkList(mode);
}
//--------------------------------------------------------------------------------
void MainWindow::compApi()
{
    initList();
    mode = stopCmd;//compCmd;
    emit sig_mkList(mode);
}
//--------------------------------------------------------------------------------
void MainWindow::crc32File()
{
    initList();
    mode = startCmd;//crcCmd;
    emit sig_mkList(mode);
}
//--------------------------------------------------------------------------------
void MainWindow::qstr(const char *buf, uint16_t len, QString *out)
{
QString ms;
char *uk = (char *)buf;
uint16_t i = 0;

    while (i < len) {
        ms.sprintf(" %02X", *uk++);
        out->append(ms);
    }
}
//----------------------------------------------------------------------
//   Функция формирует символьную строку для печати из данных
//    сформированных функцией mkFrame()
//
void MainWindow::prnFrame(const uint8_t *buf, uint16_t len, QString *st, record_t *rc)
{
    if (!buf || !st) return;

    cmd_hdr_t *frame = (cmd_hdr_t *)buf;
    uint16_t sz = sizeof(cmd_hdr_t);

    QString stx, ms;
    QByteArray bt; bt.append(all_mode[rc->cmd]);
    stx.sprintf("Frame(%u):'%s'\n\tper_rzv:0x%02X%02X\n\tsn:0x%04X\n"
                "\thd:0x%02X\n\t\trw:%u inc:%u text:%u rst:%u addr_ext:%u ld:%u\n",
                rc->cmd, bt.data(), frame->pre, frame->rzv, frame->sn,
                *(buf + 4), frame->hd.rw, frame->hd.inc, frame->hd.text, frame->hd.rst, frame->hd.addr_ext, frame->hd.ld);
    if (len > (sz + 1)) {
        if (rc->cmd == putCmd) {
            //ms.sprintf("\tdata(%u):...", len - sz - 1);
            stx.append(tr("\tdata(%1):...").arg(len - sz - 1));
        } else {
            stx.append("\tdata:");
            qstr((char *)(buf + sz), len - 1, &stx);
        }
        stx.append("\n");
    }
    ms.sprintf("\tcrc8:0x%02X\n", *(buf + len - 1));
    *st = stx.append(ms);
}
//--------------------------------------------------------------------------------
void MainWindow::slot_goProc(int8_t md)
{
    if ((total_cmd_list <= 0) || devErr) return;

    if (getrec) {
        rec = cmd_list.at(cmd_list_ind);
        cmd_list_ind++;
        if (total_cmd_list) total_cmd_list--;
    }

    QString chap;
    cmd = rec.cmd;
    patch = false;
    goCmd = false;


    if (rec.cmd >= readCmd) {
        if (rec.repeat) {
            rec.adr = apiAddr;
            rec.len = apiLen;
            if (pTmp) memcpy(to_dev_data, pTmp + apiSnd, ((apiLen + 1) << 2));
        }
        uint16_t frame_len = mkFrame(rec.cmd, rec.adr, rec.len, rec.data, (uint8_t *)buff, &ackLen);
        if (frame_len) {
            if ((cmd == textCmd) && (md != textCmd)) {
                ackLen++;
                patch = true;
            }
            if (ackLen != -1) {
                prnFrame((uint8_t *)buff, frame_len, &chap, &rec);
            } else {
                ackLen = 64;
                if (frame_len > 2)
                    chap.sprintf("< (%u):%.*s\n", frame_len, frame_len - 2, buff);
                else
                    chap.sprintf("< (%u):%.*s\n", frame_len, frame_len, buff);
            }
            LogSave(nullptr, chap, false);
            uint8_t wr_ok = 0;
            if (writes(buff, frame_len) == frame_len) wr_ok = 1;
            if (wr_ok) {
                goCmd = true;
                if (cmd < stopCmd) {
                    chap.sprintf("< (%u-%u):", frame_len, ackLen);
                    qstr(buff, frame_len, &chap);
                    chap.append("\n");
                    LogSave(nullptr, chap, false);
                }
                memset(buff, 0, sizeof(buff));
                memset(ibuff, 0, sizeof(ibuff));
                wait_ack = get_tmr(200);//2s
                if (cmd == stopCmd) rec.err = 1;
            } else {
                chap = (tr("Error sending to device %1 bytes :%2\n").arg(frame_len).arg(strerror(errno)));
                LogSave(nullptr, chap, false);
            }
        } else {
            chap.sprintf("Error : (mkFrame() == %u) <= 0", frame_len);
            ui->status->clear();
            ui->status->setText(chap);
        }
    }

}
//--------------------------------------------------------------------------------
void MainWindow::slot_timeOutAck()
{
    ui->status->clear();
    ui->status->setText("Command '" + all_mode[mode] +"' : Wait answer timeout !");
    cmd = noneCmd;
    goCmd = false;
}
//--------------------------------------------------------------------------------
void MainWindow::slot_Ready()
{
    wait_ack = 0;
    cmd = noneCmd;
    goCmd = false;
    ui->status->clear();
    ui->status->setText("Command '" + all_mode[mode] +"' : Answer from device Ready !");
}
//--------------------------------------------------------------------------------
void MainWindow::slot_mkList(int8_t md)
{
    switch (md) {
        case progCmd:
        case compCmd:
        case downCmd:
            apiBuf.size = 1;
            apiBuf.data = new QByteArray();
            apiBuf.len = 0;
            if (apiBuf.data == nullptr) {
                devErr = errGetMem;
                ui->status->clear();
                ui->status->setText("Error #" + QString::number(devErr, 10) + " '" + all_err_str[devErr] +"'");
                return;
            }
        break;
    }

    mkList(md);
    ui->status->clear();
    ui->status->setText("Make list with #" + QString::number(all_cmd_in_list, 10) + " commands for mode '" + all_mode[md] +"' done");

    if (md == crcCmd) getFile();
    else {
        tmr_cmd = get_tmr(1);//10ms
        memset((uint8_t *)&rec, 0, sizeof(record_t));
        apiSnd = 0;
        getrec = true;
        emit sig_goProc(md);
    }

}
//--------------------------------------------------------------------------------
//  Функция создает элементарную команду-запрос к датчику.
//  Функция возвращает количество байт команды при успешном выполнении функции,
//  в противном случае возвращет ноль
//
uint16_t MainWindow::mkFrame(uint8_t cmd, uint32_t addr, uint8_t len, uint8_t *in, uint8_t *out, int *ack_len)
{
uint16_t ret = 0;
cmd_hdr_t hd;
uint8_t crc = 0;
int alen = 5;

    memset((uint8_t *)&hd, 0, sizeof(cmd_hdr_t));
    hd.pre = 0xAA;
    hd.rzv = 0x00;
    hd.sn = HTONS(serNum);
    ret += sizeof(cmd_hdr_t);
    if (len) hd.hd.inc = 1;

    switch (cmd) {
        case readCmd://команда "читать SAS_Registers" (адреса 0x00..0xff)
            memcpy(out, &hd.pre, ret);
            out[ret++] = (uint8_t)addr;
            out[ret++] = len;
            crc = crc8(out + 1, ret - 1);
            out[ret++] = crc;
            alen += 4;
            if (len) alen += (len << 2);
        break;
        case writeCmd://WR_REG:
            hd.hd.rw = 1;
            hd.hd.inc = 0;
            memcpy(out, &hd.pre, ret);
            out[ret++] = (uint8_t)addr;//addr : 0..0xff
            out[ret++] = 0;//len must bee = 0 !!!
            if (in) memcpy(out + ret, in, sizeof(uint32_t));// this DWORD write to SAS_Register
            ret += sizeof(uint32_t);
            crc = crc8(out + 1, ret - 1);
            out[ret++] = crc;
        break;
        case getCmd://команда "читать flash-память" (адреса 0x180000..) - только в режиме загрузчика !
            hd.hd.inc = 1;
            hd.hd.addr_ext = 1;
            memcpy(out, &hd.pre, ret);
            memcpy(out + ret, (uint8_t *)&addr, sizeof(uint32_t));
            ret += sizeof(uint32_t);
            out[ret++] = len;
            crc = crc8(out + 1, ret - 1);
            out[ret++] = crc;
            alen += 4;
            if (len) alen += (len << 2);
        break;
        case putCmd://PUT_ADR:
            hd.hd.rw = 1;
            hd.hd.addr_ext = 1;
            memcpy(out, &hd.pre, ret);
            memcpy(out + ret, (uint8_t *)&addr, sizeof(uint32_t));
            ret += sizeof(uint32_t);
            out[ret++] = len;//0;//len must bee = 0 !!!
            if (in) {
                memcpy(out + ret, in, sizeof(uint32_t) * (len + 1));// this DWORDs write to flash memory
                ret += sizeof(uint32_t) * (len + 1);
            }
            crc = crc8(out + 1, ret - 1);
            out[ret++] = crc;
        break;
        case bootCmd://команда "перевести датчик в режим загрузчика" (для последующей записи/чтении flas-памяти)
            hd.hd.ld = 1;
            hd.hd.rw = 1;
            hd.hd.inc = 0;
            memcpy(out, &hd.pre, ret);
            out[ret++] = (uint8_t)addr;
            out[ret++] = 0;//len;
            memset(&out[ret], 0, sizeof(uint32_t));
            ret += sizeof(uint32_t);
            crc = crc8(out + 1, ret - 1);
            out[ret++] = crc;
        break;
        case rstCmd://команда "перегрузить датчик"
            hd.hd.rst = 1;
            hd.hd.rw = 1;
            hd.hd.inc = 0;
            memcpy(out, &hd.pre, ret);
            out[ret++] = (uint8_t)addr;
            out[ret++] = 0;//len;
            memset(&out[ret], 0, sizeof(uint32_t));
            ret += sizeof(uint32_t);
            crc = crc8(out + 1, ret - 1);
            out[ret++] = crc;
        break;
        case textCmd://команда "переход в текствый режим обмена с датчиком"
            hd.hd.text = 1;
            hd.hd.rw = 1;
            hd.hd.inc = 0;
            memcpy(out, &hd.pre, ret);
            out[ret++] = (uint8_t)addr;
            out[ret++] = 0;//len;
            memset(&out[ret], 0, sizeof(uint32_t));
            ret += sizeof(uint32_t);
            crc = crc8(out + 1, ret - 1);
            out[ret++] = crc;
        break;
        case stopCmd://команда "перевести датчик в режим останова"
            ret = strlen(to_stop);
            memcpy(out, to_stop, ret);
            alen = -1;
        break;
        case startCmd://команда "выполнить однократную выдачу данных из датчика в тестовом режиме обмена"
            ret = strlen(to_start);
            memcpy(out, to_start, ret);
            alen = -1;
        break;
        case binCmd://команда "перевести датчик в бинарный режим обмена"
            ret = strlen(to_bin);
            memcpy(out, to_bin, ret);
            alen = -1;
        break;

            default : ret = 0;
    }

    *ack_len = alen;

    return ret;
}
//----------------------------------------------------------------------
//   Функция выполняет процедуру добавления в список элементарных команд
//    для дальнейшей их последовательной выдаче датчику
//
void MainWindow::addToList(record_t *rc, uint8_t fin)
{
    cmd_list_ind++;
    cmd_list << *rc;
    if (fin) {
        all_cmd_in_list = total_cmd_list = cmd_list_ind + 1;
        cmd_list_ind = 0;
    }
}
//----------------------------------------------------------------------
//   Функция формирует список элементарных команд для датчика,
//     возвращая количество команд в списке или ноль если он пуст
//
int8_t MainWindow::mkList(uint8_t cmd)
{
cmd_list_ind = -1;
total_cmd_list = 0;
record_t rec;

    memset((uint8_t *)&rec, 0, sizeof(record_t));
    rec.data = nullptr;

    cmd_list.clear();

    switch (cmd) {
        case compCmd:
        case downCmd:
            rec.cmd = stopCmd;//STOP
            addToList(&rec, 0);
            //
            rec.cmd = binCmd;//BIN
            addToList(&rec, 0);
            //
            rec.cmd = readCmd;//read:52 - reaad dev_type
            rec.adr = DEV_TYPE_ADDR;//0x52;
            rec.len = 0;
            addToList(&rec, 0);
            //
            rec.cmd = readCmd;//read:0 - reaad SN
            rec.adr = 0;
            rec.len = 0;
            addToList(&rec, 0);
            //
            rec.cmd = bootCmd;//BOOT
            addToList(&rec, 0);
            //
            rec.cmd = getCmd;//GET - read flash memory by adr
            rec.adr = API_HDR_ADDR_DEF + TEST_SHIFT_ADDR;
            rec.len = (sizeof(apiHdrData_t) >> 2) - 1;//6 - 1;//читаем 7 слов размерностью uint32_t : версия упаковки, 2 слова маркера, адрес API, длинна API, CRC32 API
            addToList(&rec, 0);
            //
            memset((uint8_t *)&rec, 0, sizeof(record_t));
            rec.cmd = getCmd;//GET - read flash memory by adr
            rec.repeat = 1;
            rec.adr = apiAddr;//API_ADDR_DEF;
            rec.len = apiLen;//255;
            addToList(&rec, 0);
            //
            memset((uint8_t *)&rec, 0, sizeof(record_t));
            rec.cmd = rstCmd;//RESET
            addToList(&rec, 0);
            //
            memset((uint8_t *)&rec, 0, sizeof(record_t));
            rec.cmd = textCmd;//TEXT
            addToList(&rec, 0);
            //
            rec.cmd = startCmd;//START
            addToList(&rec, 1);
        break;
        case progCmd:
            //
            rec.cmd = stopCmd;//STOP
            addToList(&rec, 0);
            //
            rec.cmd = binCmd;//BIN
            addToList(&rec, 0);
            //
            rec.cmd = readCmd;//read:52 - read dev_type
            rec.adr = DEV_TYPE_ADDR;//0x52;
            rec.len = 0;
            addToList(&rec, 0);
            //
            rec.cmd = readCmd;//read:0 - reaad SN
            rec.adr = 0;
            rec.len = 0;
            addToList(&rec, 0);
            //
            rec.cmd = bootCmd;//BOOT
            addToList(&rec, 0);
            //
            rec.cmd = getCmd;//GET - read header api from flash
            rec.adr = API_HDR_ADDR_DEF + TEST_SHIFT_ADDR;
            rec.len = (sizeof(apiHdrData_t) >> 2) - 1;//читаем 7 слов : версия упаковки, 2 слова маркера, адрес API, длинна API, CRC32 API
            addToList(&rec, 0);
            //
            memset((uint8_t *)&rec, 0, sizeof(record_t));
            rec.cmd = putCmd;//PUT - write to flash
            rec.adr = API_HDR_ADDR_DEF + TEST_SHIFT_ADDR;
            rec.len = (sizeof(apiHdrData_t) >> 2) - 1;//len must bee set in dword - 1
            rec.data = to_dev_data;
            addToList(&rec, 0);

//!!!!!!!!!!!!!!!!!!  Собственно запись api  !!!!!!!!!!!!!!!!!!!!!!!!!
            memset((uint8_t *)&rec, 0, sizeof(record_t));
            rec.cmd = putCmd;//PUT - write to flash memory API_BODY!!!
            rec.repeat = 1;
            rec.adr = apiAddr;//API_ADDR_DEF;
            rec.len = apiLen;//255;
            rec.data = to_dev_data;
            addToList(&rec, 0);

//-----------   Чтение записанного заголовка api из флэш   ----------------
            memset((uint8_t *)&rec, 0, sizeof(record_t));
            rec.cmd = getCmd;//GET - read flash memory by adr
            rec.adr = API_HDR_ADDR_DEF + TEST_SHIFT_ADDR;
            rec.len = (sizeof(apiHdrData_t) >> 2) - 1;//читаем 7 слов : версия упаковки, 2 слова маркера, адрес API, длинна API, CRC32 API
            addToList(&rec, 0);
//-----------   Чтение записаной api из флэш для сравнения с файлом   ----------------
            memset((uint8_t *)&rec, 0, sizeof(record_t));
            rec.cmd = getCmd;//GET - read flash memory by adr
            rec.repeat = 1;
            rec.adr = apiAddr;//API_ADDR_DEF;
            rec.len = apiLen;//255;
            addToList(&rec, 0);
//------------------------------------------------------------

            memset((uint8_t *)&rec, 0, sizeof(record_t));
            rec.cmd = rstCmd;//RESET
            addToList(&rec, 0);
            //
            rec.cmd = textCmd;//TEXT
            addToList(&rec, 0);
            //
            rec.cmd = startCmd;//START
            addToList(&rec, 1);
        break;
        case readCmd://read SAS_Registers , 0x00..0xff
            rec.cmd = readCmd;
            rec.adr = (uint8_t)regAddr;
            rec.len = regLen;
            addToList(&rec, 1);
        break;
        case writeCmd://qrite to SAS_Registers by 0x00..0xff DWORD from regLen
            rec.cmd = writeCmd;
            rec.adr = (uint8_t)regAddr;
            rec.len = 0;//regLen;
            rec.data = to_dev_data;
            addToList(&rec, 1);
        break;
        case rstCmd://RD_REG:// = 0,
            rec.cmd = rstCmd;//RESET
            addToList(&rec, 1);
        break;
        case textCmd:
            rec.cmd = textCmd;//RESET
            addToList(&rec, 1);
        break;
        case binCmd:
            rec.cmd = binCmd;//BIN
            addToList(&rec, 1);
        break;
        case stopCmd:
            rec.cmd = stopCmd;//STOP
            addToList(&rec, 1);
        break;
        case startCmd:
            rec.cmd = startCmd;//START
            addToList(&rec, 1);
        break;
        case bootCmd:
            rec.cmd = bootCmd;//BOOT
            addToList(&rec, 1);
        break;
        case getCmd:
            rec.cmd = getCmd;//read flash memory by adr
            rec.adr = regAddr;//adr must bee >= API_HDR_ADDR_DEF
            rec.len = regLen;//0..0xff
            addToList(&rec, 1);
        break;
        case putCmd:
            rec.cmd = putCmd;//write flash memory by adr
            rec.adr = regAddr;// >= API_HDR_ADDR_DEF
            rec.len = regLen;//0..0xff
            rec.data = to_dev_data;
            addToList(&rec, 1);
        break;

    }

    return total_cmd_list;

}



