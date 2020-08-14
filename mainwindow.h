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

#define BUF_SIZE         4096
#define max_buf          2048
#define keyCnt           9
#define MAX_SAS_REG      43
#define TO_DEV_SIZE      1024

#define MAX_MODE         14
#define MAX_ERR_STR      13
#define MAX_ALL_DEV_TYPE 3
#define MAX_TRY          3

#define SER_NUM          0x0000
#define API_HDR_ADDR_DEF 0x180000
#define API_ADDR_DEF     0x180100
#define API_HDR_ADDR_RD  7//читаем 7 адресов, так как только там нужная инфа
#define MAGIC1           0x55AA0F0F
#define MAGIC2           0xFF00CAAC
#define DEV_TYPE_ADDR    0x52

#define API_VERSION_DEF  0x34
#define API_VERSION_OFFSET 0x12e90 //по этому смещению от начала в файле должна находиться строка "api_version:3.6" (3-мажор, 6-минор)
#define API_VERSION_STRING "api_version:"

#ifdef PROG_TEST_MODE
    #define TEST_SHIFT_ADDR 0x40000
#else
    #define TEST_SHIFT_ADDR 0
#endif


// Маркосы-аналоги функций htons() htonl()
#define HTONS(x) \
    ((uint16_t)((x >> 8) | ((x << 8) & 0xff00)))
#define HTONL(x) \
    ((uint32_t)((x >> 24) | ((x >> 8) & 0xff00) | ((x << 8) & 0xff0000) | ((x << 24) & 0xff000000)))

//********************************************************************************

enum {
    crcCmd = 0,
    readCmd,
    writeCmd,
    getCmd,
    putCmd,
    bootCmd,
    rstCmd,
    textCmd,
    downCmd,
    compCmd,
    progCmd,
    stopCmd,
    startCmd,
    binCmd,
    noneCmd
};

enum {
    errNo = 0,   // "Ok",
    errExec,     // "Operation error",//"Ошибка выполнения операции",
    errRstFlash, // "FLASH reset error",//"Ошибка сброса FLASH",
    errWrFlash,  // "FLASH write error",//"Ошибка записи FLASH",
    errNoAck,    // "Error waiting for response",//"Ошибка ожидания ответа",
    errCmdReject,// "Command rejected",//"Команда отклонена",
    errSigInt,   // "Abort from keyboard",//"Процедура прервана с клавиатуры",
    errFatal,    // "Fatal error",//"Неисправимая ошибка",
    errFileLen,  // "Error fileLen",
    errFileOpen, // "Error fOpen",
    errGetMem,   // "Error getMem",
    errRdFile,   // "Error fRead",
    errWrFile,
    errUnknown   // "Unknown error"//Неизвестная ошибка"
};

#pragma pack(push,1)
typedef struct {
    uint8_t ld:1;//Признак перехода в режим загрузчика
    uint8_t addr_ext:1;//Признак 4 байтового адреса (используется только в режиме загрузчика)
    uint8_t rst:1;
    uint8_t text:1;
    uint8_t none:2;
    uint8_t inc:1;//признак инкремента адреса при выполнении операции: =0 – нет инкремента
                  //(не блочная операция), =1 – инкремент (блочная операция)
    uint8_t rw:1;//Признак чтения/записи( чтение – 0, запись – 1)
} byte_hdr_t;
#pragma pack(pop)
#pragma pack(push,1)
typedef struct {
    uint8_t pre;
    uint8_t rzv;
    uint16_t sn;//старший байт, затем младший //SN[1],SN[0] : SN[1] - старший. SN[0] - младший - ФОРМАТ NETWORK
    byte_hdr_t hd;
//    uint8_t addr;//0x00..0xff
//    uint8_t len;//0..15, 0-одно слово, 1-два слова.....
//    uint32_t data;//ФОРМАТ NETWORK
//    uint8_t crc;
} cmd_hdr_t;
#pragma pack(pop)

//#pragma pack(push,1)
typedef struct {
    uint8_t sasAddr;
    uint8_t len;
    char name[16];
} sas_reg_t;
//#pragma pack(pop)

#pragma pack(push,1)
typedef struct {
    uint8_t cmd;
    uint8_t err;
    uint8_t cnt;
    uint8_t repeat;
    uint32_t adr;
    uint8_t len;
    uint8_t *data;
} record_t;
#pragma pack(pop)

#pragma pack(push,1)
typedef struct {
    uint16_t serNumber;
    uint8_t swVersion;
    uint8_t pkVersion;
} psw0_t;
#pragma pack(pop)

#pragma pack(push,1)
typedef struct {
    uint32_t vpk;     // версия упаковщика
    uint32_t Mag1;    // константа 1
    uint32_t Mag2;    // константа 2
    uint32_t apiAddr; // начальный адрес API во flash-памяти
    uint32_t apiLen;  // длинна API в байтах
    uint32_t apiCrc;  // контрольная сумма API
    uint32_t apiVer;  // версия API
} apiHdrData_t;
#pragma pack(pop)

