#ifndef BASEAREAMODEL_H
#define BASEAREAMODEL_H

#include <QVector>
#include <QPointF>
#include <QImage>
#include <memory>


class BaseAreaModel
{
public:
    BaseAreaModel() = default;
    virtual ~BaseAreaModel() = default;
    virtual QVector<QPointF> points() = 0;
    QImage imageArea;
};

#endif // BASEAREAMODEL_H
