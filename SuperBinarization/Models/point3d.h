#ifndef POINT3D_H
#define POINT3D_H

#include <QObject>

struct Point3D
{
    Point3D() = default;
    Point3D(qreal x, qreal y, qreal z) : x(x),y(y),z(z) {}
    qreal x, y, z;
};

#endif // POINT3D_H
