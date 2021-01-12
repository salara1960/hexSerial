#include "mainwindow.h"

#include <QApplication>
#include <QLockFile>
#include <QDir>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "UTF8");

    try {
        QLocale loc(QLocale::Russian, QLocale::RussianFederation);
        QLocale::setDefault(loc);

        QApplication app(argc, argv);

        QLockFile lockFile(QDir::temp().absoluteFilePath("hexSerial.lock"));
        if (!lockFile.tryLock(100)) {
            QMessageBox::warning(nullptr, "Attention !!!", "Program is already running");

            return 1;
        }

        MainWindow wnd;

        wnd.show();

        app.exec();
    }

    catch (MainWindow::TheError(er)) {
        auto errorCode = er.code;
        QString errorCodeStr(errorCode);
        QString errorStr = "";
        if (errorCode > 0) {
            if (errorCode & 1) errorStr.append("Error create serial port object (" + errorCodeStr + ")\n");
            if (errorCode & 2) errorStr.append("Error starting timer_wait_data (" + errorCodeStr + ")\n");
            if (errorCode & 4) errorStr.append("Error create settings object\n");
            if (errorCode & 8) errorStr.append("Error reading from serial port\n");
        }
        else {
            errorStr.append("Unknown Error (" + errorCodeStr + ")\n");
        }

        if (errorStr.length() > 0) {
            perror(reinterpret_cast<char *>(errorCodeStr.data()));
        }

        return errorCode;
    }
    catch (std::bad_alloc) {
        perror("Error while alloc memory via call function new\n");
        return -1;
    }

    return 0;
}
