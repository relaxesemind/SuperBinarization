#ifndef RECTAREAMODEL_H
#define RECTAREAMODEL_H

#include "baseareamodel.h"
#include <QRectF>

struct RectAreaModel : public BaseAreaModel
{
    RectAreaModel() = default;
    ~RectAreaModel() = default;

    QVector<QPointF> points()override;
    QRectF rect;
};

#endif // RECTAREAMODEL_H
