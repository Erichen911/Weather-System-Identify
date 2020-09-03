#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace cv;
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ////初始化投影方式
    shadowtype = 2;
    ifshowlonlat = true;
    ////获取屏幕分辨率
    QDesktopWidget* desktopWidget=QApplication::desktop();
    QRect screenRect=desktopWidget->screenGeometry();
    screen_W = screenRect.width();
    screen_H = screenRect.height();


    ////将mainwindow.ui的内容给窗口
    ui->setupUi(this);
    ui->ifshowlonlat->setChecked(true);

    ////设置窗口打开大小为全屏
    this->resize(screen_W,screen_H);
    this->move(QPoint(-10,0));

    ////设置视口与场景
    scene=new QGraphicsScene(this);
    view = new MyView(scene,this);
    view->setFixedSize(screen_W-20,screen_H-120);
    ui->gridLayout->addWidget(view);
//    scene->setSceneRect(-180*100,-90*100,360*100,180*100); ////////有问题
    view->setBackgroundBrush(QColor(107,196,240,180));
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ////找到地图层列表，绘制地图层绘制地图层路径
    mappointlist1 = GetMapList(".\\World.dat");
    mappointlist3 = GetMapList(".\\ChinaProvince.dat");
    QPainterPath mappath1 = GetMapPath(mappointlist1);
    QPainterPath mappath3 = GetMapPath(mappointlist3);

    MyItem *map = new MyItem(mappath1,mappath3,1);
    scene->addItem(map);

    ////绘制经纬线层（1层）
    LonLatItem *lonlat = new LonLatItem(shadowtype,scene);
    scene->addItem(lonlat);

    SetViewCenter();
    view->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_3_triggered()
{
    QString filename=QFileDialog::getOpenFileName();
    if(filename!="")
    {
        QList<QQMe> heightlist = GetAnotherList(filename);
//        MyTextItem *textItem = new MyTextItem(list,ceng);
//        scene->addItem(textItem);
//        view->viewport()->update();
//        QCheckBox *RAB = new QCheckBox("图层 "+QString::number(ceng)+" "+filename,dockwidget);
//        RAB->setChecked(true);
//        QHBoxLayout *dockHbox = new QHBoxLayout();
//        QPushButton *pbutton = new QPushButton("关闭图层");
//        dockHbox->addWidget(RAB);
//        dockHbox->addWidget(pbutton);
//        docklayout->addLayout(dockHbox);
//        connect(RAB,SIGNAL(toggled(bool)),this,SLOT(radioBtnSlot()));

        ////找到选择文件的最大值和最小值
        QList<double> pressurelist;
        for(int i=0;i<heightlist.size();i++)
        {
            pressurelist<<heightlist.at(i).pressure;
        }
        qSort(pressurelist.begin(),pressurelist.end());
        int max=pressurelist.last();
        int min=pressurelist.first();
        int space=1;


        int lon_num_M = 33;
        int lat_num_M = 18;
        QList<QPainterPath> pathlist;
        for(int pressure = min;pressure<=max;pressure=pressure+space)
        {
            ////开始绘制等高线
            /// 1.区域网格化
            QList<QTr> trlist = makegrid(lat_num_M,lon_num_M);
    //          qDebug()<<heightlist.at(trlist.at(trlist.size()-1).pointid1-1).pressure; //检查留用
            /// 2.差值拟合

            /// 3.格值点的预处理
            for(int i=0;i<heightlist.size();i++)//预处理等于等压线的格点数据
            {
                if(heightlist[i].pressure==pressure)
                    heightlist[i].pressure=heightlist[i].pressure+0.1;
            }
    //        ///4.找到等压线穿过的三角网格
            QList<QTr> TRlist = findusegrid(heightlist,trlist,pressure);
    //                    qDebug()<<"等压线穿过的网格数量"<<TRlist.size();//检查留用

    //        ///5.边界等值线追踪
    //        ////这是一组曲线的path
    //        ////这是一条曲线的开始
            QList<QPointF> pointlist1;
    //        ///第一遍找出边界等值线
            for(int i=0;i<TRlist.size();i++)
            {
                pointlist1.clear();
                pointlist1 = FirstTimeCalculatePoint(TRlist,&trlist,heightlist,pressure,i,shadowtype);


                ////5.3平滑

                if(pointlist1.size() > 10)
                {
                    qDebug()<<pressure<<"The first time"<<pointlist1.size();
                    if(pressure==580)
                        qDebug()<<"580"<<pointlist1.size()<<pointlist1;
                    QList<QPointF> aftersmoothlist;
                    BCurve(pointlist1,aftersmoothlist,true);
                    aftersmoothlist.append(pointlist1.last());
                    aftersmoothlist.replace(0,pointlist1.first());
                    QList<QPointF> pointlist2;
                    QList<QPointF> aftersmoothlist2;
                    for(int i = 0 ; i <aftersmoothlist.size();i++)
                    {
                        QList<double> tmp;
                        tmp<<aftersmoothlist[i].x()<<aftersmoothlist[i].y();
                        QPointF point = ShadowXY(tmp,shadowtype);
                        pointlist2<<point;
                    }
                    BCurve(pointlist2,aftersmoothlist2,true);
                    aftersmoothlist2.append(pointlist2.last());
                    aftersmoothlist2.replace(0,pointlist2.first());
                    QPainterPath aftersmootpath = Smoothedpath(pointlist2,true);
                    pathlist.push_back(aftersmootpath);
                }
            }

            ///第二遍找出闭合等值线
            for(int i=0;i<TRlist.size();i++)
            {
                pointlist1.clear();
                pointlist1 = SecondTimeCalculatePoint(TRlist,&trlist,heightlist,pressure,i,shadowtype);
                if(pointlist1.size() > 10)
                {
                    qDebug()<<pressure<<"The second time"<<pointlist1.size();
                    QList<QPointF> aftersmoothlist;
                    BCurve(pointlist1,aftersmoothlist,true);
                    aftersmoothlist.append(pointlist1.last());
                    aftersmoothlist.append(pointlist1.first());
                    QList<QPointF> pointlist2;
                    QList<QPointF> aftersmoothlist2;
                    for(int i = 0 ; i <aftersmoothlist.size();i++)
                    {
                        QList<double> tmp;
                        tmp<<aftersmoothlist[i].x()<<aftersmoothlist[i].y();
                        QPointF point = ShadowXY(tmp,shadowtype);
                        pointlist2<<point;
                    }
                    BCurve(pointlist2,aftersmoothlist2,true);
                    aftersmoothlist2.append(pointlist2.last());
                    aftersmoothlist2.append(pointlist2.first());
                    QPainterPath aftersmootpath = Smoothedpath(aftersmoothlist2,true);
                    pathlist.push_back(aftersmootpath);
    //                QList<QPointF> aftersmoothlist;
    ////                BCurve(pointlist1,aftersmoothlist,false);
    //                QPainterPath aftersmootpath = Smoothedpath(pointlist1,false);
    //                pathlist.push_back(aftersmootpath);
                }
                pointlist1.clear();
            }
        }
        VoltageItem* item = new VoltageItem(pathlist);

        scene->addItem(item);
        view->viewport()->update();

        ColdToughItem* ctitem =  FindCT(heightlist,lon_num_M,lat_num_M,4,32,-4,80,QString());
        scene->addItem(ctitem);
        HotRidgeItem* hritem =FindHR(heightlist,lon_num_M,lat_num_M,4,32,-4,80,QString());
        scene->addItem(hritem);
    }






//////            ///*7*.绘制副高
//////            QPainterPath fugao;
//////            for(int i = 0;i<AllVoltagelist.size();i++)
//////            {
//////                QList<QPoint> fugaolist = GetFuGaoList(AllVoltagelist.at(i));
//////                qDebug()<<fugaolist.size();
//////                QPainterPath  fugaopath = GetFuGaoPath(fugaolist);
//////                fugao.addPath(fugaopath);
////////                qDebug()<<fugao;
//////            }
//////            FuGaoItem *fugaoitem = new FuGaoItem(fugao,ceng);
//////            scene->addItem(fugaoitem);
//////            view->viewport()->update();
}

////绘制底层地图
QList<QList<double>> GetMapList(QString location)
{
    QFile file(location);
    file.open(QIODevice::ReadOnly);
    if(file.exists()==false)
        qDebug()<<"Can't find"<<endl;
    QTextStream in(&file);
    in.readLine();
    in.readLine();
    double X,Y;
//    bool flag = false;
//    QPainterPath path;
//    QPen pen(Qt::black,0,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin);
//    painter->setPen(pen);
    QList<QList<double>> PointList;
    while(!in.atEnd())
    {
        QString line=in.readLine();
        QList<double> point;
        if(line.size()<=30)
        {
            QStringList list=line.split("   ");
            X=list[0].simplified().toDouble();
            Y=list[1].toDouble();
            if(Y>=-60)
            {
                point.append(X);
                point.append(Y);
                PointList.append(point);
            }
        }
        else
        {
            point.append(233);
            point.append(233);
            PointList.append(point);
        }
    }
    return PointList;
}

