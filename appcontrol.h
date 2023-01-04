#ifndef APPCONTROL_H
#define APPCONTROL_H
#define MONITOR1 true
#define MONITOR2 false
#define FRAME_WIDTH 1920
#define FRAME_HEIGHT 1080
#define SOFTWARE_VERSION "0.9.2"

#include "buttonmodel.h"
#include <QAbstractItemModel>
#include <QElapsedTimer>
#include <QFont>
#include <QFontMetrics>
#include <QGuiApplication>
#include <QObject>

class AppControl : public QObject {
    Q_OBJECT

    Q_PROPERTY(ButtonModel* buttonModel READ buttonModel NOTIFY signalVideoAdapter)
public:
    explicit AppControl(QObject* parent = Q_NULLPTR);
    ~AppControl();

private:
    ButtonModel* _buttonModel;

public:
    ButtonModel* buttonModel() const;
    void setButtonModel(ButtonModel* buttonModel);

private Q_SLOTS:

Q_SIGNALS:
    void signalVideoAdapter();
};

#endif // APPCONTROL_H
