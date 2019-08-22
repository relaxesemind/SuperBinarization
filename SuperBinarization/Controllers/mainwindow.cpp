#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Views/defaultcontrols.h"
#include "Common/magic.h"
#include "Managers/managerslocator.h"
#include <QFileDialog>
#include <QColorDialog>

const float defaultWidht = 325.f;
bool planeRecalcFlag = true;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->menuBar->setNativeMenuBar(false);

    componentsScenes = {new QGraphicsScene(this), new QGraphicsScene(this), new QGraphicsScene(this)};
    ui->componentGraphicsView0->setScene(componentsScenes[0]);
    ui->componentGraphicsView1->setScene(componentsScenes[1]);
    ui->componentGraphicsView2->setScene(componentsScenes[2]);
    projectionScene = new QGraphicsScene(this);
    ui->graphicsView->setScene(projectionScene);
    drawComponentsAxis();

    QPixmap image;
//    image.load("/Users/ivanovegor/Desktop/maxresdefault.jpg");
    image.load("C:/Users/relaxes/Documents/MEPHI/46_KAF/primery_izobrazheniy_dlya_UIR/костный мозг  F0000055.bmp");
    ui->imageView->setImage(image.toImage());

    ui->tabWidget->setMinimumWidth(defaultWidht);
    ui->tabWidget->setMaximumWidth(defaultWidht);

    updatePlaneLabel();
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool isVectorWhite(const QVector3D& vector)
{
    return vector == QVector3D(255,255,254) || vector == QVector3D(255,255,255);
}

bool wtf_white(QRgb color)
{
    return color == 4294967293 or color == 4294967294;
}

void MainWindow::on_addClassPushButton_clicked()
{
    DefaultControls::shared().showStringInputDialog("Название класса: ",[this](const QString& name)
    {
        if (name.isEmpty())
        {
            return ;
        }

        ClassModel classModel;
        classModel.className = name;
        auto& storage = AppStorage::shared();
        classModel.color = ManagersLocator::shared().colorGenerator.get();
        storage.classModelsVector.append(classModel);
        storage.currentClassIndex = storage.classModelsVector.count() - 1;
        ui->label->setText("Текущий класс: " + name);
        this->updateClassListWidget();
    });
}

void MainWindow::updateClassListWidget()
{
    auto& data = AppStorage::shared().classModelsVector;
    auto& list = ui->classListWidget;

    list->clear();

    for_magic(it,data)
    {
        QString name = it->className;
        QColor color = it->color;
        QListWidgetItem *item = new QListWidgetItem(name);
        item->setBackgroundColor(color);
        item->setTextColor(Qt::white);
        list->addItem(item);
    }
}

const int offset = 5;

void MainWindow::drawComponentsAxis()
{
    for (int i = 0; i < 3; ++i)
    {
        QGraphicsLineItem *xAxis = componentsScenes[i]->addLine(offset,offset,255 + offset, offset,QPen(Qt::black));
        xAxis->setZValue(100);
        QGraphicsLineItem *yAxis =componentsScenes[i]->addLine(offset,offset,offset,-255 + offset,QPen(Qt::black));
        yAxis->setZValue(100);
    }
}

void MainWindow::drawGraphs()
{

    componentsScenes[0]->clear();
    componentsScenes[1]->clear();
    componentsScenes[2]->clear();

    drawComponentsAxis();

    bool isRGB = ui->radioButton_2->isChecked();
    bool isHSV = ui->radioButton->isChecked();
    bool isLAB = ui->radioButton_3->isChecked();

    if (isRGB)
    {
        drawRGB();
    }
    else if (isLAB)
    {
        drawLAB();
    }
    else
    {
        drawHSV();
    }
}

