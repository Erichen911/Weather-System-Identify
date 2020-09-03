#include "mainwindow.h"
#include <QApplication>
#include <QSplashScreen>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    QFile qss("style.qss");
    qss.open(QFile::ReadOnly);
    qApp->setStyleSheet(qss.readAll());
    qss.close();
    w.setWindowTitle(QString::fromLocal8Bit("天气系统识别与降水落区订正系统"));
    w.setWindowIcon(QIcon("icon.jpg"));

    QPixmap openpic("new.jpg");
    QSplashScreen splash(openpic);
    splash.show();
    for(long index = 0; index < 1000000000; index++);//显示延时
    splash.finish(&w);
    w.show();

    return a.exec();
}
