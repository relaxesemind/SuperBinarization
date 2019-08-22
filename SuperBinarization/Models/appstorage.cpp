#include "appstorage.h"
#include "Managers/managerslocator.h"

AppStorage::AppStorage()
{
    currentAngles = QVector3D(30,30,30);

    QVector3D vector = QVector3D(300,300,300);
    auto& math = ManagersLocator::shared().mathManager;
    vector = math.Xspin(vector,currentAngles.x());
    vector = math.Yspin(vector,currentAngles.y());
    vector = math.Zspin(vector,currentAngles.z());

    currentVisionVector = vector;

    points3D.insert(ColorModel::RGB,{});
    points3D.insert(ColorModel::HSV,{});
    points3D.insert(ColorModel::LAB,{});

    planeConsts = std::make_tuple(123, 1, -1, 1);
    currentAngleVector = 0;
    basisIsFilled = false;
    colorModel = ColorModel::RGB;
}

ClassModel& AppStorage::currentModel()
{
    return classModelsVector[currentClassIndex];
}
