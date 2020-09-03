#ifndef HANSHUKU_H
#define HANSHUKU_H

#include <QWidget>
#include <qmath.h>
#include <myitem.h>
#include <cv.h>
#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

QPoint ShadowXY(QList<double> point, int shadowtype);
QPoint DegreeToXY(double X,double Y);
void VectorXYToMapXY(vector<vector<Point>> point, QList<QList<QPointF>>*pointOnTheMap, int shadowtype, bool isbian, double lon_start, double lat_start);
void BCurve(QList<QPointF> originPoint, QList<QPointF> &curvePoint, bool isbian);
QList<QPolygonF> LineListToPolygon(QList<QList<QPointF>> list);

#endif // HANSHUKU_H

