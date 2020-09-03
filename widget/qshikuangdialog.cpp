#include "qshikuangdialog.h"

Qshikuangdialog::Qshikuangdialog(QGraphicsScene* sscene, QList<QXItem *> tthelistofQXitem, QList<WindDKItem *> tthelistofDKitem, QList<QBItem *> tthelistofQBitem, QList<WindItem *> tthelistofDirectionitem, QList<WindTextItem *> tthelistofPoweritem, QList<QList<QQMe>> tthelistofheightlist, QList<ColdToughItem *> tthelistofCTitem, QList<HotRidgeItem *> tthelistofHRitem , double llon_unit, double llat_unit, double llon_start, double llat_start, double llon_num, double llat_num, int sshadowtype)
{
    lon_unit = llon_unit;
    lon_start = llon_start;
    lon_num = llon_num;
    lat_unit = llat_unit;
    lat_start = llat_start;
    lat_num = llat_num;
    shadowtype = sshadowtype;
    scene = sscene;
    thelistofDirectionitem = tthelistofDirectionitem;
    thelistofPoweritem = tthelistofPoweritem;
    thelistofQXitem = tthelistofQXitem;
    thelistofQBitem = tthelistofQBitem;
    thelistofDKitem = tthelistofDKitem;
    thelistoftheheightlist = tthelistofheightlist;
    thelistCTitem = tthelistofCTitem;
    thelistHRitem = tthelistofHRitem;
//    for(int i = 0 ; i < thelistoftheheightlist.size() ; i++)
//    {
//        for(int j = 0 ; j <thelistoftheheightlist[i].size(); j++)
//        {
//            thelistoftheheightlist[i][j].pressure = thelistoftheheightlist[i][j].pressure/10;
////            qDebug()<<thelistoftheheightlist[i][j].pressure;
//        }
//    }
    HGTextItem *HGTextitem = new HGTextItem(thelistoftheheightlist[0],shadowtype);

    ////********置于窗口最前端********////
    Qt::WindowFlags flags = this->windowFlags();
    this->setWindowFlags(flags | Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint);
    ////***************************////


    ////********环境设置********////
    QHBoxLayout* zero_layout = new QHBoxLayout(this);
    QVBoxLayout* one_layout = new QVBoxLayout;
    zero_layout->addLayout(one_layout);
    QLabel* two_liuchangtitle = new QLabel(QString::fromLocal8Bit("流场"));
    one_layout->addWidget(two_liuchangtitle);
    QHBoxLayout* two_hbox_1 = new QHBoxLayout;
    one_layout->addLayout(two_hbox_1);
    QLabel* three_label = new QLabel("lv_ISBL2");
    two_hbox_1->addWidget(three_label);
    three_ISBL2 = new QComboBox();
    two_hbox_1->addWidget(three_ISBL2);
    three_ISBL2->addItem("500");
    three_ISBL2->addItem("700");
    three_ISBL2->addItem("850");
    three_ISBL2->addItem("925");
    QLabel* two_tianqititle = new QLabel(QString::fromLocal8Bit("流场天气系统识别"));
    one_layout->addWidget(two_tianqititle);
    QHBoxLayout* two_hbox_2 = new QHBoxLayout;
    one_layout->addLayout(two_hbox_2);
    three_checkpower = new QCheckBox(QString::fromLocal8Bit("风速"));
    two_hbox_2->addWidget(three_checkpower);
    three_checkdirection = new QCheckBox(QString::fromLocal8Bit("风向"));
    two_hbox_2->addWidget(three_checkdirection);
    QHBoxLayout* two_hbox_3 = new QHBoxLayout;
    one_layout->addLayout(two_hbox_3);
    three_checkqixuan = new QCheckBox(QString::fromLocal8Bit("气旋"));
    two_hbox_3->addWidget(three_checkqixuan);
    three_checkqiebian = new QCheckBox(QString::fromLocal8Bit("风切变"));
    two_hbox_3->addWidget(three_checkqiebian);
    QHBoxLayout* two_hbox_4 = new QHBoxLayout;
    one_layout->addLayout(two_hbox_4);
    three_checkdkjl = new QCheckBox(QString::fromLocal8Bit("低空急流"));
    two_hbox_4->addWidget(three_checkdkjl);
    QLabel* two_textlabel = new QLabel(QString::fromLocal8Bit("检测结果的文字显示"));
    one_layout->addWidget(two_textlabel);
    two_text = new QTextBrowser;
    one_layout->addWidget(two_text);

    QVBoxLayout* one_layout2 = new QVBoxLayout;
    zero_layout->addLayout(one_layout2);
    QLabel* two_heightitle = new QLabel(QString::fromLocal8Bit("高度场"));
    one_layout2->addWidget(two_heightitle);
    QHBoxLayout* two_hbox_5 = new QHBoxLayout;
    one_layout2->addLayout(two_hbox_5);
    QLabel* three_minheight = new QLabel(QString::fromLocal8Bit("最小高度"));
    two_hbox_5->addWidget(three_minheight);
    three_minh = new QLineEdit;
    two_hbox_5->addWidget(three_minh);
    QHBoxLayout* two_hbox_6 = new QHBoxLayout;
    one_layout2->addLayout(two_hbox_6);
    QLabel* three_maxheight = new QLabel(QString::fromLocal8Bit("最大高度"));
    two_hbox_6->addWidget(three_maxheight);
    three_maxh = new QLineEdit;
    two_hbox_6->addWidget(three_maxh);
    QHBoxLayout* two_hbox_7 = new QHBoxLayout;
    one_layout2->addLayout(two_hbox_7);
    QLabel* three_gapheight = new QLabel(QString::fromLocal8Bit("高度间隔"));
    two_hbox_7->addWidget(three_gapheight);
    three_gaph = new QLineEdit;
    two_hbox_7->addWidget(three_gaph);
    two_pushbutton = new QPushButton(QString::fromLocal8Bit("应用"));
    one_layout2->addWidget(two_pushbutton);
    QHBoxLayout* two_hbox_8 = new QHBoxLayout;
    one_layout2->addLayout(two_hbox_8);
    three_findfufao = new QCheckBox(QString::fromLocal8Bit("寻找副高"));
    two_hbox_8->addWidget(three_findfufao);
    three_checkheightvalue = new QCheckBox(QString::fromLocal8Bit("高度显示"));
    two_hbox_8->addWidget(three_checkheightvalue);
    QHBoxLayout* two_hbox_9 = new QHBoxLayout;
    one_layout2->addLayout(two_hbox_9);
    three_checkcoldtough = new QCheckBox(QString::fromLocal8Bit("冷槽"));
    two_hbox_9->addWidget(three_checkcoldtough);
    three_checkhotridge = new QCheckBox(QString::fromLocal8Bit("暖脊"));
    two_hbox_9->addWidget(three_checkhotridge);

    QHBoxLayout* two_hbox_10 = new QHBoxLayout;
    one_layout2->addLayout(two_hbox_10);
    QLabel* three_textlabel2 = new QLabel(QString::fromLocal8Bit("检测结果的文字显示"));
    two_hbox_10->addWidget(three_textlabel2);
    three_checkhight = new QCheckBox(QString::fromLocal8Bit("等高线"));
    two_hbox_10->addWidget(three_checkhight);
    two_text2 = new QTextBrowser;
    one_layout2->addWidget(two_text2);
    ////**********************////


    ////********更新高度场数据********////
    QList<QQMe> heightlist = thelistoftheheightlist[0];
    QList<double> pressurelist;
    for(int i=0;i<heightlist.size();i++)
    {
        pressurelist<<heightlist.at(i).pressure;
    }
    qSort(pressurelist.begin(),pressurelist.end());
    int max=pressurelist.last();
    int min=pressurelist.first();
    int space=2;
    this->three_maxh->setText(QString::number(max));
    this->three_minh->setText(QString::number(min));
    this->three_gaph->setText(QString::number(space));
    VoltageItem* Voltitem = DrawPressures(heightlist,max,min,space,shadowtype);
    FuGaoItem* FGItem = FindFugao(heightlist,shadowtype);
    ////****************************////

    ////********显示初始数据********////
    scene->addItem(thelistofDKitem[0]);
    scene->items().at(0)->hide();
    scene->addItem(thelistofQBitem[0]);
    scene->items().at(0)->hide();
    scene->addItem(thelistofQXitem[0]);
    scene->items().at(0)->hide();
    scene->addItem(thelistofPoweritem[0]);
    scene->items().at(0)->hide();
    scene->addItem(thelistofDirectionitem[0]);
    scene->items().at(0)->hide();
    scene->addItem(thelistHRitem[0]);
    scene->items().at(0)->hide();
    scene->addItem(thelistCTitem[0]);
    scene->items().at(0)->hide();
    scene->addItem(HGTextitem);
    scene->items().at(0)->hide();
    scene->addItem(FGItem);
    scene->items().at(0)->hide();
    scene->addItem(Voltitem);
    scene->items().at(0)->hide();
    ////**************************////


    connect(three_ISBL2,&QComboBox::currentTextChanged,this,&Qshikuangdialog::ChooseCeng);
    connect(three_checkdirection,&QCheckBox::clicked,this,&Qshikuangdialog::showdirection);
    connect(three_checkpower,&QCheckBox::clicked,this,&Qshikuangdialog::showpower);
    connect(three_checkqixuan,&QCheckBox::clicked,this,&Qshikuangdialog::showqixuan);
    connect(three_checkqiebian,&QCheckBox::clicked,this,&Qshikuangdialog::showqiebian);
    connect(three_checkdkjl,&QCheckBox::clicked,this,&Qshikuangdialog::showdkjl);
    connect(three_checkheightvalue,&QCheckBox::clicked,this,&Qshikuangdialog::showheighttext);
    connect(three_checkcoldtough,&QCheckBox::clicked,this,&Qshikuangdialog::showcoldtough);
    connect(three_checkhotridge,&QCheckBox::clicked,this,&Qshikuangdialog::showhotridge);
    connect(two_pushbutton,&QPushButton::clicked,this,&Qshikuangdialog::showpressureButton);
    connect(three_checkhight,&QCheckBox::clicked,this,&Qshikuangdialog::showhight);
    connect(three_findfufao,&QCheckBox::clicked,this,&Qshikuangdialog::showfugao);

//    two_text->insertPlainText("hello world\n");
    two_text->insertPlainText(QString::fromLocal8Bit("正在检测500hPa层天气系统\n"));
    two_text->insertPlainText(QString::fromLocal8Bit("气旋数量\n"));
    two_text->insertPlainText(QString::number(thelistofQXitem[0]->getsize()));
    two_text->insertPlainText(QString::fromLocal8Bit("\n切变数量\n"));
    two_text->insertPlainText(QString::number(thelistofQBitem[0]->getsize()));
    two_text->insertPlainText(QString::fromLocal8Bit("\n低空急流数量\n"));
    two_text->insertPlainText(QString::number(thelistofDKitem[0]->getsize()));
}

