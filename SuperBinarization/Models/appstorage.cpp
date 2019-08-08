#include "appstorage.h"

AppStorage::AppStorage()
{
    currentVisionVector = QVector3D(300,300,300);
}

ClassModel &AppStorage::currentModel()
{
    return classModelsVector[currentClassIndex];
}
