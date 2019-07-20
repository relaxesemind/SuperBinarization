#include "polyareamodel.h"
#include "Common/magic.h"

QVector<QPointF> PolyAreaModel::points()
{
    if (!polygon.isEmpty())
    {
        return polygon;
    }

    if (!lineList.isEmpty())
    {
        QVector<QPointF> points;
        for_magic(line, lineList)
        {
            QLineF qline = *line;
            QPointF startPoint = qline.p1();
            points << startPoint;
        }
        return points;
    }

    return {};
}
