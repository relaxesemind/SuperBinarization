#ifndef APPSTORAGE_H
#define APPSTORAGE_H


#include "Abstract/singletoneclass.h"
#include "Models/classmodel.h"
#include "Views/drawtool.h"

#include <QVector>
#include <QMap>

class AppStorage : public SingletonClass<AppStorage>
{
    friend class SingletonClass<AppStorage>;
    AppStorage() = default;

public:
    QVector<ClassModel> classModelsVector;
    ClassModel &currentModel();
    int currentClassIndex;
    DrawTool drawTool;
};

#endif // APPSTORAGE_H
