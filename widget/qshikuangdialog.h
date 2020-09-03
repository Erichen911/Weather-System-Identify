#ifndef QSHIKUANGDIALOG_H
#define QSHIKUANGDIALOG_H

#include <QVBoxLayout>
#include <QDebug>
#include <QComboBox>
#include <QCheckBox>
#include <QTextBrowser>
#include <QDialog>
#include <QLabel>
#include <QMessageBox>
#include <QCloseEvent>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QLineEdit>
#include <QPushButton>
#include <myitem.h>
#include <pressure.h>
class Qshikuangdialog : public QDialog
{
public:
    Qshikuangdialog(QGraphicsScene* sscene, QList<QXItem *> tthelistofQXitem, QList<WindDKItem *> tthelistofDKitem, QList<QBItem *> tthelistofQBitem, QList<WindItem *> tthelistofDirectionitem, QList<WindTextItem *> tthelistofPoweritem, QList<QList<QQMe>> tthelistofheightlist, QList<ColdToughItem *> tthelistofCTitem, QList<HotRidgeItem *> tthelistofHRitem,double llon_unit, double llat_unit, double llon_start,double llat_start,double llon_num, double llat_num,int sshadowtype);
    void closeEvent(QCloseEvent *event);
private:
    QGraphicsScene* scene;
    QList<QXItem *> thelistofQXitem;
    QList<WindDKItem *> thelistofDKitem;
    QList<QBItem *> thelistofQBitem;
    QList<WindItem *> thelistofDirectionitem;
    QList<WindTextItem *> thelistofPoweritem;
    QList<QList<QQMe>> thelistoftheheightlist;
    QList<ColdToughItem *> thelistCTitem;
    QList<HotRidgeItem *> thelistHRitem;
    QComboBox* three_ISBL2;
    QCheckBox* three_checkdirection;
    QCheckBox* three_checkpower;
    QCheckBox* three_checkqixuan;
    QCheckBox* three_checkqiebian;
    QCheckBox* three_checkdkjl;
    QTextBrowser* two_text;


    QLineEdit* three_minh;
    QLineEdit* three_maxh;
    QLineEdit* three_gaph;
    QPushButton* two_pushbutton;
    QCheckBox* three_findfufao;
    QCheckBox* three_checkheightvalue;
    QCheckBox* three_checkcoldtough;
    QCheckBox* three_checkhotridge;
    QCheckBox* three_checkhight;
    QTextBrowser* two_text2;

    int minheight;
    int maxheight;
    int gapheight;

    double lon_unit;
    double lat_unit;
    double lon_start;
    double lat_start;
    double lon_num;
    double lat_num;

    int shadowtype;

    VoltageItem *DrawPressures(QList<QQMe> heightlist, int max, int min, int space, int shadowtype);
    FuGaoItem *FindFugao(QList<QQMe> heightlist,int shadowtype);
private slots:
    void ChooseCeng();
    void showdirection();
    void showpower();
    void showqixuan();
    void showqiebian();
    void showdkjl();
    void showheighttext();
    void showcoldtough();
    void showhotridge();
    void showhight();
    void showpressureButton();
    void showfugao();
};

#endif // QSHIKUANGDIALOG_H
