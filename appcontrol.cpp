#include "appcontrol.h"
#include <QCameraInfo>
#include <QCursor>
#include <QDebug>
#include <QFont>
#include <QFontMetrics>
#include <QProcess>
#include <QScreen>
#include <QStandardPaths>
#include <QUuid>
#include <iostream>

AppControl::AppControl(QObject* parent)
    : QObject(parent)
{

    _buttonModel = new ButtonModel;
    _buttonModel->addButton(new Button("Button1", QPoint(500, 5)));
    _buttonModel->addButton(new Button("Button2", QPoint(900, 5)));
    _buttonModel->addButton(new Button("Button3", QPoint(1300, 5)));
}

AppControl::~AppControl()
{
}

ButtonModel* AppControl::buttonModel() const
{
    return _buttonModel;
}

void AppControl::setButtonModel(ButtonModel* buttonModel)
{
    _buttonModel = buttonModel;
}
