#ifndef APPSTORAGE_H
#define APPSTORAGE_H


#include "Abstract/singletoneclass.h"
#include "Models/classmodel.h"
#include "Views/drawtool.h"

#include <QVector>
#include <QVector3D>
#include <QMap>
#include <utility>


enum colorModel
{
    RGB, HSV, LAB
};

using vector6D = std::pair<QVector3D, QVector3D>;


class AppStorage : public SingletonClass<AppStorage>
{
    friend class SingletonClass<AppStorage>;
    AppStorage();

public:
    QVector<ClassModel> classModelsVector;
    ClassModel &currentModel();
    int currentClassIndex;
    DrawTool drawTool;
    QVector3D currentVisionVector;
    QVector3D currentAngles;
    QMap <colorModel,
          QVector<vector6D>
         > points3D;
};

#endif // APPSTORAGE_H
