#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Views/defaultcontrols.h"
#include "Models/appstorage.h"
#include "Common/magic.h"
#include "Managers/managerslocator.h"
#include <QFileDialog>
#include <QColorDialog>

const float defaultWidht = 325.f;

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
    drawComponentsAxis();

    QPixmap image;
    image.load("/Users/ivanovegor/Desktop/Снимок экрана 2019-04-07 в 12.40.07.png");
//    image.load("C:/Users/relaxes/Documents/MEPHI/46_KAF/primery_izobrazheniy_dlya_UIR/костный мозг  F0000055.bmp");
    ui->imageView->setImage(image.toImage());

    ui->tabWidget->setMinimumWidth(defaultWidht);
    ui->tabWidget->setMaximumWidth(defaultWidht);


    updateVisionVectorLabel();
}

MainWindow::~MainWindow()
{
    delete ui;
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

    QGraphicsPixmapItem *item0 = componentsScenes[0]->addPixmap(QPixmap::fromImage(RG));
    item0->setPos(offset,-256);

    QGraphicsPixmapItem *item1 = componentsScenes[1]->addPixmap(QPixmap::fromImage(GB));
    item1->setPos(offset,-256);

    QGraphicsPixmapItem *item2 = componentsScenes[2]->addPixmap(QPixmap::fromImage(BR));
    item2->setPos(offset,-256);
}

void MainWindow::drawHSV()
{
    QImage RG = byTwoComponents(components::H, components::S);
    QImage GB = byTwoComponents(components::S, components::V);
    QImage BR = byTwoComponents(components::V, components::H);

    QGraphicsPixmapItem *item0 = componentsScenes[0]->addPixmap(QPixmap::fromImage(RG));
    item0->setPos(offset,-256);

    QGraphicsPixmapItem *item1 = componentsScenes[1]->addPixmap(QPixmap::fromImage(GB));
    item1->setPos(offset,-256);

    QGraphicsPixmapItem *item2 = componentsScenes[2]->addPixmap(QPixmap::fromImage(BR));
    item2->setPos(offset,-256);
}

void MainWindow::drawLAB()
{
    QImage RG = byTwoComponents(components::L, components::A);
    QImage GB = byTwoComponents(components::A, components::B);
    QImage BR = byTwoComponents(components::L, components::B);

    QGraphicsPixmapItem *item0 = componentsScenes[0]->addPixmap(QPixmap::fromImage(RG));
    item0->setPos(offset,-256);

    QGraphicsPixmapItem *item1 = componentsScenes[1]->addPixmap(QPixmap::fromImage(GB));
    item1->setPos(offset,-256);

    QGraphicsPixmapItem *item2 = componentsScenes[2]->addPixmap(QPixmap::fromImage(BR));
    item2->setPos(offset,-256);
}

void MainWindow::updateVisionVectorLabel()
{
    QVector3D vector = AppStorage::shared().currentVisionVector;
    QVector3D degrees = AppStorage::shared().currentAngles;

    QString X = QString::number(static_cast<int>(vector.x()));
    QString Y = QString::number(static_cast<int>(vector.y()));
    QString Z = QString::number(static_cast<int>(vector.z()));
    QString len = QString::number(static_cast<int>(vector.length()));

    QString _X = QString::number(vector.x() / vector.length(),'f',4);
    QString _Y = QString::number(vector.y() / vector.length(),'f',4);
    QString _Z = QString::number(vector.z() / vector.length(),'f',4);

    ui->label_8->setText("V = V(" + X + ", " + Y + ", " + Z + ")");
    ui->lineEdit->setText(len);
    ui->lineEdit_2->setText(_X);
    ui->lineEdit_3->setText(_Y);
    ui->lineEdit_4->setText(_Z);

    ui->horizontalSlider->setValue((int)degrees.x());
    ui->horizontalSlider_2->setValue((int)degrees.y());
    ui->horizontalSlider_3->setValue((int)degrees.z());
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


void MainWindow::on_horizontalSlider_valueChanged(int degrees)//X
{
    auto& vector = AppStorage::shared().currentVisionVector;
    int currentDegrees = (int)AppStorage::shared().currentAngles.x();
    int delta = degrees - currentDegrees;

    AppStorage::shared().currentAngles.setX(degrees);
    vector = ManagersLocator::shared().mathManager.Xspin(vector, delta);
    AppStorage::shared().currentVisionVector = vector;

    updateVisionVectorLabel();
}


void MainWindow::on_horizontalSlider_2_valueChanged(int degrees)//y
{
    auto& vector = AppStorage::shared().currentVisionVector;
    int currentDegrees = (int)AppStorage::shared().currentAngles.y();
    int delta = degrees - currentDegrees;

    AppStorage::shared().currentAngles.setY(degrees);
    vector = ManagersLocator::shared().mathManager.Yspin(vector, delta);
    AppStorage::shared().currentVisionVector = vector;

    updateVisionVectorLabel();
}

void MainWindow::on_horizontalSlider_3_valueChanged(int degrees)//z
{
    auto& vector = AppStorage::shared().currentVisionVector;
    int currentDegrees = (int)AppStorage::shared().currentAngles.z();
    int delta = degrees - currentDegrees;

    AppStorage::shared().currentAngles.setZ(degrees);
    vector = ManagersLocator::shared().mathManager.Zspin(vector, delta);
    AppStorage::shared().currentVisionVector = vector;

    updateVisionVectorLabel();
}

void MainWindow::on_lineEdit_editingFinished()//len
{
    QString text = ui->lineEdit->text();
    bool ok = false;
    int value = text.toInt(&ok);
    if (ok)
    {
        auto& vector = AppStorage::shared().currentVisionVector;
        vector *= value / vector.length();
        updateVisionVectorLabel();
    }
}

void MainWindow::on_pushButton_clicked()
{
    QVector3D point(124,122, 444);
    auto &V = AppStorage::shared().currentVisionVector;

    ManagersLocator::shared().mathManager.projectionInLocalCoordinates(V);
}