void MainWindow::byThreeComponents(ColorModel model)
{
    if (currentProjections[0].isNull() or currentProjections[1].isNull() or currentProjections[2].isNull())
    {
        return;
    }

    auto& allClasses = AppStorage::shared().classModelsVector;
    auto& points = *AppStorage::shared().points3D.find(model);
    points.clear();

    for_magic(class_iter,allClasses)
    {
        for_magic(p_area,class_iter->areas)
        {
            BaseAreaModel *area = p_area->get();
            QImage areaImage = area->imageArea;
            if (!areaImage.isNull())
            {
                for (int y = 0; y < areaImage.height(); ++y)
                    for (int x = 0; x < areaImage.width(); ++x)
                    {
                        QRgb pixel = areaImage.pixel(x,y);
                        if (wtf_white(pixel))
                        {
                            continue;
                        }

                        int r = qRed(pixel);
                        int g = qGreen(pixel);
                        int b = qBlue(pixel);

                        float _H, _S, _V;
                        float L, A, B;

                        if (model == ColorModel::RGB)
                        {
                            vector6D point;
                            point.first = QVector3D(r,g,b);
                            point.second = class_iter->color.rgb(); //currentProjections[0].pixel(r,g);
                            points.append(point);
                        }
                        else if (model == ColorModel::HSV)
                        {
                             ManagersLocator::shared().mathManager.rgb2hsv(pixel,_H,_S,_V);

                             int h = static_cast<int>((_H / 360.f) * 255.f);
                             int s = static_cast<int>(_S * 255.f);
                             int v = static_cast<int>(_V * 255.f);

                             vector6D point;
                             point.first = QVector3D(h,s,v);
                             point.second = currentProjections[0].pixel(h,s);
                             points.append(point);
                        }
                        else if (model == ColorModel::LAB)
                        {
                             ManagersLocator::shared().mathManager.rgb2lab(r,g,b,L,A,B);
                             int l = static_cast<int>(std::abs(L));
                             int a = static_cast<int>(std::abs(A));
                             int b = static_cast<int>(std::abs(B));

                             vector6D point;
                             point.first = QVector3D(l,a,b);
                             point.second = currentProjections[0].pixel(l,a);
                             points.append(point);
                        }

                    }
            }
        }
    }
}

QImage MainWindow::byTwoComponents(components comp1, components comp2)
{
    auto& allClasses = AppStorage::shared().classModelsVector;
    QImage result(256, 256, QImage::Format_RGB32);
    result.fill(Qt::white);

    QVector<QImage> classImagesRG;

    for_magic(class_iter,allClasses)
    {
        QImage temp(256, 256, QImage::Format_RGB32);
        temp.fill(Qt::white);

        for_magic(p_area,class_iter->areas)
        {
            BaseAreaModel *area = p_area->get();
            QImage areaImage = area->imageArea;

            if (!areaImage.isNull())
            {
                for (int y = 0; y < areaImage.height(); ++y)
                    for (int x = 0; x < areaImage.width(); ++x)
                    {
                       QRgb pixel = areaImage.pixel(x,y);
                       int v1, v2;

                       QRgb q = pixel;
                       float _H, _S, _V;
                       float L, A, B;

                       bool isHSV = comp1 == components::H or comp1 == components::S or comp1 == components::V;
                       bool isLAB = comp1 == components::L or comp1 == components::A or comp1 == components::_b;

                       if (isHSV)
                       {
                           ManagersLocator::shared().mathManager.rgb2hsv(q,_H,_S,_V);
                       }

                       if (isLAB)
                       {

                           ManagersLocator::shared().mathManager.rgb2lab(qRed(pixel),qGreen(pixel),qBlue(pixel),L,A,B);
                       }

                       switch (comp1)
                       {
                       case components::R: v1 = qRed(pixel); break;
                       case components::G: v1 = qGreen(pixel); break;
                       case components::B: v1 = qBlue(pixel); break;
                       case components::H: v1 = static_cast<int>((_H / 360.f) * 255.f); break;
                       case components::S: v1 = static_cast<int>(_S * 255.f); break;
                       case components::V: v1 = static_cast<int>(_V * 255.f); break;
                       case components::L: v1 = static_cast<int>(std::abs(L)); break;
                       case components::A: v1 = static_cast<int>(std::abs(A)); break;
                       case components::_b: v1 = static_cast<int>(std::abs(B)); break;
                       }

                       switch (comp2)
                       {
                       case components::R: v2 = qRed(pixel); break;
                       case components::G: v2 = qGreen(pixel); break;
                       case components::B: v2 = qBlue(pixel); break;
                       case components::H: v2 = static_cast<int>((_H / 360.f) * 255.f); break;
                       case components::S: v2 = static_cast<int>(_S * 255.f); break;
                       case components::V: v2 = static_cast<int>(_V * 255.f); break;
                       case components::L: v2 = static_cast<int>(std::abs(L)); break;
                       case components::A: v2 = static_cast<int>(std::abs(A)); break;
                       case components::_b: v2 = static_cast<int>(std::abs(B)); break;
                       }

                       temp.setPixel(v1,v2,class_iter->color.rgb());
                    }
            }
        }

       classImagesRG.append(temp);
    }

    for_magic(image,classImagesRG)
    {
        for (int y = 0; y < image->height(); ++y)
            for (int x = 0; x < image->width(); ++x)
            {
                result.setPixel(x,y,result.pixel(x,y) + image->pixel(x,y));
            }
    }

    return result;
}

