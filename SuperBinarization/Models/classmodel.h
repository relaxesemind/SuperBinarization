#ifndef CLASSMODEL_H
#define CLASSMODEL_H

#include "Models/baseareamodel.h"

#include <QString>
#include <QColor>
#include <QVector>
#include <memory>

using pBaseAreaModel = std::shared_ptr<BaseAreaModel>;

class ClassModel
{
public:
    ClassModel() = default;
    QString className;
    QColor color;
    QVector<pBaseAreaModel> areas;
};

#endif // CLASSMODEL_H