////找副高列表
QList<QPoint> GetFuGaoList(QList<QPoint> list)
{
    QList<QPoint> fugaolist;
    for(int i = 0;i<list.size();i++)
    {
        if((list.at(i).x()>=110*100-2)&&(list.at(i).x()<=150*100+2)&&(list.at(i).y()<=-12*100+2))
            fugaolist<<list.at(i);
    }
//    qDebug()<<fugaolist.first()<<fugaolist.last();
    if(fugaolist.first().x()>=150*100-5&&fugaolist.first().x()<=150*100+5&&fugaolist.last().y()>=-12*100-10&&fugaolist.last().y()<=-12*100+10)
        fugaolist.append(QPoint(150*100,-12*100));
//    qDebug()<<fugaolist.first()<<fugaolist.last();
    return fugaolist;
}
////绘制副高路径
QPainterPath GetFuGaoPath(QList<QPoint> list)
{
    QPainterPath path;
    list<<list.first();
    path.moveTo(list.first());
    for(int i =0;i<list.size();i++)
        path.lineTo(list.at(i));
    return path;
}


void MainWindow::on_action_2_triggered()
{
    int size = scene->items().size();
    for(int i = 0 ; i<size-2;i++)
    {
        scene->removeItem(scene->items().at(0));
    }
    qDebug()<<scene->items().size();
}


////选择等经纬度投影
void MainWindow::on_lonlatshadowtrigger_triggered()
{
    scene->clear();
    shadowtype = 1;

    QPainterPath mappath1 = GetMapPath(mappointlist1);
    QPainterPath mappath3 = GetMapPath(mappointlist3);

    MyItem *map = new MyItem(mappath1,mappath3,1);
    scene->addItem(map);

    LonLatItem *lonlat = new LonLatItem(shadowtype,scene);
    scene->addItem(lonlat);

    if(windlist.size()>0)
    {
        for(int i = 0;i<windlist.size();i++)
        {
            WindItem *winditem = new WindItem(windlist[i],ceng,view,shadowtype,QPen(QColor(QColor(0,0,0)),0,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin),0.1414,0.1);
            scene->addItem(winditem);
        }
    }

    SetViewCenter();
    ifshowlonlat = true;
    ui->ifshowlonlat->setChecked(true);
}
////选择兰波托投影
void MainWindow::on_lbtshadowtrigger_triggered()
{
    scene->clear();
    shadowtype = 2;
    QPainterPath mappath1 = GetMapPath(mappointlist1);
    QPainterPath mappath3 = GetMapPath(mappointlist3);

    MyItem *map = new MyItem(mappath1,mappath3,1);
    scene->addItem(map);
    LonLatItem *lonlat = new LonLatItem(shadowtype,scene);
    scene->addItem(lonlat);


    if(windlist.size()>0)
    {
        for(int i = 0;i<windlist.size();i++)
        {
            WindItem *winditem = new WindItem(windlist[i],ceng,view,shadowtype,QPen(QColor(QColor(0,0,0)),0,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin),0.1414,0.1);
            scene->addItem(winditem);
        }
    }

    SetViewCenter();
    ifshowlonlat = true;
    ui->ifshowlonlat->setChecked(true);
}
////选择麦卡托投影3
void MainWindow::on_mktshadowtrigger_triggered()
{
    scene->clear();
    shadowtype = 3;
    QPainterPath mappath1 = GetMapPath(mappointlist1);
    QPainterPath mappath3 = GetMapPath(mappointlist3);

    MyItem *map = new MyItem(mappath1,mappath3,1);
    scene->addItem(map);
    LonLatItem *lonlat = new LonLatItem(shadowtype,scene);
    scene->addItem(lonlat);


    if(windlist.size()>0)
    {
        for(int i = 0;i<windlist.size();i++)
        {
            WindItem *winditem = new WindItem(windlist[i],ceng,view,shadowtype,QPen(QColor(QColor(0,0,0)),0,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin),0.1414,0.1);
            scene->addItem(winditem);
        }
    }

    SetViewCenter();
    ifshowlonlat = true;
    ui->ifshowlonlat->setChecked(true);
}
////是否显示经纬线
void MainWindow::on_ifshowlonlat_triggered()
{
    ifshowlonlat = !ifshowlonlat;
    scene->items().at(scene->items().size()-2)->setOpacity(int(ifshowlonlat));
    view->viewport()->update();
}



QPainterPath MainWindow::GetMapPath(QList<QList<double> > pointlist)
{
    bool flag = false;
    QPainterPath path;
    for(int i = 0 ; i < pointlist.size() ; i++ )
    {
        QList<double> fenge;
        fenge<<233;
        fenge<<233;
        if(pointlist[i]==fenge)
        {
            flag = false;
            continue;
        }
        else
        {
            QPoint newpoint = ShadowXY(pointlist[i],shadowtype);
            if(flag)
                path.lineTo(newpoint);
            else
            {
                flag =true;
                path.moveTo(newpoint);
            }
        }
    }
    return path;
}

void MainWindow::SetViewCenter()
{
    QList<double> centerXY;
    centerXY<<110<<35;
    QPoint center = ShadowXY(centerXY,shadowtype);

    double delta = view->mscale() - 0.165818;
    while(fabs(delta)>0.0001)
    {
        if(delta<0)
        {
            view->zoomIn();
        }
        else if(delta>0)
        {
            view->zoomOut();
        }
        else
            break;
        delta = view->mscale() - 0.165818;
    }
    view->centerOn(center);
}



QList<QWind> GetWindList(QTextStream* in,double& lon_unit,double& lat_unit,double& lon_start,double& lon_end,double& lat_start,double& lat_end,double& lon_num,double& lat_num)
{
    QList<QWind> windvaluelist;
    in->readLine();
    QString linefirst = in->readLine();
    QList<QString> templist;
    templist = linefirst.split(" ");
    for(int i = 0 ; i<templist.size();i++)
    {
        if(templist[i]=="")
        {
            templist.removeAt(i);
            i--;
        }
    }
    qDebug()<<templist;

    lon_unit = templist[0].toDouble();
    lat_unit = templist[1].toDouble();
    lon_start = templist[2].toDouble();
    lon_end = templist[3].toDouble();
    lat_start = templist[4].toDouble();
    lat_end = templist[5].toDouble();
    lon_num = templist[6].toDouble();
    lat_num = templist[7].toDouble();
    vector<float> pointlist;
    QString line;
    QList<QString> list;
    while(!in->atEnd())
    {
        line = in->readLine();
        list = line.split(" ");

        for(int i = 0 ; i<list.size();i++)
        {
            if(list[i]=="")
            {
                list.removeAt(i);
                i--;
            }
        }
        for(int i = 0 ; i<list.size();i++)
            pointlist.push_back(list[i].toFloat());
    }
    int k = 0;
    for(double j = lat_start; j>=lat_end;j=j+lat_unit)
    {
        for(double i = lon_start; i <= lon_end ; i=i+lon_unit )
        {
            QWind temp;
            temp.wind=pointlist[k];
            k++;
            temp.x=i;
            temp.y=j;
            windvaluelist.append(temp);
        }
    }
    for(double j = lat_start; j>=lat_end;j=j+lat_unit)
    {
        for(double i = lon_start; i <= lon_end ; i=i+lon_unit )
        {
            QWind temp;
            temp.wind=pointlist[k];
            k++;
            temp.x=i;
            temp.y=j;
            windvaluelist.append(temp);
        }
    }
    return windvaluelist;
}
void MainWindow::on_readfile_triggered()
{
    ceng++;
    QString filename=QFileDialog::getOpenFileName();
    if(filename!="")
    {
        QFile file(filename);
        file.open(QIODevice::ReadOnly);
        QTextStream in(&file);
        QString linefirst = in.readLine();
        QList<QString> templist;
        templist = linefirst.split(" ");
        qDebug()<<"the"<<templist[1]<<"data";
        int datatype=templist[1].toInt();
        double lon_unit;
        double lat_unit;
        double lon_start;
        double lon_end;
        double lat_start;
        double lat_end;
        double lon_num;
        double lat_num;
        switch (datatype) {
        case 2:
        {
            QList<QList<float>> winddata = GetWindTKList(&in);
            Find_TKJL(winddata);
            break;
        }
        case 4:
        {
            The4thDataDeal(filename);
            break;
        }
        case 11:
        {
            QList<QWind> windvaluelist = GetWindList(&in,lon_unit,lat_unit,lon_start,lon_end,lat_start,lat_end,lon_num,lat_num);
            The11thDataDeal(windvaluelist,lon_unit,lat_unit,lon_start,lon_end,lat_start,lat_end,lon_num,lat_num);
            break;
        }
        default:
            break;
        }
    }
}


