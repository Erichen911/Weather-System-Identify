#include <hanshuku.h>

////不同投影下的经纬度变换
QPoint ShadowXY(QList<double> point,int shadowtype)
{
    double x = point[0];
    double y = point[1];
    QPoint newpoint;
    switch (shadowtype) {
    case 1:
        newpoint.rx() = x*100;
        newpoint.ry() = -y*100;
        break;
    case 2:
        newpoint = DegreeToXY(x,y);
        break;
    case 3:
        newpoint.rx() = x*100;
        newpoint.ry() = -log(tan((90 + y) * M_PI / 360))/(M_PI / 180)*100;
        break;
    default:
        break;
    }
    return newpoint;
}

////等经纬度坐标To兰勃特坐标
QPoint DegreeToXY(double X,double Y)
{
    double x,y;
    double standardx = 110%360;
    double standardy = 90.0;
//            double centerx = X%360;
    double centery = (Y < -85.0) ? - 85.0 : (Y > 85.0) ? 85.0 :Y;
    double phi1 = qDegreesToRadians(30.0);
    double phi2 = qDegreesToRadians(60.0);
    double phi3 = qDegreesToRadians(45.0+30.0/2.0);
    double phi4 = qDegreesToRadians(45.0+60.0/2.0);
    double n = (qLn(qCos(phi1)/qCos(phi2)))/(qLn(qTan(phi4)/qTan(phi3)));
    double F = qCos(phi1)*pow(qTan(phi3),n)/n;
    double phi5 = qDegreesToRadians(45.0+centery/2.0);
    double phi6 = qDegreesToRadians(45.0+standardy/2.0);
    double r = qDegreesToRadians(n*(X-standardx));//perhaps
    double p = F * pow(1.0/qTan(phi5),n);
    double p0 = F * pow(1.0/qTan(phi6),n);

    x = p*qSin(r);
    y = p*qCos(r) - p0;

    x = x * 6000;
    y = y * 6000;

    return QPoint(x,y);
}

void VectorXYToMapXY(vector<vector<Point>> point, QList<QList<QPointF>> *pointOnTheMap, int shadowtype, bool isbian,double lon_start,double lat_start)
{
//    qDebug()<<point.size();
    for(int k = 0 ; k < point.size() ; k++)
    {
        QList<QPointF> temp;
        for(int i = 0 ; i < point[k].size();i++)
        {
            QList<double> temppoint;
            temppoint<<point[k][i].x*0.25+lon_start;
            temppoint<<lat_start-point[k][i].y*0.25;
            QPointF temppointnew;
            temppointnew = ShadowXY(temppoint,shadowtype);
            temp.push_back(temppointnew);
        }
        QList<QPointF> smoothtemp;
        BCurve(temp,smoothtemp,isbian);
        *pointOnTheMap<<temp;
    }
}

