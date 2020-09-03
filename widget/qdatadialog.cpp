#include "qdatadialog.h"

Qdatadialog::Qdatadialog(int sshadowtype,QGraphicsScene* sscene,QList<QString> tthelistofsavename,QList<QList<QList<QPointF>>> tthelistofdkjlline,QList<QList<QList<QPointF>>> tthelistofdkjlarea,QList<QList<QList<QPointF>>> tthelistofqiebianline,QList<QList<QList<QPointF>>> tthelistoflengcaoline,QList<QList<QList<QPointF>>> tthelistoffugaoline,QList<QList<Point3f>> tthelistofqixuan)
{
    scene = sscene;
    thelistofsavename = tthelistofsavename;
    thelistofdkjlarea=tthelistofdkjlarea;
    thelistofdkjlline=tthelistofdkjlline;
    thelistoffugaoline=tthelistoffugaoline;
    thelistoflengcaoline=tthelistoflengcaoline;
    thelistofqiebianline=tthelistofqiebianline;
    thelistofqixuan=tthelistofqixuan;
    shadowtype=sshadowtype;
    ////********环境设置********////
    QVBoxLayout* zero_layout = new QVBoxLayout(this);
    QHBoxLayout* one_layout = new QHBoxLayout();
    zero_layout->addLayout(one_layout);
    QLabel* two_label = new QLabel(QString::fromLocal8Bit("time"));
    one_layout->addWidget(two_label);
    two_time = new QComboBox();
    one_layout->addWidget(two_time);

    QHBoxLayout* one_layout_2 = new QHBoxLayout();
    zero_layout->addLayout(one_layout_2);
    QLabel* two_label_2 = new QLabel(QString::fromLocal8Bit("pressure"));
    one_layout_2->addWidget(two_label_2);
    two_pressure = new QComboBox();
    one_layout_2->addWidget(two_pressure);

    QHBoxLayout* one_layout_3 = new QHBoxLayout();
    zero_layout->addLayout(one_layout_3);
    QLabel* two_label_3 = new QLabel(QString::fromLocal8Bit("cyclone"));
    one_layout_3->addWidget(two_label_3);
    two_cyclone = new QCheckBox();
    one_layout_3->addWidget(two_cyclone);
    QLabel* two_label_4 = new QLabel(QString::fromLocal8Bit("shear"));
    one_layout_3->addWidget(two_label_4);
    two_shear = new QCheckBox();
    one_layout_3->addWidget(two_shear);

    QHBoxLayout* one_layout_4 = new QHBoxLayout();
    zero_layout->addLayout(one_layout_4);
    QLabel* two_label_5 = new QLabel(QString::fromLocal8Bit("flow"));
    one_layout_4->addWidget(two_label_5);
    two_flow = new QCheckBox();
    one_layout_4->addWidget(two_flow);
    QLabel* two_label_6 = new QLabel(QString::fromLocal8Bit("height"));
    one_layout_4->addWidget(two_label_6);
    two_height = new QCheckBox();
    one_layout_4->addWidget(two_height);

    QHBoxLayout* one_layout_5 = new QHBoxLayout();
    zero_layout->addLayout(one_layout_5);
    QLabel* two_label_7 = new QLabel(QString::fromLocal8Bit("winddirction"));
    one_layout_5->addWidget(two_label_7);
    two_winddirction = new QCheckBox();
    one_layout_5->addWidget(two_winddirction);
    QLabel* two_label_8 = new QLabel(QString::fromLocal8Bit("windpower"));
    one_layout_5->addWidget(two_label_8);
    two_windpower = new QCheckBox();
    one_layout_5->addWidget(two_windpower);

    QHBoxLayout* one_layout_6 = new QHBoxLayout();
    zero_layout->addLayout(one_layout_6);
    QLabel* two_label_9 = new QLabel(QString::fromLocal8Bit("trough"));
    one_layout_6->addWidget(two_label_9);
    two_trough = new QCheckBox();
    one_layout_6->addWidget(two_trough);
    QLabel* two_label_10 = new QLabel(QString::fromLocal8Bit("fugao"));
    one_layout_6->addWidget(two_label_10);
    two_fugao = new QCheckBox();
    one_layout_6->addWidget(two_fugao);

    QLabel* one_label = new QLabel(QString::fromLocal8Bit("detection results:"));
    zero_layout->addWidget(one_label);
    one_text = new QTextBrowser();
    zero_layout->addWidget(one_text);



    QSet<QString> set;
    for(int i = 0 ; i < thelistofsavename.size();i++)
    {
        QString tmp = thelistofsavename[i].left(14);

        set.insert(tmp);
    }
    namelist = set.toList();
    qSort(namelist.begin(),namelist.end());
    for(int i = 0 ; i < set.size();i++)
    {
        two_time->addItem(namelist[i]);
    }
    two_pressure->addItem("500");
    two_pressure->addItem("700");
    two_pressure->addItem("850");
    two_pressure->addItem("925");

    qixuandialog = new QPushButton(QString::fromLocal8Bit("See The Lives Of The Cyclone"));
    zero_layout->addWidget(qixuandialog);


    qDebug()<<thelistofdkjlarea.size()<<thelistofdkjlline.size()<<thelistoffugaoline.size()<<thelistoflengcaoline.size()<<thelistofqiebianline.size()<<thelistofqixuan.size();
    suoyin=two_time->currentIndex()*4+two_pressure->currentIndex();

    QList<QList<QPointF>> dkjllines=thelistofdkjlline[suoyin];
    QList<QList<QPointF>> dkjlareas=thelistofdkjlarea[suoyin];
    QList<QList<QPointF>> qiebianlines=thelistofqiebianline[suoyin];
    QList<QList<QPointF>> lengcaolines=thelistoflengcaoline[suoyin];
    QList<QList<QPointF>> fugaolines=thelistoffugaoline[suoyin];
    QList<Point3f> qixuans=thelistofqixuan[suoyin];

    QXItem* qxitem = FindQX(qixuans);
    scene->addItem(qxitem);
    scene->items().at(0)->hide();
    WindDKItem* dkjlitem = FindDKJL(dkjllines,dkjlareas);
    scene->addItem(dkjlitem);
    scene->items().at(0)->hide();
    QBItem* qbItem = FindQB(qiebianlines);
    scene->addItem(qbItem);
    scene->items().at(0)->hide();
    ColdToughItem* lcitem = FindCT(lengcaolines);
    scene->addItem(lcitem);
    scene->items().at(0)->hide();
    FuGaoItem* fgitem = FindFG(fugaolines);
    scene->addItem(fgitem);
    scene->items().at(0)->hide();

    connect(two_cyclone,&QCheckBox::clicked,this,&Qdatadialog::showqixuan);
    connect(two_flow,&QCheckBox::clicked,this,&Qdatadialog::showdkjl);
    connect(two_shear,&QCheckBox::clicked,this,&Qdatadialog::showqiebian);
    connect(two_fugao,&QCheckBox::clicked,this,&Qdatadialog::showfugao);
    connect(two_trough,&QCheckBox::clicked,this,&Qdatadialog::showcoldtough);
    connect(two_time,&QComboBox::currentTextChanged,this,&Qdatadialog::ChooseCeng);
    connect(two_pressure,&QComboBox::currentTextChanged,this,&Qdatadialog::ChooseCeng);
    connect(qixuandialog,&QPushButton::clicked,this,&Qdatadialog::showqixuanlife);
}

