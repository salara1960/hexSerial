#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <inttypes.h>
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifdef __WIN32
#include <windows.h>
#else
#include <arpa/inet.h>
#include <endian.h>
#endif

#include <QApplication>
#include <QMainWindow>
#include <QPushButton>
#include <QByteArray>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QFile>
#include <QDateTime>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QMessageBox>
#include <QPixmap>
#include <QCheckBox>
#include <QSystemTrayIcon>
#include <QCloseEvent>
#include <QMouseEvent>


//********************************************************************************

#define SET_DEBUG
#define SET_MOUSE_KEY


#define max_buf 2048

#define STX  0x02
#define ETX  0x03
#define EOT  0x04
#define ENQ  0x05
#define ACK  0x06
#define DLE  0x10
#define NAK  0x15
#define ESC  0x1B
#define LF   0x0A
#define CR   0x0D

#define keyCnt 5
#define sendkeyCnt 6

typedef enum {
    KEY_ACK = 0,
    KEY_NAK,
    KEY_ENQ,
    KEY_EOT,
    KEY_KEY
} keys_t;

//********************************************************************************

namespace Ui {
class MainWindow;
}

class SettingsDialog;
class pwdDialog;

//********************************************************************************
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    class TheError {
        public :
            int code;
            TheError(int);
    };

    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void timerEvent(QTimerEvent *event);

#ifdef SET_MOUSE_KEY
protected:
    virtual void mousePressEvent(QMouseEvent *);
#endif

public slots:
    void slotButtonData();
    void KeyProg(QByteArray);
    int initSerial();
    void deinitSerial();
    void LogSave(const char *, const QByteArray &, bool, bool);
    void About();
    //void Config();
    unsigned char myhextobin(const char *);
    void hexTobin(const char *, QByteArray *);
    void clrLog();
    //
    //tray
    void changeEvent(QEvent*);
    void closeEvent(QCloseEvent *);
    void trayIconActivated(QSystemTrayIcon::ActivationReason reason);
    void trayActionExecute();
    void setTrayIconActions();
    void showTrayIcon();

#ifdef SET_MOUSE_KEY
    //Press mouse button
    void slotRM(int, int);
#endif

private slots:
    void ReadData();
    void slotError(QSerialPort::SerialPortError);
    void on_ack_clicked();
    void on_nak_clicked();
    void on_enq_clicked();
    void on_eot_clicked();
    void on_answer_clicked();
    void on_any_clicked();

    void on_connect();
    void on_disconnect();

    void slotWrite(QByteArray &);

signals:
    void sigWrite(QByteArray &);
    void sigAbout();
    void sigButtonData();
    void sigConn();
    void sigDisc();

#ifdef SET_MOUSE_KEY
    void sigRM(int, int);
#endif

private:
    Ui::MainWindow *ui;
    int tmr_sec, MyError;
    QSerialPort *sdev;
    QByteArray rxData, txData;
    QString sdevName, sdevConf;
    bool hex, first, con;
    //KeyProg
    int keyId;
    QByteArray keyArr[keyCnt];
    pwdDialog *keys;
    QPushButton *keyAdr[keyCnt];
    const QString keyName[keyCnt] = {"ACK", "NAK", "ENQ", "EOT", "KEY"};
    const char defKeys[keyCnt] = {ACK, NAK, ENQ, EOT, LF};
    const char defSendKeys[sendkeyCnt] = {STX, 0x55, 0x00, 0x00, ETX, 0x56};//(02 55 00 a0 03 f6}
    //settings
    SettingsDialog *conf = nullptr;
    //tray
    QMenu *trayIconMenu;
    QAction *minA;
    QAction *maxA;
    QAction *quitA;
    QSystemTrayIcon *trayIcon;
};

#endif // MAINWINDOW_H
