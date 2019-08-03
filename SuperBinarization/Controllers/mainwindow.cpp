#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Views/defaultcontrols.h"
#include "Models/appstorage.h"
#include "Common/magic.h"
#include "Managers/managerslocator.h"

#include <QColorDialog>


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
//    image.load("/Users/ivanovegor/Desktop/Снимок экрана 2019-04-07 в 12.40.07.png");
    image.load("C:/Users/relaxes/Documents/MEPHI/46_KAF/primery_izobrazheniy_dlya_UIR/костный мозг  F0000055.bmp");
    ui->imageView->setImage(image.toImage());
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

void MainWindow::drawComponentsAxis()
{
    const int offset = 5;
    for (int i = 0; i < 3; ++i)
    {
        componentsScenes[i]->addLine(offset,offset,255 + offset, offset,QPen(Qt::black));
        componentsScenes[i]->addLine(offset,offset,offset,-255 + offset,QPen(Qt::black));
    }
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
}
