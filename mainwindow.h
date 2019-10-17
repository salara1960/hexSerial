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


//********************************************************************************

#define SET_DEBUG

#define max_buf 2048

#define STX  0x02
#define ETX  0x03
#define EOT  0x04
#define ENQ  0x05
#define ACK  0x06
#define DLE  0x10
#define NAK  0x15
#define ESC  0x1B


//********************************************************************************

namespace Ui {
class MainWindow;
}

class SettingsDialog;

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

public slots:
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
    //

private slots:
    void ReadData();
    void slotError(QSerialPort::SerialPortError);
    void on_ack_clicked();
    void on_nak_clicked();
    void on_enq_clicked();
    void on_eot_clicked();
    void on_answer_clicked();

    void on_connect_clicked();
    void on_disconnect_clicked();

    void slotWrite(QByteArray &);

signals:
    void sigWrite(QByteArray &);
    void sigAbout();

private:
    Ui::MainWindow *ui;
    int tmr_sec, MyError;
    QSerialPort *sdev;
    QByteArray rxData, txData;
    QString sdevName, sdevConf;
    bool hex, first, con;
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
