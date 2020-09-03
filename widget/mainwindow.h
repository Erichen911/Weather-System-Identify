#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDesktopWidget>
#include <QMainWindow>
#include <QFileDialog>
#include <QDebug>
#include <myitem.h>
#include <myview.h>
#include <QWidget>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QButtonGroup>
#include <QPushButton>
#include <QDockWidget>
#include <QGridLayout>
#include <QLabel>
#include <QComboBox>
#include <QtMath>
#include <vector>
#include <hanshuku.h>
#include <cv.h>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <dikongjiliu.h>
#include <tjuwind.h>
#include <handle.h>
#include <f_wind.h>
#include <QDir>
#include <qwinddialog.h>
#include <dkjlhanshu.h>
#include <cxx4/netcdfcpp.h>
#include <map>
#include <QThread>
#include <QThreadPool>
#include <qshikuangdialog.h>
#include <qdatadialog.h>
#include <QJsonObject>
#include <QJsonDocument>
#include <Line_jicao.h>
#include <pressure.h>
#include <QTextCodec>
#include <QXmlStreamReader>
#include <qcyclonedialog.h>
using namespace std;
using namespace cv;
using namespace netCDF;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_action_3_triggered();

    void on_action_2_triggered();

    void on_lonlatshadowtrigger_triggered();

    void on_lbtshadowtrigger_triggered();

    void on_mktshadowtrigger_triggered();

    void on_ifshowlonlat_triggered();

    void on_testbutton_triggered();

//    void on_TanKongJiLiuTrigger_triggered();

    void on_readfile_triggered();

    void on_get_shikuang_trigger_triggered();

private:
    QList<QList<QWind>> windlist;
    QList<QList<double>> mappointlist1;//地图1
    QList<QList<double>> mappointlist3;//地图2
    int shadowtype;
    bool ifshowlonlat;
    Ui::MainWindow *ui;
    int screen_W;//屏幕宽
    int screen_H;//屏幕高
    int ceng=1;

    QScrollArea *pscroll;
    QGraphicsScene *scene;
    MyView *view;
    QGraphicsView *mapview;

    QPainterPath GetMapPath(QList<QList<double> > pointlist);
    void SetViewCenter();
    void GetMicapsList(int datatype);
    void The11thDataDeal(QList<QWind> windvaluelist,double lon_unit,double lat_unit,double lon_start,double lon_end,double lat_start,double lat_end,double lon_num,double lat_num);
    void The4thDataDeal(QString filename);
    QXItem* FindQX(QList<QWind> windvaluelistU, QList<QWind> windvaluelistV, double lon_num, double lat_num, double lon_unit, double lon_start, double lat_unit, double lat_start, QString savename);
    WindDKItem* FindDKJL(QList<QWind> windvaluelistU, QList<QWind> windvaluelistV, QList<QWind> winddirectionlist, QList<QWind> windpowerlist, double lat_num, double lon_num, double lon_unit, double lon_start, double lat_start,QString savename);
    QBItem* FindQB(QList<QWind> windvaluelistU, QList<QWind> windvaluelistV, double lon_start, double lat_start, double lon_num, double lat_num, double lon_unit, double lat_unit, QString savename);
    ColdToughItem* FindCT(QList<QQMe> valueQ, int lon_num, int lat_num, int lon_unit, int lon_start, int lat_unit, int lat_start,QString savename);
    HotRidgeItem* FindHR(QList<QQMe> valueQ, int lon_num, int lat_num, int lon_unit, int lon_start, int lat_unit, int lat_start, QString savename);
    void Find_TKJL(QList<QList<float>> winddata);
    void GetShikuangData(QString filename, vector<float>* ceng, vector<float>* lontitude, vector<float>* lattitude, vector<vector<float> > *U, vector<vector<float> > *Vwind, vector<vector<float> > *HIGHT025, vector<vector<float> > *HIGHT, int &lon_num, int &lat_num, float &lon_start, float &lat_start, float &lon_unit, float &lat_unit);
    void TheShikuangDataDeal(QList<QList<QWind> > thelistofwindvaluelist, QList<QList<QQMe>> thelistofheightlist, QList<QList<QQMe> > thelistofheightlist_025, /* QList<QList<QQMe> > thelistofheightlist_025,*/ double lon_unit, double lat_unit, double lon_start, double lon_end, double lat_start, double lat_end, double lon_num, double lat_num, QString string, QString name);

    void ReadingFile(QString string,vector<float>* u,vector<float>* v, vector<float> z);

 };

QList<QList<double> > GetMapList(QString location);
QList<QQMe> GetAnotherPath(QString location);
//QList<QTr> makegrid(int M,int N);
//QList<QTr> findusegrid(QList<QMe> list, QList<QTr> trlist, double pressure);
//int findnextid(int lastid, int id, QList<QMe> list, QList<QTr> *trlist, double pressure, QList<QPointF> *pointlist);
//QPointF getXY(double Z0,double Zi,double Zj,int Xi,int Xj,int Yi,int Yj);
//QList<QPoint> Bessel(QList<QPoint> pointlist);
//QList<QPoint> SmoothingContour(QList<QPoint> pointlist);
//QPainterPath Smoothedpath(QList<QPoint> aftersmoothlist, bool isfrom0);
//QList<QPointF> FirstTimeCalculatePoint(QList<QTr> TRlist, QList<QTr> *trlist, QList<QMe> list, double pressure, int i);
//QList<QPointF> SecondTimeCalculatePoint(QList<QTr> TRlist, QList<QTr> *trlist, QList<QMe> list, double pressure,int i);
QList<QPoint> GetFuGaoList(QList<QPoint> list);
QPainterPath GetFuGaoPath(QList<QPoint> list);
QList<QWind> GetWindList(QTextStream *in, double &lon_unit, double &lat_unit, double &lon_start, double &lon_end, double &lat_start, double &lat_end, double &lon_num, double &lat_num);
QList<QList<float>> GetWindTKList(QTextStream* in);
void drawlonlat(bool ifdrawlonlat, int shadowtype, QGraphicsScene *scene);


#endif // MAINWINDOW_H
