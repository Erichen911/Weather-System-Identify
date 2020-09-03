#ifndef QDATADIALOG_H
#define QDATADIALOG_H

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
#include <QSet>
#include <qcyclonedialog.h>
class Qdatadialog : public QDialog
{
public:
    Qdatadialog(int sshadowtype,QGraphicsScene* sscene,QList<QString> tthelistofsavename,QList<QList<QList<QPointF>>> tthelistofdkjlline,QList<QList<QList<QPointF>>> tthelistofdkjlarea,QList<QList<QList<QPointF>>> tthelistofqiebianline,QList<QList<QList<QPointF>>> tthelistoflengcaoline,QList<QList<QList<QPointF>>> tthelistoffugaoline,QList<QList<Point3f>> tthelistofqixuan);
    void closeEvent(QCloseEvent *event);
    QXItem* FindQX(QList<Point3f> qixuans);
    WindDKItem* FindDKJL(QList<QList<QPointF>> dkjllines,QList<QList<QPointF>> dkjlareas);
    QBItem* FindQB(QList<QList<QPointF>> qiebianlines);
    ColdToughItem* FindCT(QList<QList<QPointF>> lengcaolines);
    FuGaoItem* FindFG(QList<QList<QPointF>> fugaolines);
private:
    QComboBox* two_time;
    QComboBox* two_pressure;
    QCheckBox* two_cyclone;
    QCheckBox* two_shear;
    QCheckBox* two_windpower;
    QCheckBox* two_winddirction;
    QCheckBox* two_height;
    QCheckBox* two_fugao;
    QCheckBox* two_trough;
    QCheckBox* two_flow;
    QTextBrowser* one_text;
    QPushButton* qixuandialog;

    QGraphicsScene* scene;
    int shadowtype;
    QList<QString> thelistofsavename;

    QList<QList<QList<QPointF>>> thelistofdkjlline;
    QList<QList<QList<QPointF>>> thelistofdkjlarea;
    QList<QList<QList<QPointF>>> thelistofqiebianline;
    QList<QList<QList<QPointF>>> thelistoflengcaoline;
    QList<QList<QList<QPointF>>> thelistoffugaoline;
    QList<QList<Point3f>> thelistofqixuan;

    int suoyin;
    QList<QString> namelist;

private slots:
    void ChooseCeng();
    void showqixuan();
    void showqiebian();
    void showdkjl();
    void showfugao();
    void showcoldtough();
    void showqixuanlife();
};

#endif // QDATADIALOG_H
