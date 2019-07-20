#ifndef BASEAREAMODEL_H
#define BASEAREAMODEL_H

#include <QVector>
#include <QPointF>


class BaseAreaModel
{
public:
    BaseAreaModel() = default;
    virtual ~BaseAreaModel() = default;
    virtual QVector<QPointF> points() = 0;
};

#endif // BASEAREAMODEL_H