void Qdatadialog::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton button;
    button=QMessageBox::question(this,QString::fromLocal8Bit("exit"),QString::fromLocal8Bit("Do you want to close this session"),QMessageBox::Yes|QMessageBox::No);
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
    }
}

QXItem *Qdatadialog::FindQX(QList<Point3f> qixuans)
{
    QList<QPointF> pointlist;
    QList<float> r;
    for(int i = 0 ; i<qixuans.size();i++)
    {
        Point3f point = qixuans[i];
        QList<double> tmp;
        tmp<<point.x<<point.y;
        QPointF thepoint= ShadowXY(tmp,shadowtype);
        pointlist.append(thepoint);
        r.append(point.z);
    }
    QXItem* item = new QXItem(pointlist,1,shadowtype,r);
    return item;
}

WindDKItem *Qdatadialog::FindDKJL(QList<QList<QPointF> > dkjllines, QList<QList<QPointF> > dkjlareas)
{
    QList<QList<QPointF>> pointlist;
    QList<QList<QPointF>> arealist;


    for(int i = 0 ; i<dkjllines.size();i++)
    {
        QList<QPointF> tmp;
        for(int j = 0 ; j < dkjllines[i].size();j++)
        {
            QList<double> a;
            a<<dkjllines[i][j].x()<<dkjllines[i][j].y();
            QPointF point = ShadowXY(a,shadowtype);
            tmp.push_back(point);
        }
        pointlist.push_back(tmp);
    }
    for(int i = 0 ; i<dkjlareas.size();i++)
    {
        QList<QPointF> tmp;
        for(int j = 0 ; j < dkjlareas[i].size();j++)
        {
            QList<double> a;
            a<<dkjlareas[i][j].x()<<dkjlareas[i][j].y();
            QPointF point = ShadowXY(a,shadowtype);
            tmp.push_back(point);
        }
        arealist.push_back(tmp);
    }


    QList<QPolygonF> scontourpolygonlist = LineListToPolygon(arealist);
    WindDKItem *dkjlItem = new WindDKItem(scontourpolygonlist,pointlist,1);
    return dkjlItem;
}

