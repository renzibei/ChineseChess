#include "mainwindow.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    //QDesktopWidget* desktop =  QApplication::desktop();
    //QScreen * screen =  QGuiApplication::primaryScreen();

    MainWindow w;
    w.show();

    return a.exec();
}
