#include "appstorage.h"

ClassModel &AppStorage::currentModel()
{
    return classModelsVector[currentClassIndex];
}