void Qshikuangdialog::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton button;
    button=QMessageBox::question(this,QString::fromLocal8Bit("退出"),QString::fromLocal8Bit("是否确认关闭当前图层"),QMessageBox::Yes|QMessageBox::No);
    if(button==QMessageBox::No)
    {
        event->ignore(); // 忽略退出信号，程序继续进行
    }
    else if(button==QMessageBox::Yes)
    {
        event->accept(); // 接受退出信号，程序退出

        scene->removeItem(scene->items().at(0));
        scene->removeItem(scene->items().at(0));
        scene->removeItem(scene->items().at(0));
        scene->removeItem(scene->items().at(0));
        scene->removeItem(scene->items().at(0));
        scene->removeItem(scene->items().at(0));
        scene->removeItem(scene->items().at(0));
        scene->removeItem(scene->items().at(0));
        scene->removeItem(scene->items().at(0));
        scene->removeItem(scene->items().at(0));
    }
}

VoltageItem* Qshikuangdialog::DrawPressures(QList<QQMe> heightlist,int max,int min,int space,int shadowtype)
{
    int lon_num_M = (lon_num-9)/16+1;
    int lat_num_M = (lat_num-9)/16+1;
    QList<QPainterPath> pathlist;
//    max=589;
//    min=589;
//    qDebug()<<lon_num_M<<lat_num_M;
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
        ///4.找到等压线穿过的三角网格
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
//            qDebug()<<"This is a unbreakable break point";
            ////5.3平滑

            if(pointlist1.size() > 10)
            {
//                qDebug()<<pressure<<"The first time"<<pointlist1.size();
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
                QFont font;
                font.setPointSize(60);
                aftersmootpath.addText(pointlist2[pointlist2.size()/2],font,QString::number(pressure));
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
//                qDebug()<<pressure<<"The second time"<<pointlist1.size();
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
                QFont font;
                font.setPointSize(60);
                aftersmootpath.addText(pointlist2[pointlist2.size()/2],font,QString::number(pressure));
                pathlist.push_back(aftersmootpath);
            }
            pointlist1.clear();
        }
    }
    VoltageItem* item = new VoltageItem(pathlist);
    return item;
}