////***************************第11类数据处理**************************

void MainWindow::The11thDataDeal(QList<QWind> windvaluelist,double lon_unit,double lat_unit,double lon_start,double lon_end,double lat_start,double lat_end,double lon_num,double lat_num)
{
    int mid = windvaluelist.size()/2;

    //找风向，风速
    QList<QWind> windvaluelistU =windvaluelist.mid(0,mid);
    QList<QWind> windvaluelistV =windvaluelist.mid(mid,windvaluelist.size());
    QList<QWind> winddirectionlist;
    QList<QWind> windpowerlist;
    for(int i = 0;i<windvaluelistU.size();i++)
    {
        QWind winddirection;
        winddirection.y = windvaluelistU.at(i).y;
        winddirection.x = windvaluelistU.at(i).x;
        winddirection.wind =qAtan2(windvaluelistV.at(i).wind,windvaluelistU.at(i).wind);

        QWind windpower;
        windpower.y = windvaluelistU.at(i).y;
        windpower.x = windvaluelistU.at(i).x;
        windpower.wind = sqrt(pow(windvaluelistV.at(i).wind,2)+pow(windvaluelistU.at(i).wind,2));

        winddirectionlist<<winddirection;
        windpowerlist<<windpower;
    }

//    qDebug()<<lon_start<<lon_num<<lon_unit;
//    qDebug()<<lat_start<<lat_num<<lat_unit;
    QXItem *qxitem = FindQX(windvaluelistU,windvaluelistV,lon_num,lat_num,lon_unit,lon_start,lat_unit,lat_start,QString());
    WindDKItem *dkjlitem = FindDKJL(windvaluelistU,windvaluelistV,winddirectionlist,windpowerlist,lat_num,lon_num,lon_unit,lon_start,lat_start,QString());
    QBItem *qbitem = FindQB(windvaluelistU,windvaluelistV,lon_start,lat_start,lon_num,lat_num,lon_unit,lat_unit,QString());

    WindItem *winditem = new WindItem(winddirectionlist,ceng,view,shadowtype,QPen(QColor(QColor(0,0,0)),0,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin),0.1414,0.1);
    WindTextItem *windTextItem = new WindTextItem(windpowerlist,ceng,view,shadowtype);
    scene->addItem(qbitem);
    scene->addItem(dkjlitem);
    scene->addItem(qxitem);
    scene->addItem(winditem);
    scene->addItem(windTextItem);
    view->viewport()->update();

    //添加选择窗口
    QWindDialog* window = new QWindDialog(scene);
    window->setWindowTitle(QString::fromLocal8Bit("第二类数据"));
    window->show();
}

