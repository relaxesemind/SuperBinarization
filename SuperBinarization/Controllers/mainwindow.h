#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include "Models/classmodel.h"
#include <array>

enum components
{
    R,G,B,H,S,V,L,A,_b
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_addClassPushButton_clicked();

    void on_rectRadioButton_clicked(bool checked);

    void on_curveRadioButton_clicked(bool checked);

    void on_classListWidget_doubleClicked(const QModelIndex &index);

    void on_classListWidget_clicked(const QModelIndex &index);

    void on_radioButton_2_clicked(bool checked);

    void on_radioButton_clicked(bool checked);

    void on_radioButton_3_clicked(bool checked);

    void on_action_triggered();

    void on_tabWidget_currentChanged(int index);

    void on_horizontalSlider_valueChanged(int value);

    void on_horizontalSlider_2_valueChanged(int value);

    void on_horizontalSlider_3_valueChanged(int value);

    void on_lineEdit_editingFinished();

private:
    Ui::MainWindow *ui;
    std::array<QGraphicsScene *, 3> componentsScenes;

private:
    void updateClassListWidget();
    void drawComponentsAxis();
    void drawGraphs();
    QImage byTwoComponents(components comp1, components comp2);
    void drawRGB();
    void drawHSV();
    void drawLAB();
    void updateVisionVectorLabel(QVector3D vector);
};

#endif // MAINWINDOW_H
