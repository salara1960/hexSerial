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
    #include <termios.h>
    #include <unistd.h>
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
#include <QFileDialog>
#include <QIODevice>


//********************************************************************************


#define SET_DEBUG
#define SET_MOUSE_KEY

#define max_buf 2048

#define keyCnt 9


typedef enum {
    KEY_ACK = 0,
    KEY_NAK,
    KEY_ENQ,
    KEY_EOT,
    KEY_KEY,
    KEY_INFO,
    KEY_DEEP,
    KEY_CRC,
    KEY_ONE
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
    int chkDone(QByteArray *buf);

#ifdef SET_MOUSE_KEY
protected:
    virtual void mousePressEvent(QMouseEvent *);
#endif

public slots:
    void slotButtonData();
    void KeyProg(QString);
    int initSerial();
    uint32_t crc32(const uint32_t crc_origin, const uint8_t *buf, const uint32_t size);
    void deinitSerial();
    void LogSave(const char *, const QByteArray &, bool);
    void About();
    unsigned char myhextobin(const char *);
    void hexTobin(const char *, QByteArray *);
    void clrLog();
    //
    //tray
    /*
    void changeEvent(QEvent*);
    void closeEvent(QCloseEvent *);
    void trayIconActivated(QSystemTrayIcon::ActivationReason reason);
    void trayActionExecute();
    void setTrayIconActions();
    void showTrayIcon();
    */

#ifdef SET_MOUSE_KEY
    //Press mouse button
    void slotRM(int, int);
#endif

private slots:
    void ReadData();
    void slotError(QSerialPort::SerialPortError);
    void on_answer_clicked();
    /*
    void on_ack_clicked();
    void on_nak_clicked();
    void on_enq_clicked();
    void on_eot_clicked();
    void on_any_clicked();
    void on_info_clicked();
    void on_deep_clicked();
    void on_crc_clicked();
    void on_one_clicked();
    */
    void keyPrs();

    void on_connect();
    void on_disconnect();
    void getFile();

    void slotWrite(QByteArray &);

signals:
    void sigWrite(QByteArray &);
    void sigAbout();
    void sigButtonData();
    void sigConn();
    void sigDisc();

    void snd_ind(int);


#ifdef SET_MOUSE_KEY
    void sigRM(int, int);
#endif

private:
    Ui::MainWindow *ui;
    int tmr_sec, MyError;
    QSerialPort *sdev;
    QByteArray rxData, txData;
    QString sdevName, sdevConf;
    bool first, con;
    //KeyProg
    int keyId;
    QString keyArr[keyCnt];
    pwdDialog *keys;
    QPushButton *keyAdr[keyCnt];
    const QString keyName[keyCnt] = {"Stop", "Start", "Dirs", "Bin", "Text", "Info", "Deep", "CRC32", "Key"};
    const QString keyData[keyCnt] = {"#", "m1\r\n", "$dirs\r\n", "#082;bin\r\n", "#082;sc33\r\n", "#149\r\n", "#140\r\n", "#147\r\n", "s\r\n"};
    //settings
    SettingsDialog *conf = nullptr;
    uint32_t Tik, ms10;

    qint64 fileSize;
    uint8_t *pTmp;
    uint32_t crcFile;

    //tray
    /*
    QMenu *trayIconMenu;
    QAction *minA;
    QAction *maxA;
    QAction *quitA;
    QSystemTrayIcon *trayIcon;
    */
};

#endif // MAINWINDOW_H
