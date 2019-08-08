#ifndef MATHMANAGER_H
#define MATHMANAGER_H

#include <QRgb>
#include <QVector3D>

class MathManager
{
public:
    MathManager() = default;
    void rgb2lab( float R, float G, float B, float & l_s, float &a_s, float &b_s );
    void rgb2hsv(QRgb q, float &h, float &s, float &v);
    QVector3D projectionOfPointIntoPlane(const QVector3D& point,const QVector3D& normalVector);
    QVector3D Zspin(QVector3D vector, int degrees);
    QVector3D Xspin(QVector3D vector, int degrees);
    QVector3D Yspin(QVector3D vector, int degrees);
};

#endif // MATHMANAGER_H
