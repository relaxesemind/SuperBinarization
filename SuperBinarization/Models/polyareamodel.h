#ifndef POLYAREAMODEL_H
#define POLYAREAMODEL_H

#include "baseareamodel.h"
#include <QPolygonF>
#include <QList>
#include <QLineF>

struct PolyAreaModel : public BaseAreaModel
{
    PolyAreaModel() = default;
    ~PolyAreaModel() = default;

    QVector<QPointF> points()override;
    QPolygonF polygon;
    QList<QLineF> lineList;
};

#endif // POLYAREAMODEL_H