void MainWindow::drawRGB()
{
    QImage RG = byTwoComponents(components::R, components::G);
    QImage GB = byTwoComponents(components::G, components::B);
    QImage BR = byTwoComponents(components::B, components::R);
    currentProjections[0] = RG;
    currentProjections[1] = GB;
    currentProjections[2] = BR;

    auto& storage = AppStorage::shared();
    auto& points = *storage.points3D.find(ColorModel::RGB);
    points.clear();

    for (int y = 0; y < RG.height(); ++y)
        for (int x = 0; x < RG.width(); ++x)
        {
            QRgb XY = RG.pixel(x,y);
            if (!wtf_white(XY))
            {

            }

            QVector3D point = ManagersLocator::shared().mathManager.point3D(XY,XY,XY);
        }

    QGraphicsPixmapItem *item0 = componentsScenes[0]->addPixmap(QPixmap::fromImage(RG));
    item0->setPos(offset,-256);

    QGraphicsPixmapItem *item1 = componentsScenes[1]->addPixmap(QPixmap::fromImage(GB));
    item1->setPos(offset,-256);

    QGraphicsPixmapItem *item2 = componentsScenes[2]->addPixmap(QPixmap::fromImage(BR));
    item2->setPos(offset,-256);

    storage.colorModel = ColorModel::RGB;
}

void MainWindow::drawHSV()
{
    QImage RG = byTwoComponents(components::H, components::S);
    QImage GB = byTwoComponents(components::S, components::V);
    QImage BR = byTwoComponents(components::V, components::H);

    currentProjections[0] = RG;
    currentProjections[1] = GB;
    currentProjections[2] = BR;

    QGraphicsPixmapItem *item0 = componentsScenes[0]->addPixmap(QPixmap::fromImage(RG));
    item0->setPos(offset,-256);

    QGraphicsPixmapItem *item1 = componentsScenes[1]->addPixmap(QPixmap::fromImage(GB));
    item1->setPos(offset,-256);

    QGraphicsPixmapItem *item2 = componentsScenes[2]->addPixmap(QPixmap::fromImage(BR));
    item2->setPos(offset,-256);
    AppStorage::shared().colorModel = ColorModel::HSV;
}

void MainWindow::drawLAB()
{
    QImage RG = byTwoComponents(components::L, components::A);
    QImage GB = byTwoComponents(components::A, components::B);
    QImage BR = byTwoComponents(components::L, components::B);

    currentProjections[0] = RG;
    currentProjections[1] = GB;
    currentProjections[2] = BR;

    QGraphicsPixmapItem *item0 = componentsScenes[0]->addPixmap(QPixmap::fromImage(RG));
    item0->setPos(offset,-256);

    QGraphicsPixmapItem *item1 = componentsScenes[1]->addPixmap(QPixmap::fromImage(GB));
    item1->setPos(offset,-256);

    QGraphicsPixmapItem *item2 = componentsScenes[2]->addPixmap(QPixmap::fromImage(BR));
    item2->setPos(offset,-256);
    AppStorage::shared().colorModel = ColorModel::LAB;
}


void MainWindow::updatePlaneLabel()
{
    QLabel *planeLabel = ui->label_18;
    planeABCD values = AppStorage::shared().planeConsts;
    QString A = QString::number(static_cast<int>(std::get<0>(values)));
    QString B = QString::number(static_cast<int>(std::get<1>(values)));
    QString C = QString::number(static_cast<int>(std::get<2>(values)));
    QString D = QString::number(static_cast<int>(std::get<3>(values)));

    QString a = A;
    QString b = B;
    QString c = C;
    QString d = D;

    B = *B.begin() == "-" ? "X " + B : "X + " + B;
    C = *C.begin() == "-" ? "Y " + C + "Z = " : "Y + " + C + "Z = ";

    planeLabel->setText(A + B + C + D);
    ui->lineEdit_5->setText(a);
    ui->lineEdit_6->setText(b);
    ui->lineEdit_7->setText(c);
    ui->lineEdit_8->setText(d);
}

