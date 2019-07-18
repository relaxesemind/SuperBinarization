#ifndef APPSTORAGE_H
#define APPSTORAGE_H


#include "Abstract/singletoneclass.h"
#include "Models/classmodel.h"
#include "Views/drawtool.h"

#include <QVector>

class AppStorage : public SingletonClass<AppStorage>
{
    friend class SingletonClass<AppStorage>;
    AppStorage() = default;

public:
    QVector<ClassModel> classModelsVector;
    DrawTool drawTool;
};

#endif // APPSTORAGE_H