////三次B样条
void BCurve(QList<QPointF> originPoint,QList<QPointF> &curvePoint,bool isbian)
{
    curvePoint = originPoint;
//    double m1[4][4]={{-1,3,-3,1},{3,-6,3,0},{-3,0,3,0},{1,4,1,0}};
//    double m2[3][4]={{-1,3,-3,1},{2,-4,2,0},{-1,0,1,0}};

//    double temp0[1][3];
//    double temp1[1][4];
//    double temp2[4][2];
//    double temp3[1][2];
//    double u=0,du;
//    int k,length,flagu=0;
//    //    CString str;

//    length=originPoint.size()-1;

//    if(!isbian)//闭合
//    {
//        length=originPoint.size()-1;
//    }
//    else//开
//    {
//        length=originPoint.size()-3;
//    }


//    for(int j=0;j<length;j++)
//    {

//        if(!isbian)//闭合
//        {
//            for(int k=0;k<4;k++)
//            {
//                temp2[k][0]=originPoint[(j+k)%(originPoint.size()-1)].x();
//                temp2[k][1]=originPoint[(j+k)%(originPoint.size()-1)].y();
//            }
//        }
//        else//开
//        {
//            for(int k=0;k<4;k++)
//            {
//                temp2[k][0]=originPoint[j+k].x();
//                temp2[k][1]=originPoint[j+k].y();
//            }
//        }




//        while(u<=1)
//        {
//            temp1[0][0]=u*u*u;temp1[0][1]=u*u;temp1[0][2]=u;temp1[0][3]=1;

//            //        cvMatMul(temp1,m1,temp1);
//            double temp1temp[1][4];
//            temp1temp[0][0]=temp1[0][0]*m1[0][0]+temp1[0][1]*m1[1][0]+temp1[0][2]*m1[2][0]+temp1[0][3]*m1[3][0];
//            temp1temp[0][1]=temp1[0][0]*m1[0][1]+temp1[0][1]*m1[1][1]+temp1[0][2]*m1[2][1]+temp1[0][3]*m1[3][1];
//            temp1temp[0][2]=temp1[0][0]*m1[0][2]+temp1[0][1]*m1[1][2]+temp1[0][2]*m1[2][2]+temp1[0][3]*m1[3][2];
//            temp1temp[0][3]=temp1[0][0]*m1[0][3]+temp1[0][1]*m1[1][3]+temp1[0][2]*m1[2][3]+temp1[0][3]*m1[3][3];

//            //        cvConvertScale(temp1,temp1,1.0/6,0);
//            temp1[0][0] = temp1temp[0][0]/6;
//            temp1[0][1] = temp1temp[0][1]/6;
//            temp1[0][2] = temp1temp[0][2]/6;
//            temp1[0][3] = temp1temp[0][3]/6;

//            //        cvMatMul(temp1,temp2,temp3);
//            temp3[0][0] = temp1[0][0]*temp2[0][0]+temp1[0][1]*temp2[1][0]+temp1[0][2]*temp2[2][0]+temp1[0][3]*temp2[3][0];
//            temp3[0][1] = temp1[0][0]*temp2[0][1]+temp1[0][1]*temp2[1][1]+temp1[0][2]*temp2[2][1]+temp1[0][3]*temp2[3][1];

//            QPointF templinepoint;
//            templinepoint.rx()=temp3[0][0];
//            templinepoint.ry()=temp3[0][1];
//            curvePoint<<templinepoint;

//            if(flagu==1)
//            {
//                break;
//            }

//            //        cvmSet(temp0,0,0,u*u);cvmSet(temp0,0,1,u);cvmSet(temp0,0,2,1);
//            temp0[0][0] = u*u;
//            temp0[0][1] = u;
//            temp0[0][2] = 1;
//            //        cvMatMul(temp0,m2,temp1);
//            temp1temp[0][0]=temp0[0][0]*m2[0][0]+temp0[0][1]*m2[1][0]+temp0[0][2]*m2[2][0]+temp0[0][3]*m2[3][0];
//            temp1temp[0][1]=temp0[0][0]*m2[0][1]+temp0[0][1]*m2[1][1]+temp0[0][2]*m2[2][1]+temp0[0][3]*m2[3][1];
//            temp1temp[0][2]=temp0[0][0]*m2[0][2]+temp0[0][1]*m2[1][2]+temp0[0][2]*m2[2][2]+temp0[0][3]*m2[3][2];
//            temp1temp[0][3]=temp0[0][0]*m2[0][3]+temp0[0][1]*m2[1][3]+temp0[0][2]*m2[2][3]+temp0[0][3]*m2[3][3];
//            //        cvConvertScale(temp1,temp1,1.0/2,0);
//            temp1[0][0] = temp1temp[0][0]/2;
//            temp1[0][1] = temp1temp[0][1]/2;
//            temp1[0][2] = temp1temp[0][2]/2;
//            temp1[0][3] = temp1temp[0][3]/2;


//            //        cvMatMul(temp1,temp2,temp3);
//            temp3[0][0] = temp1[0][0]*temp2[0][0]+temp1[0][1]*temp2[1][0]+temp1[0][2]*temp2[2][0]+temp1[0][3]*temp2[3][0];
//            temp3[0][1] = temp1[0][0]*temp2[0][1]+temp1[0][1]*temp2[1][1]+temp1[0][2]*temp2[2][1]+temp1[0][3]*temp2[3][1];


//            du=sqrt(temp3[0][0]*temp3[0][0]+temp3[0][1]*temp3[0][1]);
//            du=0.1/du;
//            u=u+du;
//            if(u>1&&j==length-1)
//            {
//                u=1;
//                flagu=1;
//            }
//        }
//        u=du-(1-(u-du));//计算下一段的开始
//    }
}

QList<QPolygonF> LineListToPolygon(QList<QList<QPointF>> list)
{
    QList<QPolygonF> polygonlist;
    for(int i =0 ;i<list.size();i++)
    {
        QPolygonF polygon;
        for(int j = 0 ; j<list[i].size();j++)
        {
            polygon.push_back(list[i][j]);
        }
        polygonlist<<polygon;
    }
    return polygonlist;
}
