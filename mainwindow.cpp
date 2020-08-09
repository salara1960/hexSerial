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
const QString vers = "1.4";//09.08.2020  !!! +++ !!!



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
    connect(ui->actionFile,       &QAction::triggered, this, &MainWindow::getFile);

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
    keyAdr[keyId] = ui->any;
    QString tstr;
    for (int i = 0; i < keyCnt; ++i) {
        keyArr[i].clear();
        keyArr[i].append(defKeys[i]);
        tstr.clear();
        for (int j = 0; j < keyArr[i].length(); j++) {
            //keyArr[i].clear();
            //keyArr[i].append(defKeys[i]);
            if (keyArr[i].at(j) != 0) {
                tstr.append(" 0x" + QString::number(keyArr[i].at(j), 16));
            }
        }
        keyAdr[i]->setToolTip(tstr);
    }
    //
    QByteArray stz; stz.clear();
    //stz.append(defSendKeys, sendkeyCnt);
    //ui->stx->setText(stz.toHex());
    ui->stx->setText("#");
    ui->stx->setEnabled(false);
    ui->hexBox->setCheckState(Qt::Unchecked);
    ui->asciiBox->setCheckState(Qt::Checked);
    ui->crlfBox->setCheckState(Qt::Checked);
    ui->hexBox->setEnabled(false);
    ui->asciiBox->setEnabled(false);
    ui->crlfBox->setEnabled(false);
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
    if (pTmp) free(pTmp);

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
            if (keyId < keyCnt - 1)
                keyAdr[keyId]->setToolTip(tmp.toHex(' '));
            else {
                QString tstr; tstr.clear();
                for (int j = 0; j < keyArr[keyId].length(); j++) {
                    if (keyArr[keyId].at(j) != 0) {
                        tstr.append(" 0x" + QString::number(keyArr[keyId].at(j), 16));
                    }
                }
                keyAdr[keyId]->setToolTip(tstr);
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
        QByteArray tmp;
        tmp.append(keyArr[keyId]);
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
                ui->status->clear();
                ui->status->setText("File " + *nm + " Size:" + QString::number(fileSize, 10) + ", CRC:0x" + QString::number(crcFile, 16));
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
#ifndef __WIN32
void MainWindow::setSPEED(int fd, const void *ptr)
{
    if ((fd < 0) || (ptr == nullptr)) return;

    struct termios tio;
    uint32_t speed = 0, blen = 0, parity = 0, sbits = 0, fctrl = 0;
    const SettingsDialog::Settings *p = (const SettingsDialog::Settings *)ptr;

    switch (p->baudRate) {
        case 460800: speed = B460800; break;
        case 500000: speed = B500000; break;
        case 576000: speed = B576000; break;
        case 921600: speed = B921600; break;
        case 1000000: speed = B1000000; break;
        case 1152000: speed = B1152000; break;
        case 1500000: speed = B1500000; break;
        case 2000000: speed = B2000000; break;
        case 2500000: speed = B2500000; break;
        case 3000000: speed = B3000000; break;
        case 3500000: speed = B3500000; break;
        case 4000000: speed = B4000000; break;
            default : speed = B230400;
    }
    switch (p->dataBits) {
        case 7: blen = CS7; break;
        case 6: blen = CS6; break;
        case 5: blen = CS5; break;
            default :blen = CS8;
    }

    switch ((int)p->parity) {
        case 0: //no
            break;
        case 2: //even
            parity = PARENB;
            break;
        case 3: //odd
            parity = PARODD;
            break;
        case 4: //space
            break;
        case 5: //mark
            break;
    }
    if (p->stopBits == 2) sbits = CSTOPB;
    if (p->flowControl == 1) fctrl = CRTSCTS;
    else if (p->flowControl == 2) fctrl = IXON | IXOFF;

    tcgetattr(fd, &tio);
    cfmakeraw(&tio);//set RAW mode
    tio.c_cflag = speed | blen | parity | sbits | fctrl | CREAD | CLOCAL;//CLOCAL-игнорировать управление линиями с помощью модема.
    tcflush(fd, TCIFLUSH);
    tcsetattr(fd, TCSANOW, &tio);
}
#endif
//--------------------------------------------------------------------------------
int MainWindow::initSerial()
{
    deinitSerial();

    sdev = new QSerialPort(sdevName);
    if (sdev) {
        SettingsDialog::Settings p = conf->settings();
        sdevName = p.name;   sdev->setPortName(sdevName);

#ifdef __WIN32
        //if (p.baudRate > 115200) p.baudRate = 115200;
#endif
        //if (p.baudRate <= 115200) {
            sdev->setBaudRate(p.baudRate);
            sdev->setDataBits(p.dataBits);
            sdev->setParity(p.parity);
            sdev->setFlowControl(p.flowControl);
            sdev->setStopBits(p.stopBits);
        //}

        if (!sdev->open(QIODevice::ReadWrite)) {
            delete sdev;
            sdev = nullptr;
            return 1;
        } else {
            //
            if (p.baudRate > 115200) {
#ifndef __WIN32
                //setSPEED(sdev->handle(), (const void *)&p);
#endif
            }
            //
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
    //int len = st.length();
    //fw.append(" (" + QString::number(len, 10) + ")");
    //if (rxd) fw.append("> ");
    //    else fw.append("< ");
    if (hex) fw.append(st.toHex(' '));
        else fw.append(st);
//    if (fw.indexOf("\r\n", 0) != -1) fw.truncate(st.length() - 2);
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

        ui->stx->setEnabled(true);
        //ui->hexBox->setEnabled(true);
        ui->asciiBox->setEnabled(true);
        ui->crlfBox->setEnabled(true);
        rxData.clear();
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
    //ui->hexBox->setEnabled(false);
    ui->asciiBox->setEnabled(false);
    ui->crlfBox->setEnabled(false);

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

        LogSave(nullptr, mas, false, false);
        //rxData.clear();
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

    if (ui->crlfBox->checkState() == Qt::Checked) tmp.append(cr_lf);
    if (tmp.length()) emit sigWrite(tmp);

}
//-----------------------------------------------------------------------
void MainWindow::on_any_clicked()
{
    if (sdev) {
        QByteArray m(keyArr[KEY_KEY]);
        hex = true;
        emit sigWrite(m);
    } else {
        keyId = KEY_KEY;
        emit sigButtonData();
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
        if (ms10 >= 99) {
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
    }
}
//-----------------------------------------------------------------------
int MainWindow::chkDone(QByteArray *buf)
{
int ret = -1;

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

    return ret;
}
//-----------------------------------------------------------------------
void MainWindow::ReadData()
{
//bool eol = false;
int ix = -1;

    while (!sdev->atEnd()) {
        rxData += sdev->readAll();
        ix = chkDone(&rxData);
        if (ix != -1) break;
    }
        /*if (rxData.length()) {
            if (ui->asciiBox->checkState() == Qt::Checked) {
                hex = false;
                eol = chkDone(&rxData);
                if (!eol) return;
            } else {
                eol = true;
                hex = true;
            }
            if (eol) {
                LogSave(nullptr, rxData, true, false);
                rxData.clear();
            }
        }*/
    if (ix != -1) {
        hex = false;
        int pos = 0;
        QByteArray dat, line = rxData.mid(0, ix);
        dat = line;
        if ((pos = line.indexOf("\r\n>", 0)) == -1) {
            if ((pos = line.indexOf("\r\n", 0)) != -1) line.remove(pos, 2);
        }
        LogSave(nullptr, line, true, false);
        if (rxData.length() > (ix + 1)) {
            dat = rxData.mid(ix, -1);
            //rxData.clear();
            rxData = dat;
        } else rxData.clear();

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