QXItem *MainWindow::FindQX(QList<QWind> windvaluelistU, QList<QWind> windvaluelistV,double lon_num,double lat_num,double lon_unit,double lon_start,double lat_unit,double lat_start,QString savename)
{

    ////************************************************////
//    QFile file(savename);
//    file.open(QFile::Append);
//    QTextStream write(&file);
//    write<<"\tcyclone\n";
    ////************************************************////





    vector<float> u,v;
    for(int i = 0; i <windvaluelistU.size();i++)
    {
        u.push_back(windvaluelistU[i].wind);
        v.push_back(windvaluelistV[i].wind);
    }
    int Sizea;

    vector<float> xy = WindTestPoint(u,v,lon_num,lat_num,lon_unit,lon_start,lat_unit,lat_start,Sizea);
    QList<QPointF> pointlist;
    QList<float> r;
    for(int i = 0 ; i < Sizea*4 ; i = i+4)
    {
        if(xy[i+2]==1&&xy[1+3]>=2)
        {
            QList<double> point;
            point<<xy[i]<<xy[i+1];
//            write<<"\t"<<xy[i]<<"\t"<<xy[i+1]<<"\t"<<xy[i+3]<<"\n";
            QPointF temp = ShadowXY(point,shadowtype);
            pointlist<<temp;
            r.push_back(xy[i+3]);
        }
    }
//    qDebug()<<r;

    QXItem* item = new QXItem(pointlist,1,shadowtype,r);
//    file.close();

    return item;
}
WindDKItem *MainWindow::FindDKJL(QList<QWind> windvaluelistU, QList<QWind> windvaluelistV, QList<QWind> winddirectionlist, QList<QWind> windpowerlist, double lat_num, double lon_num, double lon_unit, double lon_start, double lat_start, QString savename)
{
    ////************************************************////
//    QFile file(savename);
//    file.open(QFile::Append);
//    QTextStream write(&file);
//    write<<"\tlow jet\n";
    ////************************************************////



    vector<float> u;
    vector<float> v;
    for(int i = 0;i < windvaluelistU.size();i++)
    {
        u.push_back(windvaluelistU[i].wind);
        v.push_back(windvaluelistV[i].wind);
    }
    Mat pic_u, pic_v;
    pic_u = zoom_2(u, lat_num, lon_num,lon_unit);
    pic_v = zoom_2(v, lat_num, lon_num,lon_unit);
//    qDebug()<<"finish step 1";
    //角度
    Mat angle(pic_u.size(), CV_32F, Scalar(0));
    //风向
    Mat speed(pic_u.size(), CV_32F, Scalar(0));
    //西南风
    Mat ws(pic_u.size(), CV_8U, Scalar(0));
    //东南风
    Mat es(pic_u.size(), CV_8U, Scalar(0));

    for(int i = 0 ; i <lat_num ; i++)
    {
        float *data1 = angle.ptr<float>(i);
        float *data2 = speed.ptr<float>(i);
        for(int j = 0 ; j < lon_num ; j++)
        {
            data1[j] = winddirectionlist[lon_num*i+j].wind;
            data2[j] = windpowerlist[lon_num*i+j].wind;
        }
    }
    for (int j = 0; j < lat_num; j++)
    {
        float*data1 = speed.ptr<float>(j);
        float*data2 = angle.ptr<float>(j);
        uchar*data3 = ws.ptr<uchar>(j);
        uchar*data4 = es.ptr<uchar>(j);
        for (int i = 0; i < lon_num; i++)
        {
            if (data1[i] >= 12 && data2[i] > 0 && data2[i] <= 1.57)
            {
                data3[i] = 255;
            }
            if (data1[i] >= 12 && data2[i] <= 3.14 && data2[i] > 1.57)
            {
                data4[i] = 255;
            }
        }
    }

    for(int i = 0 ; i < lat_num ; i++)
    {
        float*data1 = angle.ptr<float>(i);
        for(int j = 0 ; j < lon_num ;j++)
        {
            data1[j] = data1[j]/M_PI*180;
        }
    }
//    qDebug()<<"finish step 2";
    vector<vector<Point>> espoint;
    vector<vector<Point>> wspoint;

    vector<vector<Point>> escontour;
    vector<vector<Point>> wscontour;

    QList<QList<QPointF>> espointOnTheMap;
    QList<QList<QPointF>> wspointOnTheMap;

    QList<QList<QPointF>> escontourOnTheMap;
    QList<QList<QPointF>> wscontourOnTheMap;

    Mat element = getStructuringElement(MORPH_RECT, Size(2, 2));
//    morphologyEx(es, es, MORPH_OPEN, element);
    morphologyEx(es, es, MORPH_OPEN, element);
    morphologyEx(es, es, CV_MOP_CLOSE, element);
//    morphologyEx(ws, ws, MORPH_OPEN, element);
    morphologyEx(ws, ws, MORPH_OPEN, element);
    morphologyEx(ws, ws, CV_MOP_CLOSE, element);


////    qDebug()<<"finish step 3";
    espoint = yet(es,-1,speed, angle, pic_u.rows,pic_u.cols,&escontour);
    wspoint = yet(ws, 1,speed, angle, pic_u.rows,pic_u.cols,&wscontour);
////    qDebug()<<"finish step 4";
////    for(int i = 0 ;i<wspoint.size();i++)
////    {
////        for(int j = 0 ; j <wspoint[i].size();j++)
////        {
////            write<<"\t"<<wspoint[i][j].x<<"\t"<<wspoint[i][j].y;
////        }
////        write<<"\n";
////    }

    vector<vector<Point>> spoint, spoint1, spoint2;
    int nl = pic_u.rows;
    int nc = pic_u.cols;
    spoint = connects(wspoint, espoint);
////    qDebug()<<"finish step 5";
    spoint1 = smooth(spoint, nl, nc);
    spoint.clear();
    spoint = smooth(spoint1, nl, nc);
//    qDebug()<<"finish step 6";
//    spoint = smooth(spoint2, nl, nc);
    Point b;
        //新加，适当延长两端端点,最终结果存入spoint
    for (int j = 0; j < spoint.size(); j++)
    {
        b.x = spoint[j][0].x- 0.5*(spoint[j][1].x- spoint[j][0].x) - 0.5*(spoint[j][2].x - spoint[j][1].x);
        b.y = spoint[j][0].y-0.5*(spoint[j][1].y - spoint[j][0].y) - 0.5*(spoint[j][2].y - spoint[j][1].y);
        spoint[j].insert(spoint[j].begin(),b);
        b.x = spoint[j][spoint[j].size() - 1].x+ 0.5*(spoint[j][spoint[j].size() - 1].x- spoint[j][spoint[j].size() - 2].x) + 0.5*(spoint[j][spoint[j].size() - 2].x - spoint[j][spoint[j].size() - 3].x);
        b.y = spoint[j][spoint[j].size() - 1].y+ 0.5*(spoint[j][spoint[j].size() - 1].y - spoint[j][spoint[j].size() - 2].y) + 0.5*(spoint[j][spoint[j].size() - 2].y - spoint[j][spoint[j].size() - 3].y);
        spoint[j].push_back(b);
    }
    ////位置矫正
    for(int i = 0 ; i < spoint.size() ; i++)
    {
        for(int j = 0 ; j < spoint[i].size() ; j++)
        {
            spoint[i][j].x = spoint[i][j].x+3;
        }
    }

    VectorXYToMapXY(spoint,&wspointOnTheMap,shadowtype,true,lon_start,lat_start);
    VectorXYToMapXY(espoint,&espointOnTheMap,shadowtype,true,lon_start,lat_start);
    VectorXYToMapXY(wscontour,&wscontourOnTheMap,shadowtype,false,lon_start,lat_start);
    VectorXYToMapXY(escontour,&escontourOnTheMap,shadowtype,false,lon_start,lat_start);


    QList<QPolygonF> wscontourpolygonlist = LineListToPolygon(wscontourOnTheMap);
    QList<QPolygonF> escontourpolygonlist = LineListToPolygon(escontourOnTheMap);

    wscontourpolygonlist.append(escontourpolygonlist);

//    WindDKItem *dkjlItem;
    WindDKItem *dkjlItem = new WindDKItem(wscontourpolygonlist,wspointOnTheMap,1);
//    file.close();
    return dkjlItem;

}
QBItem *MainWindow::FindQB(QList<QWind> windvaluelistU, QList<QWind> windvaluelistV, double lon_start, double lat_start, double lon_num, double lat_num, double lon_unit, double lat_unit,QString savename)
{

    ////************************************************////
//    QFile file(savename);
//    file.open(QFile::Append);
//    QTextStream write(&file);
//    write<<"\tshear\n";
    ////************************************************////



    vector<double> u,v;
    for(int i = 0; i <windvaluelistU.size();i++)
    {
        u.push_back(windvaluelistU[i].wind);
        v.push_back(windvaluelistV[i].wind);
    }
    double* upoint = &u[0];
    double* vpoint = &v[0];
    vector<vector<Point2f>> qbx = qiebianxian(upoint,vpoint,lon_start,lat_start,lon_num,lat_num,lon_unit,lat_unit,1);

    ////********cv::Point2f->QPointF********////
    QList<QList<QPointF>> qbx_change;
    for(int i = 0 ; i< qbx.size();i++)
    {
        QList<QPointF> qbx_change_one;
        for(int j = 0 ; j < qbx[i].size() ;j++)
        {
            QPointF point(qbx[i][j].x,qbx[i][j].y);
            qbx_change_one.push_back(point);
        }
        qbx_change.push_back(qbx_change_one);
    }
    ////************************************////
    QList<QPainterPath> pathlist;
    for(int i = 0 ; i < qbx_change.size() ; i++)
    {
        qbx_change[i].removeLast();
        QPainterPath path;
        for(int j = 0 ; j < qbx_change[i].size() ; j++)
        {
            QList<double> point;
            point<<qbx_change[i][j].x()<<qbx_change[i][j].y();
            QPointF temp = ShadowXY(point,shadowtype);
            if(j==0)
                path.moveTo(temp);
            else
                path.lineTo(temp);
        }
        pathlist.push_back(path);
    }
//    int count = 0 ;
//    int fontsize = qbx[count];
//    count++;
//    qDebug()<<fontsize;
//    for(int i = 0 ; i < fontsize ; i++)
//    {
//        int linesize = qbx[count];
//        count++;
//        qDebug()<<i<<"linesize"<<linesize;
//        for(int j = 0 ; j < linesize ; j++)
//        {
//            QList<double> point;
//            point<<qbx[count];
//            count++;
//            point<<qbx[count];
//            count++;
//            QPointF temp = ShadowXY(point,shadowtype);
//            if(j==0)
//            {
//                path.moveTo(temp);
//            }
//            else
//                path.lineTo(temp);
//        }
//    }
//    QList<QPainterPath> qiebian;
//    int count=0;
//    int coldfontsize = qbx[count];
//    count++;
//    for(int i = 0 ; i < coldfontsize ;i++)
//    {
//        int pointnumber = qbx[count];
////        qDebug()<<pointnumber;
//        count++;
//        QPainterPath path;
//        for(int j = 0 ; j < pointnumber;j=j++)
//        {
//            QList<double> point;
//            point<<qbx[count];
//            count++;
////            write<<"\t"<<qbx[count];
//            point<<qbx[count];
////            write<<"\t"<<qbx[count];
//            count++;
//            QPointF temp = ShadowXY(point,shadowtype);
//            if(j==0)
//            {
//                path.moveTo(temp);
//            }
//            else
//                path.lineTo(temp);
////            if(j==0)
////                write<<"\n";
//        }
//        qiebian<<path;
//    }
//    write<<"\n";
//    qDebug()<<qbx[count]<<qbx[count+1]<<qbx[count+2];
//    QList<QPainterPath> qiebian;
//    for(int i = 0 ; i<2 ; i++)
//    {
//        QPainterPath path;
//        path.moveTo(134,54);
//        path.lineTo(135,54);
//        path.lineTo(132,34);
//        qiebian.push_back(path);
//    }
    QBItem* item = new QBItem(pathlist,1,shadowtype);
//    file.close();
    return item;
}

ColdToughItem *MainWindow::FindCT(QList<QQMe> valueQ, int lon_num, int lat_num, int lon_unit, int lon_start, int lat_unit, int lat_start, QString savename)
{
    ////************************************************////
//    QFile file(savename);
//    file.open(QFile::Append);
//    QTextStream write(&file);
//    write<<"\tcoldridge\n";
    ////************************************************////


    ////*********冷槽暖脊test********///
    vector<float> value;
    for(int i = 0 ; i < valueQ.size() ; i++ )
    {
        value.push_back(valueQ[i].pressure);
    }
    float *valuepoint = &value[0];
    int SizeNume;
    float *cold = coldtrough(valuepoint,lat_num,lon_num,lon_unit,lon_start,lat_unit,lat_start,SizeNume,scene,shadowtype);
//    qDebug()<<"coldSize"<<SizeNume;
    bool start = true;
    bool ifcontinue = false;
    bool ifmove = true;
    QPainterPath path;
    int tempSize;

    for(int i = 0 ; i<SizeNume;i++)
    {
        if(ifcontinue)
        {
            ifcontinue =false;
            continue;
        }

        if(start)
        {
            tempSize = cold[i];
            start = false;
            ifmove = true;
        }
        else
        {
            QList<double> temppoint;
            temppoint<<cold[i]<<cold[i+1];
//            write<<"\t"<<cold[i]<<"\t"<<cold[i+1];
            QPointF temp = ShadowXY(temppoint,shadowtype);
            if(ifmove)
            {
                path.moveTo(temp);
                ifmove = false;
            }
            else
                path.lineTo(temp);
            ifcontinue = true;
            tempSize--;
            if(tempSize==0)
            {
//                write<<"\n";
                start = true;
            }
        }
    }
    ColdToughItem* item = new ColdToughItem(path,shadowtype);
//    file.close();
    return item;
}