FuGaoItem *Qshikuangdialog::FindFugao(QList<QQMe> heightlist, int shadowtype)
{

    int lon_num_M = (lon_num-9)/16+1;
    int lat_num_M = (lat_num-9)/16+1;
    QPainterPath path;
    QList<QList<QPointF>> thelistof588point;
    QList<QList<QPointF>> thelistoffugaopoint;
    QList<QList<QPointF>> thelistoffugaopoint2;
    /// 1.区域网格化
    QList<QTr> trlist = makegrid(lat_num_M,lon_num_M);
    /// 3.格值点的预处理
    for(int i=0;i<heightlist.size();i++)//预处理等于等压线的格点数据
    {
        if(heightlist[i].pressure==588)
            heightlist[i].pressure=heightlist[i].pressure+0.1;
    }
    ///4.找到等压线穿过的三角网格
    QList<QTr> TRlist = findusegrid(heightlist,trlist,588);
    ///5.边界等值线追踪
    for(int i = 0 ; i <TRlist.size();i++)
    {
        QList<QPointF> pointlist1;
        pointlist1 = FirstTimeCalculatePoint(TRlist,&trlist,heightlist,588,i,shadowtype);
        if(pointlist1.size() > 10)
        {
//            qDebug()<<"588"<<pointlist1.size()<<pointlist1;
            thelistof588point.push_back(pointlist1);
        }
    }
//    qDebug()<<"thelistof588"<<thelistof588point.size()<<thelistof588point.last().last();
    for(int i=0 ; i < thelistof588point.size();i++)
    {
        QList<QPointF> fugaopointlist;
        for(int j = 0 ; j < thelistof588point[i].size();j++)
        {
            if(thelistof588point[i][j].x()>=110&&thelistof588point[i][j].y()>=-15)
            {
                fugaopointlist<<thelistof588point[i][j];
            }
        }
        if(fugaopointlist.size()>0)
            thelistoffugaopoint.push_back(fugaopointlist);
    }
//    qDebug()<<"thelistoffugao"<<thelistoffugaopoint.size();
    for(int i = 0 ; i < thelistoffugaopoint.size();i++)
    {
        QList<QPointF> aftersmoothlist;
        BCurve(thelistoffugaopoint[i],aftersmoothlist,true);
        QList<QPointF> pointlist2;
        ////看下边添加一个点
        QList<double> tmpfirst;
        tmpfirst<<thelistoffugaopoint[i].first().x()<<thelistoffugaopoint[i].first().y() ;
        QPointF pointfirst = ShadowXY(tmpfirst,shadowtype);
        pointlist2<<pointfirst;
        ////看上边
        for(int i = 0 ; i <aftersmoothlist.size();i++)
        {
            QList<double> tmp;
            tmp<<aftersmoothlist[i].x()<<aftersmoothlist[i].y();
            QPointF point = ShadowXY(tmp,shadowtype);
            pointlist2<<point;
        }
        ////看下边添加最后一个点
        QList<double> tmplast1;
        tmplast1<<thelistoffugaopoint[i].last().x()<<thelistoffugaopoint[i].last().y() ;
        QPointF pointlast1 = ShadowXY(tmplast1,shadowtype);
        pointlist2<<pointlast1;
        if(fabs(tmpfirst[0]-150)>1)
        {
            QList<double> tmplast0;
            tmplast0<<150<<-8;
            QPointF pointlast0 = ShadowXY(tmplast0,shadowtype);
            pointlist2<<pointlast0;
        }
        QList<double> tmplast;
        tmplast<<thelistoffugaopoint[i].first().x()<<thelistoffugaopoint[i].first().y() ;
        QPointF pointlast = ShadowXY(tmplast,shadowtype);
        pointlist2<<pointlast;
        ////看上边
        if(thelistoffugaopoint[i][0].y()==-8)
        {
            QList<double> tmp;
            tmp<<thelistoffugaopoint[i].last().x()<<-8;
            QPointF point = ShadowXY(tmp,shadowtype);
            pointlist2<<point;
        }
        thelistoffugaopoint2.push_back(pointlist2);
    }
//    qDebug()<<"thelistoffugao2"<<thelistoffugaopoint2.size();
    QList<QPainterPath> thelistofpath;
    for(int i = 0 ; i < thelistoffugaopoint2.size();i++)
    {
        QPainterPath path = Smoothedpath(thelistoffugaopoint2[i],true);
        thelistofpath.push_back(path);
    }
//    qDebug()<<"thelistofpath"<<thelistofpath.size();
    FuGaoItem* item = new FuGaoItem(thelistofpath);
    return item;
}

