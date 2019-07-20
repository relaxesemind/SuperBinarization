#include "rectareamodel.h"

QVector<QPointF> RectAreaModel::points()
{
    return {rect.bottomLeft(), rect.topLeft(), rect.topRight(), rect.bottomRight()};
}