#pragma pack(push,1)
typedef struct {
    QByteArray *data;
    uint32_t len;
    uint32_t size;
} apiBuf_t;
#pragma pack(pop)

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
    void LogSave(const char *, QByteArray &, bool);
    void LogSave(const char *, const QString &, bool);
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
    bool getApiVer(const uint8_t *buf);
    void qstr(const char *buf, uint16_t len, QString *out);
    int writes(const char *data, int len);
    uint32_t get10ms();
    uint32_t get_tmr(uint32_t);
    int check_tmr(uint32_t);
    void initList();
    uint8_t crc8(const uint8_t *uk, uint16_t bytes);
    uint16_t mkFrame(uint8_t cmd, uint32_t addr, uint8_t len, uint8_t *in, uint8_t *out, int *ack_len);
    void addToList(record_t *rc, uint8_t fin);
    int8_t mkList(uint8_t cmd);
    void prnFrame(const uint8_t *buf, uint16_t len, QString *st, record_t *rc);

#ifdef SET_MOUSE_KEY
    //Press mouse button
    void slotRM(int, int);
#endif

private slots:
    void ReadData();
    void slotError(QSerialPort::SerialPortError);
    void on_answer_clicked();
    void keyPrs();

    void on_connect();
    void on_disconnect();
    void getFile();
    void getApi();
    void progApi();
    void compApi();
    void crc32File();
    void slot_mkList(int8_t);
    void slot_goProc(int8_t);
    void slot_Ready();
    void slot_timeOutAck();

    void slotWrite(QByteArray &);

signals:
    void sigWrite(QByteArray &);
    void sigAbout();
    void sigButtonData();
    void sigConn();
    void sigDisc();
    void sig_goProc(int8_t);
    void sig_Ready();
    void sig_timeOutAck();

    void sig_mkList(int8_t);


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

    //
    const sas_reg_t sas_reg[MAX_SAS_REG] = {
        {   0, 1, "PSW0"},
        {   1, 1, "PSW1"},

        {0x21, 1, "CHECK"},
        {0x22, 1, "STAGE"},
        {0x23, 2, "K1_AJUST"},
        {0x25, 2, "K2_AJUST"},
        {0x27, 2, "K_DL1"},
        {0x29, 2, "K_TMIN"},
        {0x2B, 2, "K_TMAX"},
        {0x2D, 1, "K_TEMP"},
        {0x2E, 1, "BAR_VALUE"},
        {0x2F, 2, "PSI1"},
        {0x31, 2, "TETA1"},
        {0x33, 2, "GAMMA1"},
        {0x35, 2, "dW1_0[0]"},
        {0x37, 2, "dW1_0[1]"},
        {0x39, 2, "dW1_0[2]"},
        {0x3B, 2, "PSI2"},
        {0x3D, 2, "TETA2"},
        {0x3F, 2, "GAMMA2"},
        {0x41, 2, "dW2_0[0]"},
        {0x43, 2, "dW2_0[1]"},
        {0x45, 2, "dW2_0[2]"},

        {0x4D, 1, "TEXT_PARAM1"},
        {0x4E, 1, "TEXT_PARAM2"},
        {0x4F, 1, "TEXT_PARAM3"},
        {0x50, 1, "BAUDRATE"},
        {0x51, 1, "SENSOR_BASE"},
        {0x52, 1, "DEV_CONF"},
        {0x53, 6, "PRES_CONF"},
        {0x59, 1, "PressF"},
        {0x5A, 1, "PressFP"},
        {0x5B, 1, "Deep"},
        {0x5C, 1, "Feet"},
        {0x5D, 1, "PressK"},
        {0x5E, 1, "TempK"},
        {0x5F, 1, "VccF"},
        {0x60, 1, "framID"},
        {0x61, 1, "maxFRAMsize"},
        {0x62, 1, "maxRecords"},
        {0x63, 1, "framStatus"},
        {0x64, 1, "i2cError"},
        {0x65, 1, "i2cErrorCounter"}
    };

    QList <record_t> cmd_list;
    int8_t cmd_list_ind = 0;
    int8_t total_cmd_list = 0;
    int8_t all_cmd_in_list = 0;
    uint8_t mode = noneCmd;
    uint16_t serNum = SER_NUM;
    uint32_t regAddr;
    uint8_t regLen;
    uint32_t apiAddr;
    uint32_t apiLen;
    uint32_t apiSnd;
    uint32_t api_version = API_VERSION_DEF;
    apiBuf_t apiBuf;
    uint8_t devErr, cmd;
    uint32_t tmr_cmd, wait_ack;
    record_t rec;
    bool getrec, patch, goCmd;
    int ackLen;
    char buff[BUF_SIZE + 8] = {0};
    char ibuff[BUF_SIZE + 8] = {0};
    uint8_t to_dev_data[TO_DEV_SIZE];
    QString chap;
    const QByteArray to_stop = "#\r\n";
    const QByteArray to_start = "m1\r\n";
    const QByteArray to_bin = "#082;bin\r\n";
    const QString all_mode[MAX_MODE] = {"crc", "read", "write", "get", "put", "boot", "rst", "text", "download", "compare", "prog", "stop", "start", "bin"};
    const QString all_dev_type[MAX_ALL_DEV_TYPE] = {"svm", "svp", "noname"};
    const QString all_err_str[MAX_ERR_STR] = {
        "Ok",
        "Operation error", // "Ошибка выполнения операции",
        "FLASH reset error", // "Ошибка сброса FLASH",
        "FLASH write error", // "Ошибка записи FLASH",
        "No answer", // "Ошибка ожидания ответа",
        "Command rejected", // "Команда отклонена",
        "Abort from keyboard", // "Процедура прервана с клавиатуры",
        "Fatal error", // "Неисправимая ошибка",
        "Error fileLen",
        "Error fileOpen",
        "Error getMem",
        "Error fileRead",
        "Unknown error"//Неизвестная ошибка"
    };

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