HotRidgeItem *MainWindow::FindHR(QList<QQMe> valueQ, int lon_num, int lat_num, int lon_unit, int lon_start, int lat_unit, int lat_start, QString savename)
{
    ////************************************************////
//    QFile file(savename);
//    file.open(QFile::Append);
//    QTextStream write(&file);
//    write<<"\thotridge\n";
    ////************************************************////


    vector<float> value;
    for(int i = 0 ; i < valueQ.size() ; i++ )
    {
        value.push_back(valueQ[i].pressure);
    }
    float *valuepoint = &value[0];
    int SizeNume;
    float *hot = warmridge(valuepoint,lat_num,lon_num,lon_unit,lon_start,lat_unit,lat_start,SizeNume,scene,shadowtype);
//    qDebug()<<"hotSize"<<SizeNume;
    bool start = true;
    bool ifcontinue = false;
    bool ifmove = true;
    QPainterPath path;
    int tempSize;

    for(int i = 0 ; i<SizeNume;i++)
    {
        if(ifcontinue)
        {
            ifcontinue =false;
            continue;
        }

        if(start)
        {
            tempSize = hot[i];
            start = false;
            ifmove = true;
        }
        else
        {
            QList<double> temppoint;
            temppoint<<hot[i]<<hot[i+1];
//            write<<"\t"<<hot[i]<<"\t"<<hot[i+1];
            QPointF temp = ShadowXY(temppoint,shadowtype);
            if(ifmove)
            {
                path.moveTo(temp);
                ifmove=false;
            }
            else
                path.lineTo(temp);
            ifcontinue = true;
            tempSize--;
            if(tempSize==0)
            {
//                write<<"\n";
                start = true;
            }
        }
    }
//    QPainterPath path;
//    path.moveTo(0,0);
//    path.lineTo(0,1);
    HotRidgeItem* item = new HotRidgeItem(path,shadowtype);
//    qDebug()<<"hotridge"<<path;
//    file.close();
    return item;
}



////***************************第二类数据处理**************************

QList<QList<float>> GetWindTKList(QTextStream* in)
{
    in->readLine();
    QList<float> fengxiang;
    QList<float> fengsu;
    QList<float> jingdu;
    QList<float> weidu;
    while (!in->atEnd())
    {
       QString line = in->readLine().simplified();
       QList<QString> list = line.split(" ");
       //       qDebug()<<list[1]<<list[2]<<list[8]<<list[9];
       if(list[8].toFloat() <= 360)
       {
            jingdu.append(list[1].toFloat());
            weidu.append(list[2].toFloat());
            fengxiang.append(list[8].toFloat());
            fengsu.append(list[9].toFloat());
       }
    }
    QList<QList<float>> winddata;
    winddata<<jingdu<<weidu<<fengxiang<<fengsu;
    return winddata;
}

void MainWindow::Find_TKJL(QList<QList<float>> winddata)
{
    float jingdu[1000];
    float weidu[1000];
    float fengxiang[1000];
    float fengsu[1000];
    vector<vector<Point2f>> Linelist;
    int num = winddata[0].size();
    for(int i = 0 ; i < num ; i++)
    {
        fengxiang[i] = winddata[2][i];
        fengsu[i] = winddata[3][i];
        jingdu[i] = winddata[0][i];
        weidu[i] = winddata[1][i];
    }
    QList<QWind> windpowerlist;
    QList<QWind> winddirectionlist;
    for(int i = 0 ; i < num ; i++)
    {
        if(weidu[i]>=4 && weidu[i] <= 54 && jingdu[i]>=73 && jingdu[i]<=135)
        {
            QWind powertemp;
            QWind directiontemp;
            powertemp.wind = fengsu[i];
            powertemp.x = jingdu[i];
            powertemp.y = weidu[i];
            directiontemp.x = jingdu[i];
            directiontemp.y = weidu[i];
            directiontemp.wind = -qDegreesToRadians(fengxiang[i]+90);
            windpowerlist<<powertemp;
            winddirectionlist<<directiontemp;
        }
    }



    Linelist = dikongjiliu(fengxiang,fengsu,jingdu,weidu,num);
    for(int i = 0 ; i < Linelist.size() ; i++)
    {
        QPainterPath path;
        for(int j = 0 ; j < Linelist[i].size() ; j++)
        {
            QList<double> temp;
            temp<<Linelist[i].at(j).x;
            temp<<Linelist[i].at(j).y;
            QPoint point=ShadowXY(temp,shadowtype);
            if(j == 0)
                path.moveTo(point);
            else
                path.lineTo(point);
        }
//        qDebug()<<path;

        scene->addPath(path,QPen(Qt::red,30));
        view->viewport()->update();
    }


    WindItem *winditem = new WindItem(winddirectionlist,ceng,view,shadowtype,QPen(QColor(QColor(255,165,0,255)),0,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin),1.414,0.7);
    WindTextItem *windTextItem = new WindTextItem(windpowerlist,ceng,view,shadowtype);
    scene->addItem(winditem);
    scene->addItem(windTextItem);
}


////*****************************************************************

////***************************第四类数据处理**************************
void MainWindow::The4thDataDeal(QString filename)
{
    QList<QQMe> heightlist = GetAnotherList(filename);

    ////找到选择文件的最大值和最小值
    QList<double> pressurelist;
    for(int i=0;i<heightlist.size();i++)
    {
        pressurelist<<heightlist.at(i).pressure;
    }
    qSort(pressurelist.begin(),pressurelist.end());
    int max=pressurelist.last();
    int min=pressurelist.first();
    int space=1;


    int lon_num_M = 33;
    int lat_num_M = 18;
    QList<QPainterPath> pathlist;
    for(int pressure = min;pressure<=max;pressure=pressure+space)
    {
        ////开始绘制等高线
        /// 1.区域网格化
        QList<QTr> trlist = makegrid(lat_num_M,lon_num_M);
//          qDebug()<<heightlist.at(trlist.at(trlist.size()-1).pointid1-1).pressure; //检查留用
        /// 2.差值拟合

        /// 3.格值点的预处理
        for(int i=0;i<heightlist.size();i++)//预处理等于等压线的格点数据
        {
            if(heightlist[i].pressure==pressure)
                heightlist[i].pressure=heightlist[i].pressure+0.1;
        }
//        ///4.找到等压线穿过的三角网格
        QList<QTr> TRlist = findusegrid(heightlist,trlist,pressure);
//                    qDebug()<<"等压线穿过的网格数量"<<TRlist.size();//检查留用

//        ///5.边界等值线追踪
//        ////这是一组曲线的path
//        ////这是一条曲线的开始
        QList<QPointF> pointlist1;
//        ///第一遍找出边界等值线
        for(int i=0;i<TRlist.size();i++)
        {
            pointlist1.clear();
            pointlist1 = FirstTimeCalculatePoint(TRlist,&trlist,heightlist,pressure,i,shadowtype);


            ////5.3平滑

            if(pointlist1.size() > 10)
            {
                QList<QPointF> aftersmoothlist;
                BCurve(pointlist1,aftersmoothlist,true);
                aftersmoothlist.append(pointlist1.last());
                aftersmoothlist.replace(0,pointlist1.first());
                QList<QPointF> pointlist2;
                QList<QPointF> aftersmoothlist2;
                for(int i = 0 ; i <aftersmoothlist.size();i++)
                {
                    QList<double> tmp;
                    tmp<<aftersmoothlist[i].x()<<aftersmoothlist[i].y();
                    QPointF point = ShadowXY(tmp,shadowtype);
                    pointlist2<<point;
                }
                BCurve(pointlist2,aftersmoothlist2,true);
                aftersmoothlist2.append(pointlist2.last());
                aftersmoothlist2.replace(0,pointlist2.first());
                QPainterPath aftersmootpath = Smoothedpath(pointlist2,true);
                pathlist.push_back(aftersmootpath);
            }
        }

        ///第二遍找出闭合等值线
        for(int i=0;i<TRlist.size();i++)
        {
            pointlist1.clear();
            pointlist1 = SecondTimeCalculatePoint(TRlist,&trlist,heightlist,pressure,i,shadowtype);
            if(pointlist1.size() > 10)
            {
                QList<QPointF> aftersmoothlist;
                BCurve(pointlist1,aftersmoothlist,true);
                aftersmoothlist.append(pointlist1.last());
                aftersmoothlist.append(pointlist1.first());
                QList<QPointF> pointlist2;
                QList<QPointF> aftersmoothlist2;
                for(int i = 0 ; i <aftersmoothlist.size();i++)
                {
                    QList<double> tmp;
                    tmp<<aftersmoothlist[i].x()<<aftersmoothlist[i].y();
                    QPointF point = ShadowXY(tmp,shadowtype);
                    pointlist2<<point;
                }
                BCurve(pointlist2,aftersmoothlist2,true);
                aftersmoothlist2.append(pointlist2.last());
                aftersmoothlist2.append(pointlist2.first());
                QPainterPath aftersmootpath = Smoothedpath(aftersmoothlist2,true);
                pathlist.push_back(aftersmootpath);
            }
            pointlist1.clear();
        }
    }
    VoltageItem* item = new VoltageItem(pathlist);

    scene->addItem(item);

    ColdToughItem* ctitem =FindCT(heightlist,lon_num_M,lat_num_M,4,32,-4,80,QString());
    scene->addItem(ctitem);
    HotRidgeItem* hritem =FindHR(heightlist,lon_num_M,lat_num_M,4,32,-4,80,QString());
    scene->addItem(hritem);
}
////*****************************************************************

