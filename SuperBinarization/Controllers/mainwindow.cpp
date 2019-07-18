#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Views/defaultcontrols.h"
#include "Models/appstorage.h"
#include "Common/magic.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->menuBar->setNativeMenuBar(false);

    QPixmap image;
    image.load("/Users/ivanovegor/Desktop/Снимок экрана 2019-04-07 в 12.40.07.png");
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
        AppStorage::shared().classModelsVector.append(classModel);
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
        list->addItem(name);
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
