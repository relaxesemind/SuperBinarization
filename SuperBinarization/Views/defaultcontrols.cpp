#include "defaultcontrols.h"
#include <QDebug>
#include <QMessageBox>
#include <QPushButton>

template<class Action>
void DefaultControls::showConfirmDialog(const QString &title, Action okAction)
{
    QMessageBox _box;
    QPushButton *_okBut = _box.addButton(QMessageBox::Ok);
    QPushButton *_cancelBut = _box.addButton(QMessageBox::Cancel);
    _box.setText(title);
    _box.exec();

    if (_box.clickedButton() == _okBut)
    {
        okAction();
    }

    if (_box.clickedButton() == _cancelBut)
    {
        qDebug() << "cancel";
    }
}

void DefaultControls::showMessage(const QString &title, const QString &description)
{
    QMessageBox::information(nullptr, title, description);
}