////***************************实况数据处理**************************
void MainWindow::on_get_shikuang_trigger_triggered()
{
    vector<float> cengshu;
    vector<float> lontitude;
    vector<float> lattitude;
    vector<vector<float>> U;
    vector<vector<float>> V;
    vector<vector<float>> HGT025;
    vector<vector<float>> HGT;
    int lon_num;
    int lat_num;
    float lon_unit;
    float lat_unit;
    float lon_start;
    float lat_start;
    float lon_end;
    float lat_end;
    QString filename = QFileDialog::getOpenFileName();
//    QTextCodec *code = QTextCodec::codecForName("gb18030");
//    std::string name = code->fromUnicode(filename).data();
    //    QString filename = "D:\\work\\ppj\\MODP_TJQX_ECNC_ADCN_2017062500-132.nc";
    if(filename!="")
    {
        GetShikuangData(filename,&cengshu,&lontitude,&lattitude,&U,&V,&HGT025,&HGT,lon_num,lat_num,lon_start,lat_start,lon_unit,lat_unit);

        QList<QList<QWind>> thelistofwindvaluelist;
        QList<QList<QQMe>> thelistofheightlist025;
        QList<QList<QQMe>> thelistofheightlist;
        //    qDebug()<<lat_start<<lat_unit<<lat_end;
        //    qDebug()<<lon_start<<lon_unit<<lon_end;

        for(int i = 0 ; i < cengshu.size() ; i++)
        {
            int C = i;
            vector<float> UX = U[C];
            vector<float> VX = V[C];
            vector<float> HGTX = HGT[C];
            vector<float> HGTX025 = HGT025[C];
            //        qDebug()<<"HGTX025"<<i<<HGTX025.size();
            //        qDebug()<<UX.size()<<VX.size()<<lontitude.size()<<lattitude.size();
            QList<QWind> windvaluelist;
            int k = 0;
            for(double j = lat_start; j >=lat_start + (lat_num-1)*lat_unit;j=j+lat_unit)
            {
                for(double i = lon_start; i <= lon_start + (lon_num-1)*lon_unit ; i=i+lon_unit )
                {
                    QWind temp;
                    temp.wind=UX[k];
                    k++;
                    temp.x=i;
                    temp.y=j;
                    windvaluelist.append(temp);
                }
            }
            k=0;
            for(double j = lat_start; j>=lat_start + (lat_num-1)*lat_unit;j=j+lat_unit)
            {
                for(double i = lon_start; i <= lon_start + (lon_num-1)*lon_unit ; i=i+lon_unit )
                {
                    QWind temp;
                    temp.wind=VX[k];
                    k++;
                    temp.x=i;
                    temp.y=j;
                    windvaluelist.append(temp);
                }
            }

            thelistofwindvaluelist<<windvaluelist;

            ////********处理高度场信息********////
            QList<QQMe> heightlist;
            for(double j = 17; j>=0;j--)
            {
                for(double k = 0; k < 23 ; k++)
                {
                    QQMe temp;
                    temp.pointid=int((17-j)*23+k);
                    temp.x=k*lon_unit*16+lon_start;
                    temp.y=j*lat_unit*16+lat_start;
                    temp.pressure=int(HGTX[j*23+k])/10.0;
                    heightlist.append(temp);
                }
            }
            //        if(i == 10)
            //            qDebug()<<heightlist[0].pointid<<heightlist[0].pressure<<heightlist[0].x<<heightlist[0].y;
            thelistofheightlist<<heightlist;

            ////********处理高度场信息2********////
            QList<QQMe> heightlist025;
            for(double j = 70; j>=0;j--)
            {
                for(double k = 0; k < 91 ; k++)
                {
                    QQMe temp025;
                    temp025.pointid=int(j*91+k);
                    temp025.x=k*lon_unit*4+lon_start;
                    temp025.y=j*lat_unit*4+lat_start;
                    temp025.pressure=int(HGTX025[j*91+k])/10.0;
                    heightlist025.append(temp025);
                }
            }
            thelistofheightlist025<<heightlist025;
            //        qDebug()<<"after"<<heightlist025.size();

            ////****************************////
        }

        lon_end = lon_start + (lon_num-1)*lon_unit;
        lat_end = lat_start + (lat_num-1)*lat_unit;
        ////**********数据显示********////
        QString none;
        TheShikuangDataDeal(thelistofwindvaluelist,thelistofheightlist,thelistofheightlist,lon_unit,lat_unit,lon_start,lon_end,lat_start,lat_end,lon_num,lat_num,none,none);
    }
}

