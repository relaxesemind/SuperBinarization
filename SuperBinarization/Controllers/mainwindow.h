#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <array>

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

private:
    Ui::MainWindow *ui;
    std::array<QGraphicsScene *, 3> componentsScenes;

private:
    void updateClassListWidget();
    void drawComponentsAxis();
};

#endif // MAINWINDOW_H
