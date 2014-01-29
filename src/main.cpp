#include <QApplication>
#include <QSettings>
#include <QtGui>
#include <QTextCodec>
#include <iostream>
#include "town.h"
using namespace std;
int main(int argc, char* argv[]){
    QApplication app(argc, argv);
    app.setApplicationName(QString("Township"));
    app.setWindowIcon (QIcon(":/images/logo.png"));
    MainWindow game = MainWindow(0,0,QString());
    game.showMaximized();
    return app.exec();
}