void MainWindow::updateVisionLabel()
{
   QVector3D vector = AppStorage::shared().currentVisionVector;

   QString X = QString::number(static_cast<int>(vector.x()));
   QString Y = QString::number(static_cast<int>(vector.y()));
   QString Z = QString::number(static_cast<int>(vector.z()));

   ui->label_8->setText("V = V(" + X + ", " + Y + ", " + Z + ")");
}

void MainWindow::on_rectRadioButton_clicked(bool checked)
{
    AppStorage::shared().drawTool = DrawTool::Rect;
}

void MainWindow::on_curveRadioButton_clicked(bool checked)
{
    AppStorage::shared().drawTool = DrawTool::Curve;
}

void MainWindow::on_classListWidget_doubleClicked(const QModelIndex &index)
{
    auto& data = AppStorage::shared().classModelsVector;
    ClassModel &model = data[index.row()];

    QColor newColor = QColorDialog::getColor(model.color,this,"Выберите новый цвет класса");
    if (newColor.isValid())
    {
        model.color = newColor;
        updateClassListWidget();
        ui->imageView->updateWithCurrentClass(model);
    }
}

void MainWindow::on_classListWidget_clicked(const QModelIndex &index)
{
    auto& storage = AppStorage::shared();
    storage.currentClassIndex = index.row();
    ClassModel& model = storage.classModelsVector[index.row()];
    ui->imageView->updateWithCurrentClass(model);
    ui->label->setText("Текущий класс: " + model.className);
    drawGraphs();
}

void MainWindow::on_radioButton_2_clicked(bool checked)
{
    ui->label_2->setText("R");
    ui->label_3->setText("G");
    ui->label_4->setText("G");
    ui->label_5->setText("B");
    ui->label_6->setText("B");
    ui->label_7->setText("R");
    drawGraphs();
}

void MainWindow::on_radioButton_clicked(bool checked)
{
    ui->label_2->setText("H");
    ui->label_3->setText("S");
    ui->label_4->setText("S");
    ui->label_5->setText("V");
    ui->label_6->setText("V");
    ui->label_7->setText("H");
    drawGraphs();
}

void MainWindow::on_radioButton_3_clicked(bool checked)
{
    ui->label_2->setText("L");
    ui->label_3->setText("A");
    ui->label_4->setText("A");
    ui->label_5->setText("B");
    ui->label_6->setText("L");
    ui->label_7->setText("B");
    drawGraphs();
}

void MainWindow::on_action_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(nullptr, "Выберите изображение", "", "*.jpg *.jpeg *.bmp *.png");
    if (!fileName.isEmpty())
    {
        QPixmap pixmap;
        pixmap.load(fileName);
        ui->imageView->setImage(pixmap.toImage());
    }
}


void MainWindow::on_tabWidget_currentChanged(int index)
{
    if (index == 2)
    {
        ui->tabWidget->setMaximumWidth(defaultWidht + 150);
        ui->tabWidget->setMinimumWidth(defaultWidht + 150);
    }
    else
    {
        ui->tabWidget->setMinimumWidth(defaultWidht);
        ui->tabWidget->setMaximumWidth(defaultWidht);
    }
}


//void MainWindow::on_horizontalSlider_3_valueChanged(int degrees)//z
//{
//    auto& vector = AppStorage::shared().currentVisionVector;
//    int currentDegrees = (int)AppStorage::shared().currentAngles.z();
//    int delta = degrees - currentDegrees;

//    AppStorage::shared().currentAngles.setZ(degrees);
//    vector = ManagersLocator::shared().mathManager.Zspin(vector, delta);
//    AppStorage::shared().currentVisionVector = vector;

//    updateVisionVectorLabel();
//}

QRgb highlightColor(QRgb color)
{
    QColor c(color);
    const int high = 25;
    c.setRed(c.red() + high > 255 ? 255 : c.red() + high);
    c.setGreen(c.green() + high > 255 ? 255 : c.green() + high);
    c.setBlue(c.blue() + high > 255 ? 255 : c.blue() + high);

    return c.rgb();
}

QRgb rgbFromVector3D(const QVector3D& vector)
{
    QColor color;
    color.setRed(vector.x());
    color.setGreen(vector.y());
    color.setBlue(vector.z());

    return color.rgb();
}