void MainWindow::GetShikuangData(QString filename,vector<float> *cengshu, vector<float> *lontitude, vector<float> *lattitude, vector<vector<float>> *Uwind, vector<vector<float>> *Vwind,vector<vector<float>> *HIGHT025,vector<vector<float>> *HIGHT,int& lon_num,int& lat_num,float& lon_start,float& lat_start,float& lon_unit,float& lat_unit)
{
    string fn = filename.toStdString();
    NcFile file(/*"D:\\work\\ppj\\MODP_TJQX_ECNC_ADCN_2017062500-132.nc"*/fn,NcFile::read);
    multimap<string,NcDim> maps = file.getDims();
    multimap<string,NcDim>::iterator iter;
    iter= maps.find("lv_ISBL2");
    NcDim dim_ISBL2 = iter->second;
    iter = maps.find("g0_lat_3");
    NcDim dim_lat = iter->second;
    iter = maps.find("g0_lon_4");
    NcDim dim_lon = iter->second;
    int ceng_count = dim_ISBL2.getSize();
    int lat_count = dim_lat.getSize();
    int lon_count = dim_lon.getSize();
    ////********统计经纬度个数********////
    lon_num = lon_count;
    lat_num = lat_count;
    ////***************************////
    NcVar ISBL2 = file.getVar("lv_ISBL2");
    NcVar lonvar = file.getVar("g0_lon_4");
    NcVar latvar = file.getVar("g0_lat_3");
    NcVar Uvar = file.getVar("EDA");
    NcVar Vvar = file.getVar("VV");
    NcVar HGTvar = file.getVar("HGT");
    NcVar Qvar = file.getVar("Q");
    NcVar TMPvar = file.getVar("TMP");

    double *point_ceng = new double[ceng_count];
    double *point_lon = new double[lon_count];
    double *point_lat = new double[lat_count];
    double *point_U = new double [ceng_count*lon_count*lat_count];
    double *point_V = new double [ceng_count*lon_count*lat_count];
    double *point_HGT = new double [ceng_count*lon_count*lat_count];
    double *point_Q = new double [ceng_count*lon_count*lat_count];
    double *point_TMP = new double [ceng_count*lon_count*lat_count];

    ISBL2.getVar(point_ceng);
    lonvar.getVar(point_lon);
    latvar.getVar(point_lat);
    Uvar.getVar(point_U);
    Vvar.getVar(point_V);
    HGTvar.getVar(point_HGT);
    Qvar.getVar(point_Q);
    TMPvar.getVar(point_TMP);

    vector<float> U;
    vector<float> V;
    vector<float> Q;
    vector<float> HGT;
    vector<float> TMP;


    for(int i = 0 ; i< ceng_count ; i++)
    {
        cengshu->push_back(point_ceng[i]);
    }
    for(int i = 0 ; i< lon_count ; i++)
    {
        lontitude->push_back(point_lon[i]);
    }
    for(int i = 0 ; i< lat_count ; i++)
    {
        lattitude->push_back(point_lat[i]);
    }
    for(int i = 0 ; i< ceng_count*lon_count*lat_count ; i++)
    {
        U.push_back(point_U[i]);
    }
    for(int i = 0 ; i< ceng_count*lon_count*lat_count ; i++)
    {
        V.push_back(point_V[i]);
    }
    for(int i = 0 ; i< ceng_count*lon_count*lat_count ; i++)
    {
        Q.push_back(point_Q[i]);
    }
    for(int i = 0 ; i< ceng_count*lon_count*lat_count ; i++)
    {
        HGT.push_back(point_HGT[i]);
    }
    for(int i = 0 ; i< ceng_count*lon_count*lat_count ; i++)
    {
        TMP.push_back(point_TMP[i]);
    }
    dim_ISBL2.~NcDim();
    dim_lat.~NcDim();
    dim_lon.~NcDim();
    ISBL2.~NcVar();
    lonvar.~NcVar();
    latvar.~NcVar();
    Uvar.~NcVar();
    Vvar.~NcVar();
    HGTvar.~NcVar();
    Qvar.~NcVar();
    TMPvar.~NcVar();
    file.close();
    ////********统计经纬度的分辨率与起止点********////
    lon_start = lontitude->at(0);
    lat_start = lattitude->at(0);
    lon_unit = lontitude->at(1)-lontitude->at(0);
    lat_unit = lattitude->at(1)-lattitude->at(0);
    ////*************************************////
//    qDebug()<<HGT[0+1014410]<<HGT[360+1014410]<<HGT[101080+1014410]<<HGT[101440+1014410];
//    qDebug()<<lat_unit<<lon_unit;
    ////********颠倒纬度********////
    vector<float> forU;
    vector<float> forV;
    vector<float> forHGT;
    vector<vector<float>> testU;
    vector<vector<float>> testV;
    vector<vector<float>> testHGT;
    vector<vector<float>> beforetichuHGT;
    for(int i = 0 ; i < ceng_count ; i++)
    {
        forU.clear();
        forV.clear();
        forHGT.clear();
        for(int j = 0 ; j <lon_num*lat_num ; j++)
        {
            forU.push_back(U[i*lon_num*lat_num+j]);
            forV.push_back(V[i*lon_num*lat_num+j]);
            forHGT.push_back(HGT[i*lon_num*lat_num+j]);
        }
        testU.push_back(forU);
        testV.push_back(forV);
        testHGT.push_back(forHGT);
    }
//    qDebug()<<testHGT[10][0]<<testHGT[10][360]<<testHGT[10][101080]<<testHGT[10][101440];
    vector<float> tmpU;
    vector<float> tmpV;
    vector<float> tmpHGT;
    for(int i = 0 ; i < testU.size() ; i++)
    {
        tmpU.clear();
        tmpV.clear();
        tmpHGT.clear();
        for(int j = lat_num-1 ; j >=0 ; j--)
        {
            for(int k = 0 ; k < lon_num ; k++)
            {
                tmpU.push_back(testU[i][j*lon_num+k]);
                tmpV.push_back(testV[i][j*lon_num+k]);
//                if((j*lon_num+k+1)%361==0||(j*lon_num+k+1)>101080)
//                    continue;
                tmpHGT.push_back(testHGT[i][j*lon_num+k]);
            }
        }
        Uwind->push_back(tmpU);
        Vwind->push_back(tmpV);
//        HIGHT025->push_back(tmpHGT);
        beforetichuHGT.push_back(tmpHGT);
//        HIGHT->push_back(tmpHGT);
    }

//    qDebug()<<beforetichuHGT[10][0]<<beforetichuHGT[10][360]<<beforetichuHGT[10][101080]<<beforetichuHGT[10][101440];
    lat_start = lat_start+lat_unit*(lat_num-1);//lat_start = 60
    lat_unit = -lat_unit;//lat_unit = -0.25
    ////***********************////

    vector<vector<float>> aftertichuHGT;

    for(int i = 0 ; i < beforetichuHGT.size() ; i++)
    {
        int k = 0;
        int count = 0;
        vector<float> smallHGTtmp;
        for(int j = 0 ; j<beforetichuHGT[i].size();j++)
        {
            if((j-k*9)%16==0)
            {
                smallHGTtmp.push_back(beforetichuHGT[i][j]/*beforetichuHGT->at(i).at(j)*/);
                count++;
            }
            if(count!=0&&count%23==0)
            {
                k=k+16;
                j=j+8+15*361;
            }
            if(j>=98553)
                break;
        }
//        if(i==10)
//            qDebug()<<smallHGTtmp[413];

//        for(int j =0;j<beforetichuHGT[i].size();j++)
//        {
//            if(j%16==0&&(j/360)%16==0)
//                smallHGTtmp.push_back(beforetichuHGT[i][j]);
//        }
        HIGHT->push_back(smallHGTtmp);
    }

    vector<vector<float>> aftertichuHGT2;
    for(int i = 0 ; i < beforetichuHGT.size() ; i++)
    {
        int k = 0;
        int count = 0;
        vector<float> smallHGTtmp;
        for(int j = 0 ; j<beforetichuHGT[i].size();j++)
        {
            if((j-k*1)%4==0)
            {
                smallHGTtmp.push_back(beforetichuHGT[i][j]/*beforetichuHGT->at(i).at(j)*/);
                count++;
            }
            if(count!=0&&count%91==0)
            {
                k=k+4;
                j=j+3*361;
            }
        }
        HIGHT025->push_back(smallHGTtmp);
//        qDebug()<<smallHGTtmp.size();
    }



}

void MainWindow::TheShikuangDataDeal(QList<QList<QWind>> thelistofwindvaluelist, QList<QList<QQMe>> thelistofheightlist,QList<QList<QQMe>> thelistofheightlist_025, double lon_unit, double lat_unit, double lon_start, double lon_end, double lat_start, double lat_end, double lon_num, double lat_num, QString string, QString name)
{
    QList<QXItem *> thelistofQXitem;
    QList<WindDKItem *> thelistofDKitem;
    QList<QBItem *> thelistofQBitem;
    QList<WindItem *> thelistofDirectionitem;
    QList<WindTextItem *> thelistofPoweritem;
    QList<ColdToughItem *> thelistofCTitem;
    QList<HotRidgeItem *>thelistofHRitem;
    QThreadPool threadpool;
    QList<QList<QQMe>> thelistofheightlist2;
    ////*********下边记得注释*********////
//    QString cunchu = "F:/finish/201706";
//    QString savename = cunchu+"/"+string+".txt";
//    QFile file(savename);
//    file.open(QFile::Append);
//    QTextStream write(&file);

//    write<<"\n"<<name<<"\n";
//    file.close();
    ////****************************////
    for(int i = 0; i<thelistofwindvaluelist.size();i++)
    {
        if( i==10 || i==12 || i==14 || i ==16)
        {
            qDebug()<<"find the Item in the "<<i<<"ceng";
//            switch (i) {
//            case 10:
//            {
//                file.open(QFile::Append);
//                QTextStream write(&file);

//                write<<"500MPa detective:\n";
//                file.close();
//                break;
//            }
//            case 12:
//            {
//                file.open(QFile::Append);
//                QTextStream write(&file);

//                write<<"700MPa detective:\n";
//                file.close();
//                break;
//            }
//            case 14:
//            {
//                file.open(QFile::Append);
//                QTextStream write(&file);

//                write<<"850MPa detective:\n";
//                file.close();
//                break;
//            }
//            default:
//                break;
//            }
            QList<QQMe> heightlist = thelistofheightlist[i];
            thelistofheightlist2<<heightlist;

            QList<QWind> windvaluelist = thelistofwindvaluelist[i];

            int mid = windvaluelist.size()/2;

            //找风向，风速
            QList<QWind> windvaluelistU =windvaluelist.mid(0,mid);
            QList<QWind> windvaluelistV =windvaluelist.mid(mid,windvaluelist.size());
            QList<QWind> winddirectionlist;
            QList<QWind> windpowerlist;
            for(int i = 0;i<windvaluelistU.size();i++)
            {
                QWind winddirection;
                winddirection.y = windvaluelistU.at(i).y;
                winddirection.x = windvaluelistU.at(i).x;
                winddirection.wind =qAtan2(windvaluelistV.at(i).wind,windvaluelistU.at(i).wind);

                QWind windpower;
                windpower.y = windvaluelistU.at(i).y;
                windpower.x = windvaluelistU.at(i).x;
                windpower.wind = sqrt(pow(windvaluelistV.at(i).wind,2)+pow(windvaluelistU.at(i).wind,2));

                winddirectionlist<<winddirection;
                windpowerlist<<windpower;
            }

//    //        qDebug()<<lon_start<<lon_num<<lon_unit;
//    //        qDebug()<<lat_start<<lat_num<<lat_unit;

            QString savename;
            ////****+CT+WR****////
            ColdToughItem *ctitem = FindCT(thelistofheightlist_025[i],23,18,4,60,4,-8,savename);
            thelistofCTitem<<ctitem;
//            qDebug()<<"1";
            HotRidgeItem *hritem = FindHR(thelistofheightlist_025[i],23,18,4,60,4,-8,savename/*heightlist,23,18,4,60,-4,60,savename*/);
            thelistofHRitem<<hritem;
//            qDebug()<<"2";
            QXItem *qxitem = FindQX(windvaluelistU,windvaluelistV,lon_num,lat_num,lon_unit,lon_start,lat_unit,lat_start,savename);
            thelistofQXitem<<qxitem;
//            qDebug()<<"3";
            WindDKItem *dkjlitem = FindDKJL(windvaluelistU,windvaluelistV,winddirectionlist,windpowerlist,lat_num,lon_num,lon_unit,lon_start,lat_start,savename);
            thelistofDKitem<<dkjlitem;
//            qDebug()<<"4";
            QBItem *qbitem = FindQB(windvaluelistU,windvaluelistV,lon_start,lat_start,lon_num,lat_num,lon_unit,lat_unit,savename);
            thelistofQBitem<<qbitem;
//            qDebug()<<"5";
            WindItem *winditem = new WindItem(winddirectionlist,ceng,view,shadowtype,QPen(QColor(QColor(0,0,0)),0,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin),0.1414,0.1);
            thelistofDirectionitem<<winditem;
//            qDebug()<<"go on ";
            WindTextItem *windTextItem = new WindTextItem(windpowerlist,ceng,view,shadowtype);
            thelistofPoweritem<<windTextItem;
//            qDebug()<<"go on ";

        }
    }

////*********下边这两行记得取消注释*********////
    Qshikuangdialog *window = new Qshikuangdialog(scene,thelistofQXitem,thelistofDKitem,thelistofQBitem,thelistofDirectionitem,thelistofPoweritem,thelistofheightlist2,thelistofCTitem,thelistofHRitem,lon_unit,lat_unit,lon_start,lat_start,lon_num,lat_num,shadowtype);
    window->show();
////************************************////

}


