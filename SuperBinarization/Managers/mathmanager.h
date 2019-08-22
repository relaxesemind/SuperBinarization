#ifndef MATHMANAGER_H
#define MATHMANAGER_H

#include <QRgb>
#include <QVector3D>
#include <QVector2D>
#include "Models/appstorage.h"

enum pointPosOverPlane
{
    front,
    behind,
    into
};

class MathManager
{
public:
    MathManager() = default;
    void rgb2lab( float R, float G, float B, float & l_s, float &a_s, float &b_s );
    void rgb2hsv(QRgb q, float &h, float &s, float &v);
    QVector3D projectionOfPointIntoPlane(const QVector3D& point,const QVector3D& normalVector);
    QPointF projectionInLocalCoordinates(QVector3D point);
    QVector3D Zspin(QVector3D vector, int degrees);
    QVector3D Xspin(QVector3D vector, int degrees);
    QVector3D Yspin(QVector3D vector, int degrees);
    QVector3D point3D(QRgb XY, QRgb YZ, QRgb XZ);
    pointPosOverPlane beyondThePlane(const QVector3D& point);
    QVector3D findMiddlePoint(ColorModel ColorModel);
    planeABCD defaultPlane(ColorModel ColorModel);
    QVector3D defaultVisionVector(ColorModel ColorModel);
    QVector3D rotateVisionVector(float angle);
};

#endif // MATHMANAGER_H