void MainWindow::on_pushButton_clicked()
{
    auto& storage = AppStorage::shared();
    auto& math = ManagersLocator::shared().mathManager;
    auto& beyonded = storage.beyondedRgb;
    auto& redLineBasis = storage.redLineBasis;
    beyonded.clear();
    redLineBasis.clear();
    ColorModel color_model = storage.colorModel;
    auto& points = *storage.points3D.find(color_model);
    byThreeComponents(color_model);
    QImage sourceImage = ui->imageView->getImage();

    QImage result(256, 256, QImage::Format_RGB32);
    result.fill(Qt::white);
    projectionScene->clear();

    if (points.isEmpty())
    {
        return;
    }


    if (planeRecalcFlag)
    {
        storage.planeConsts = math.defaultPlane(color_model);
        storage.currentVisionVector = math.defaultVisionVector(color_model);
        ui->horizontalSlider->setValue(0);
        planeRecalcFlag = false;
    }

    updatePlaneLabel();
    updateVisionLabel();

    for_magic(point, points)
    {
        vector6D p = *point;
        QRgb color = p.second;
        QVector3D projectPoint = math.projectionOfPointIntoPlane(p.first, storage.currentVisionVector);
        pointPosOverPlane pointPosition = math.beyondThePlane(projectPoint);
        QPoint local = math.projectionInLocalCoordinates(projectPoint).toPoint();

//        if (pointPosition == pointPosOverPlane::front)
//        {
//            color = QColor(Qt::black).rgb(); //highlightColor(color);
//            QRgb rgb = rgbFromVector3D(p.first);
//            beyonded.append(rgb);
//        }

        if (local.x() > result.width() - 1 or local.x() < 0 or
                local.y() < 0 or local.y() > result.height() - 1)
        {
            continue;
        }

        if (pointPosition == pointPosOverPlane::into)
        {
           storage.redLineBasis.append(local);
        }

        int x = local.x();
        int y = local.y();

        result.setPixel(x,y,color);
    }

    projectionScene->addPixmap(QPixmap::fromImage(result));

    if (storage.redLineBasis.count() >= 2)
    {
        QPointF p1 = storage.redLineBasis.first();
        QPointF p2 = storage.redLineBasis.last();
        if (p1 != p2 and p2 != QPointF(0,0))
        {
            while (p2.x() < result.width() and p2.y() < result.height())
            {
                p2 *= 1.1;
            }

            if (p1 != QPointF(0,0))
            {
                while (p1.x() > 0 and p1.y() > 0)
                {
                    p1 -= QPointF(1,1);
                }
            }

            QPen pen(QBrush(QColor(Qt::red)),3,Qt::SolidLine,Qt::RoundCap);
            projectionScene->addLine({p1, p2}, pen);
        }
    }
}

void MainWindow::on_lineEdit_5_editingFinished()
{
    auto& currentA = std::get<0>(AppStorage::shared().planeConsts);
    QString text = ui->lineEdit_5->text();
    bool ok;
    int value = text.toInt(&ok);
    if (ok)
    {
        currentA = value;
        updatePlaneLabel();
       // planeRecalcFlag = true;
    }
}

void MainWindow::on_lineEdit_6_editingFinished()
{
    auto& currentA = std::get<1>(AppStorage::shared().planeConsts);
    QString text = ui->lineEdit_6->text();
    bool ok;
    int value = text.toInt(&ok);
    if (ok)
    {
        currentA = value;
        updatePlaneLabel();
      //  planeRecalcFlag = true;
    }
}

void MainWindow::on_lineEdit_8_editingFinished()
{
    auto& currentA = std::get<3>(AppStorage::shared().planeConsts);
    QString text = ui->lineEdit_8->text();
    bool ok;
    int value = text.toInt(&ok);
    if (ok)
    {
        currentA = value;
        updatePlaneLabel();
       // planeRecalcFlag = true;
    }

}

void MainWindow::on_lineEdit_7_editingFinished()
{
    auto& currentA = std::get<2>(AppStorage::shared().planeConsts);
    QString text = ui->lineEdit_7->text();
    bool ok;
    int value = text.toInt(&ok);
    if (ok)
    {
        currentA = value;
        updatePlaneLabel();
       // planeRecalcFlag = true;
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->imageView->showAllClasses();
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    auto& math = ManagersLocator::shared().mathManager;
    auto& vector = AppStorage::shared().currentVisionVector;
    int delta = value - AppStorage::shared().currentAngleVector;
    if (delta == 0)
    {
        return;
    }

    AppStorage::shared().currentAngleVector += delta;
    vector = math.rotateVisionVector(delta);
    updateVisionLabel();
    ui->label_10->setText(QString::number(value));
}















