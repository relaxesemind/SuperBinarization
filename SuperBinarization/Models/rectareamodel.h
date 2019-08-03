#ifndef RECTAREAMODEL_H
#define RECTAREAMODEL_H

#include "baseareamodel.h"
#include <QRectF>
#include <QGraphicsRectItem>

struct RectAreaModel : public BaseAreaModel
{
    RectAreaModel() = default;
    ~RectAreaModel() = default;

    QVector<QPointF> points()override;
    QRectF rect;
};

using pRectAreaModel = std::shared_ptr<RectAreaModel>;

#endif // RECTAREAMODEL_H