////*****************************************************************

void MainWindow::on_testbutton_triggered()
{
    QString string = QFileDialog::getExistingDirectory();
    QDir dir(string);
    QStringList list = dir.entryList();

    QList<QList<QList<QPointF>>> thelistofdkjlline;
    QList<QList<QList<QPointF>>> thelistofdkjlarea;
    QList<QList<QList<QPointF>>> thelistofqiebianline;
    QList<QList<QList<QPointF>>> thelistoflengcaoline;
    QList<QList<QList<QPointF>>> thelistoffugaoline;
    QList<QList<Point3f>> thelistofqixuan;
    QList<QString> thelistofsavename;
    int lat_count;
    int lon_count;
    float lat_start;
    float lat_end;
    float lat_unit;
    float lon_start;
    float lon_end;
    float lon_unit;

    for(int i = 0 ; i<list.size();i++)
    {
        QString tmp = list[i];
        if(tmp.right(3)!=".nc" && tmp!="." && tmp!="..")
        {
            QString dirpath = string+"/"+tmp;
            ////********读经纬度信息与天气系统信息********////
            QString savename = dirpath+"/"+dirpath.right(19)+".xml";
            thelistofsavename.append(dirpath.right(19));
            qDebug()<<savename;
            QFile Infofile(savename);
            Infofile.open(QFile::ReadOnly);
            QXmlStreamReader reader;
            reader.setDevice(&Infofile);
            QList<QList<QPointF>> dkjlline;
            QList<QList<QPointF>> dkjlarea;
            QList<QList<QPointF>> qiebianline;
            QList<QList<QPointF>> lengcaoline;
            QList<QList<QPointF>> fugaoline;
            QList<Point3f> qixuan;
            while(!reader.atEnd())
            {
                QXmlStreamReader::TokenType type = reader.readNext();

                if(reader.name()=="lat_count")
                    lat_count=reader.readElementText().toFloat();
                if(reader.name()=="lon_count")
                    lon_count=reader.readElementText().toFloat();
                if(reader.name()=="lat_start")
                    lat_start=reader.readElementText().toFloat();
                if(reader.name()=="lat_end")
                    lat_end=reader.readElementText().toFloat();
                if(reader.name()=="lat_unit")
                    lat_unit=reader.readElementText().toFloat();
                if(reader.name()=="lon_start")
                    lon_start=reader.readElementText().toFloat();
                if(reader.name()=="lon_end")
                    lon_end=reader.readElementText().toFloat();
                if(reader.name()=="lon_unit")
                    lon_unit=reader.readElementText().toFloat();
                if(reader.name()=="cyclone")
                {
                    QString tmp = reader.readElementText();
                    QStringList tmplist = tmp.split(" ");
//                    QList<QString> tmplist2;
//                    for(int j = 0 ; j<tmplist.size();j++)
//                    {
//                        tmplist2.push_back(tmplist[j]);
//                    }
                    Point3f point(tmplist[0].toFloat(),tmplist[1].toFloat(),tmplist[2].toFloat());
                    qixuan.append(point);
                }
                if(reader.name()=="shear")
                {
                    QString tmp = reader.readElementText();
                    QStringList tmplist = tmp.split(" ");
                    QString test;
                    QList<QPointF> qiebian;
                    for(int j = 0 ; j<tmplist.size()-1;j++)
                    {
                        test = tmplist[j];
                        QStringList onepoint = test.split(",");
                        float x = onepoint[0].remove(0,1).toFloat();
                        float y = onepoint[1].remove(onepoint[1].size()-1,1).toFloat();
                        QPointF point(x,y);
                        qiebian.append(point);
                    }
                    qiebianline.append(qiebian);
                }
                if(reader.name()=="flowmidline")
                {
                    QString tmp = reader.readElementText();
                    QStringList tmplist = tmp.split(" ");
                    QString test;
                    QList<QPointF> dkjlml;
                    for(int j = 0 ; j<tmplist.size()-1;j++)
                    {
                        test = tmplist[j];
                        QStringList onepoint = test.split(",");
                        float x = onepoint[0].remove(0,1).toFloat();
                        float y = onepoint[1].remove(onepoint[1].size()-1,1).toFloat();
                        QPointF point(x,y);
                        dkjlml.append(point);
                    }
                    dkjlline.append(dkjlml);
                }

                if(reader.name()=="flowcontour")
                {
                    QString tmp = reader.readElementText();
                    QStringList tmplist = tmp.split(" ");
                    QString test;
                    QList<QPointF> dkjlc;
                    for(int j = 0 ; j<tmplist.size()-1;j++)
                    {
                        test = tmplist[j];
                        QStringList onepoint = test.split(",");
                        float x = onepoint[0].remove(0,1).toFloat();
                        float y = onepoint[1].remove(onepoint[1].size()-1,1).toFloat();
                        QPointF point(x,y);
                        dkjlc.append(point);
                    }
                    dkjlarea.append(dkjlc);
                }

                if(reader.name()=="fugao")
                {
                    QString tmp = reader.readElementText();
                    QStringList tmplist = tmp.split(" ");
                    QString test;
                    QList<QPointF> fugao;
                    for(int j = 0 ; j<tmplist.size()-1;j++)
                    {
                        test = tmplist[j];
                        QStringList onepoint = test.split(",");
                        float x = onepoint[0].remove(0,1).toFloat();
                        float y = onepoint[1].remove(onepoint[1].size()-1,1).toFloat();
                        QPointF point(x,y);
                        fugao.append(point);
                    }
                    fugaoline.append(fugao);
                }

                if(reader.name()=="trough")
                {
                    QString tmp = reader.readElementText();
                    QStringList tmplist = tmp.split(" ");
                    QString test;
                    QList<QPointF> lengcao;
                    for(int j = 0 ; j<tmplist.size()-1;j++)
                    {
                        test = tmplist[j];
                        QStringList onepoint = test.split(",");
                        float x = onepoint[0].remove(0,1).toFloat();
                        float y = onepoint[1].remove(onepoint[1].size()-1,1).toFloat();
                        QPointF point(x,y);
                        lengcao.append(point);
                    }
                    lengcaoline.append(lengcao);
                }

            }
//            qDebug()<<lat_count<<lon_count<<lat_start<<lat_end<<lat_unit<<lon_start<<lon_end<<lon_unit;
            thelistofqixuan.append(qixuan);
            thelistofqiebianline.append(qiebianline);
            thelistofdkjlarea.append(dkjlarea);
            thelistofdkjlline.append(dkjlline);
            thelistoffugaoline.append(fugaoline);
            thelistoflengcaoline.append(lengcaoline);
//            qDebug()<<"shear"<<qiebianline.size()<<"cyclone"<<qixuan.size()<<"flowarea"<<dkjlarea.size()<<"flowmidline"<<dkjlline.size()<<"fugao"<<fugaoline.size()<<"trough"<<lengcaoline.size();
            Infofile.close();
            ////*************************************////
            QString upath=dirpath+"/u";
            QString vpath=dirpath+"/v";
            QString zpath=dirpath+"/z";
        }
    }
    qDebug()<<thelistofsavename.size();
    Qdatadialog *window = new Qdatadialog(shadowtype,scene,thelistofsavename,thelistofdkjlline,thelistofdkjlarea,thelistofqiebianline,thelistoflengcaoline,thelistoffugaoline,thelistofqixuan);
    window->show();
}