void Qshikuangdialog::ChooseCeng()
{

    int index = three_ISBL2->currentIndex();
    ////********更新高度场数据********////
    QList<QQMe> heightlist = thelistoftheheightlist[index];
    QList<double> pressurelist;
    for(int i=0;i<heightlist.size();i++)
    {
        pressurelist<<heightlist.at(i).pressure;
    }
    qSort(pressurelist.begin(),pressurelist.end());
    int max=pressurelist.last();
    int min=pressurelist.first();
//    int max = 589;
//    int min = 589;
    int space=2;
    this->three_maxh->setText(QString::number(max));
    this->three_minh->setText(QString::number(min));
    this->three_gaph->setText(QString::number(space));
    VoltageItem* Voltitem = DrawPressures(heightlist,max,min,space,shadowtype);
//    qDebug()<<"done1";
    FuGaoItem* FGItem = FindFugao(heightlist,shadowtype);
//    qDebug()<<"done2";
    ////****************************////

    scene->removeItem(scene->items().at(0));
    scene->removeItem(scene->items().at(0));
    scene->removeItem(scene->items().at(0));
    scene->removeItem(scene->items().at(0));
    scene->removeItem(scene->items().at(0));
    scene->removeItem(scene->items().at(0));
    scene->removeItem(scene->items().at(0));
    scene->removeItem(scene->items().at(0));
    scene->removeItem(scene->items().at(0));
    scene->removeItem(scene->items().at(0));

    HGTextItem *HGTextitem = new HGTextItem(thelistoftheheightlist[index],shadowtype);

    scene->addItem(thelistofDKitem[index]);
    scene->items().at(0)->hide();
    scene->addItem(thelistofQBitem[index]);
    scene->items().at(0)->hide();
    scene->addItem(thelistofQXitem[index]);
    scene->items().at(0)->hide();
    scene->addItem(thelistofPoweritem[index]);
    scene->items().at(0)->hide();
    scene->addItem(thelistofDirectionitem[index]);
    scene->items().at(0)->hide();
    scene->addItem(thelistHRitem[index]);
    scene->items().at(0)->hide();
    scene->addItem(thelistCTitem[index]);
    scene->items().at(0)->hide();
    scene->addItem(HGTextitem);
    scene->items().at(0)->hide();
    scene->addItem(FGItem);
    scene->items().at(0)->hide();
    scene->addItem(Voltitem);
    scene->items().at(0)->hide();








    ////********重置CheckBox********////
    three_checkcoldtough->setCheckState(Qt::Unchecked);
    three_checkhotridge->setCheckState(Qt::Unchecked);
    three_checkdirection->setCheckState(Qt::Unchecked);
    three_checkpower->setCheckState(Qt::Unchecked);
    three_checkqixuan->setCheckState(Qt::Unchecked);
    three_checkqiebian->setCheckState(Qt::Unchecked);
    three_checkdkjl->setCheckState(Qt::Unchecked);
    three_checkheightvalue->setCheckState(Qt::Unchecked);
    three_findfufao->setCheckState(Qt::Unchecked);
    ////***************************////

    ////********文本框文字显示********////
    two_text->clear();
    switch (index) {
    case 0:
    {
        two_text->insertPlainText(QString::fromLocal8Bit("正在检测500hPa层天气系统\n"));
        break;
    }
    case 1:
    {
        two_text->insertPlainText(QString::fromLocal8Bit("正在检测700hPa层天气系统\n"));
        break;
    }
    case 2:
    {
        two_text->insertPlainText(QString::fromLocal8Bit("正在检测850hPa层天气系统\n"));
        break;
    }
    case 3:
    {
        two_text->insertPlainText(QString::fromLocal8Bit("正在检测925hPa层天气系统\n"));
        break;
    }
    default:
        break;
    }
    QXItem* qxtmp = thelistofQXitem[index];
    QBItem* qbtmp = thelistofQBitem[index];
    WindDKItem* dkjltmp = thelistofDKitem[index];
    two_text->insertPlainText(QString::fromLocal8Bit("气旋数量\n"));
    two_text->insertPlainText(QString::number(qxtmp->getsize()));
    two_text->insertPlainText(QString::fromLocal8Bit("\n切变数量\n"));
    two_text->insertPlainText(QString::number(qbtmp->getsize()));
    two_text->insertPlainText(QString::fromLocal8Bit("\n低空急流数量\n"));
    two_text->insertPlainText(QString::number(dkjltmp->getsize()));
    ////***************************////

}