QBItem *Qdatadialog::FindQB(QList<QList<QPointF> > qiebianlines)
{
    QList<QPainterPath> pathlist;
    for(int i = 0 ; i < qiebianlines.size() ; i++)
    {
        qiebianlines[i].removeLast();
        QPainterPath path;
        for(int j = 0 ; j < qiebianlines[i].size() ; j++)
        {
            QList<double> point;
            point<<qiebianlines[i][j].x()<<qiebianlines[i][j].y();
            QPointF temp = ShadowXY(point,shadowtype);
            if(j==0)
                path.moveTo(temp);
            else
                path.lineTo(temp);
        }
        pathlist.push_back(path);
    }
    QBItem* item = new QBItem(pathlist,1,shadowtype);
    return item;
}

ColdToughItem *Qdatadialog::FindCT(QList<QList<QPointF> > lengcaolines)
{
    QPainterPath pathlist;
    for(int i = 0 ; i < lengcaolines.size() ; i++)
    {
        QPainterPath path;
        for(int j = 0 ; j < lengcaolines[i].size() ; j++)
        {
            QList<double> point;
            point<<lengcaolines[i][j].x()<<lengcaolines[i][j].y();
            QPointF temp = ShadowXY(point,shadowtype);
            if(j==0)
                path.moveTo(temp);
            else
                path.lineTo(temp);
        }
        pathlist.addPath(path);
    }
    ColdToughItem* item = new ColdToughItem(pathlist,shadowtype);
    return item;
}

FuGaoItem *Qdatadialog::FindFG(QList<QList<QPointF>> fugaolines)
{
    QList<QList<QPointF>> thelistoffugaopoint2;
    for(int i = 0 ; i<fugaolines.size();i++)
    {
        QList<QPointF> tmp;
        for(int j = 0 ; j < fugaolines[i].size();j++)
        {
            QList<double> a;
            a<<fugaolines[i][j].x()<<fugaolines[i][j].y();
            QPointF point = ShadowXY(a,shadowtype);
            tmp.push_back(point);
        }
        thelistoffugaopoint2.push_back(tmp);
    }
    QList<QPainterPath> thelistofpath;
    for(int i = 0 ; i < thelistoffugaopoint2.size();i++)
    {
        QPainterPath path = Smoothedpath(thelistoffugaopoint2[i],true);
        thelistofpath.push_back(path);
    }
    FuGaoItem *item = new FuGaoItem(thelistofpath);
    return item;
}

void Qdatadialog::ChooseCeng()
{
    scene->removeItem(scene->items().at(0));
    scene->removeItem(scene->items().at(0));
    scene->removeItem(scene->items().at(0));
    scene->removeItem(scene->items().at(0));
    scene->removeItem(scene->items().at(0));

    suoyin=two_time->currentIndex()*4+two_pressure->currentIndex();

    QList<QList<QPointF>> dkjllines=thelistofdkjlline[suoyin];
    QList<QList<QPointF>> dkjlareas=thelistofdkjlarea[suoyin];
    QList<QList<QPointF>> qiebianlines=thelistofqiebianline[suoyin];
    QList<QList<QPointF>> lengcaolines=thelistoflengcaoline[suoyin];
    QList<QList<QPointF>> fugaolines=thelistoffugaoline[suoyin];
    QList<Point3f> qixuans=thelistofqixuan[suoyin];

    QXItem* qxitem = FindQX(qixuans);
    scene->addItem(qxitem);
    scene->items().at(0)->hide();
    WindDKItem* dkjlitem = FindDKJL(dkjllines,dkjlareas);
    scene->addItem(dkjlitem);
    scene->items().at(0)->hide();
    QBItem* qbItem = FindQB(qiebianlines);
    scene->addItem(qbItem);
    scene->items().at(0)->hide();
    ColdToughItem* lcitem = FindCT(lengcaolines);
    scene->addItem(lcitem);
    scene->items().at(0)->hide();
    FuGaoItem* fgitem = FindFG(fugaolines);
    scene->addItem(fgitem);
    scene->items().at(0)->hide();
}

void Qdatadialog::showqixuan()
{
    bool ifshow = two_cyclone->checkState();
    if(ifshow)
        scene->items().at(4)->show();
    else
        scene->items().at(4)->hide();
}

void Qdatadialog::showqiebian()
{
    bool ifshow = two_shear->checkState();
    if(ifshow)
        scene->items().at(2)->show();
    else
        scene->items().at(2)->hide();
}

void Qdatadialog::showdkjl()
{
    bool ifshow = two_flow->checkState();
    if(ifshow)
        scene->items().at(3)->show();
    else
        scene->items().at(3)->hide();
}

void Qdatadialog::showfugao()
{
    bool ifshow = two_fugao->checkState();
    if(ifshow)
        scene->items().at(0)->show();
    else
        scene->items().at(0)->hide();
}

void Qdatadialog::showcoldtough()
{
    bool ifshow = two_trough->checkState();
    if(ifshow)
        scene->items().at(1)->show();
    else
        scene->items().at(1)->hide();
}

void Qdatadialog::showqixuanlife()
{
    qDebug()<<"hello world";
    Qcyclonedialog* cyclonedialog = new Qcyclonedialog(scene,shadowtype,thelistofqixuan,namelist);
    cyclonedialog->show();
}

