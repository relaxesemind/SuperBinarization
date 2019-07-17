#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->menuBar->setNativeMenuBar(false);
//    QImage image(600,600,QImage::Format_RGB32);
//    image.fill(Qt::white);
    QPixmap image;
    image.load("/Users/ivanovegor/Desktop/Снимок экрана 2019-04-07 в 12.40.07.png");
    ui->imageView->setImage(image.toImage());
}

MainWindow::~MainWindow()
{
    delete ui;
}