void Qshikuangdialog::showdirection()
{
    bool ifshow = three_checkdirection->checkState();
    if(ifshow)
    {
        scene->items().at(5)->show();
    }
    else
        scene->items().at(5)->hide();
}

void Qshikuangdialog::showpower()
{
    bool ifshow = three_checkpower->checkState();
    if(ifshow)
        scene->items().at(6)->show();
    else
        scene->items().at(6)->hide();
}

void Qshikuangdialog::showqixuan()
{
    bool ifshow = three_checkqixuan->checkState();
    if(ifshow)
        scene->items().at(7)->show();
    else
        scene->items().at(7)->hide();
}

void Qshikuangdialog::showqiebian()
{
    bool ifshow = three_checkqiebian->checkState();
    if(ifshow)
        scene->items().at(8)->show();
    else
        scene->items().at(8)->hide();
}

void Qshikuangdialog::showdkjl()
{
    bool ifshow = three_checkdkjl->checkState();
    if(ifshow)
    {
        scene->items().at(9)->show();
    }
    else
        scene->items().at(9)->hide();
}

void Qshikuangdialog::showheighttext()
{
    bool ifshow = three_checkheightvalue->checkState();
    if(ifshow)
    {
        scene->items().at(2)->show();
    }
    else
        scene->items().at(2)->hide();
}

