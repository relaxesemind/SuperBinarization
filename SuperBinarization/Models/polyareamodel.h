#ifndef POLYAREAMODEL_H
#define POLYAREAMODEL_H

#include "baseareamodel.h"
#include <QPolygonF>
#include <QList>
#include <QLineF>
#include <QGraphicsLineItem>

struct PolyAreaModel : public BaseAreaModel
{
    PolyAreaModel() = default;
    ~PolyAreaModel() = default;

    QVector<QPointF> points()override;
    QPolygonF polygon;
    QList<QLineF> nativeLine;
};

using pPolyAreaModel = std::shared_ptr<PolyAreaModel>;

#endif // POLYAREAMODEL_H
