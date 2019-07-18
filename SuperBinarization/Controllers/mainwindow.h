#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

private:
    Ui::MainWindow *ui;

private:
    void updateClassListWidget();
};

#endif // MAINWINDOW_H
