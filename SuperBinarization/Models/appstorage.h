#ifndef APPSTORAGE_H
#define APPSTORAGE_H


#include "Abstract/singletoneclass.h"
#include "Models/classmodel.h"
#include "Views/drawtool.h"

#include <QVector>
#include <QVector3D>
#include <QMap>


enum colorModel
{
    RGB, HSV, LAB
};


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
    QMap<colorModel, QVector<QVector3D>> points3D;
};

#endif // APPSTORAGE_H