void Qshikuangdialog::showcoldtough()
{
    bool ifshow = three_checkcoldtough->checkState();
    if(ifshow)
        scene->items().at(3)->show();
    else
        scene->items().at(3)->hide();
}

void Qshikuangdialog::showhotridge()
{
    bool ifshow = three_checkhotridge->checkState();
    if(ifshow)
        scene->items().at(4)->show();
    else
        scene->items().at(4)->hide();
}

void Qshikuangdialog::showhight()
{
    bool ifshow = three_checkhight->checkState();
    if(ifshow)
        scene->items().at(0)->show();
    else
        scene->items().at(0)->hide();
}

void Qshikuangdialog::showpressureButton()
{
    int maxh = this->three_maxh->text().toInt();
    int minh = this->three_minh->text().toInt();
    int gaph = this->three_gaph->text().toInt();
    int index = three_ISBL2->currentIndex();
    QList<QQMe> heightlist = thelistoftheheightlist[index];
    QList<double> pressurelist;
    for(int i=0;i<heightlist.size();i++)
    {
        pressurelist<<heightlist.at(i).pressure;
    }
    VoltageItem* Voltitem = DrawPressures(heightlist,maxh,minh,gaph,shadowtype);
    scene->removeItem(scene->items().at(0));
    scene->addItem(Voltitem);
    bool ifshow = three_checkhight->checkState();
    if(ifshow)
        scene->items().at(0)->show();
    else
        scene->items().at(0)->hide();
}

void Qshikuangdialog::showfugao()
{
    bool ifshow = three_findfufao->checkState();
//    qDebug()<<ifshow;
    if(ifshow)
        scene->items().at(1)->show();
    else
        scene->items().at(1)->hide();
}

