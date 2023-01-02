#ifndef CURSOR_H
#define CURSOR_H

#include "utils_global.h"
#include <QApplication>
#include <QCursor>
#include <QDesktopWidget>
#include <QMouseEvent>
#include <QObject>
#include <QSize>
#include <QTimer>
#include <QWidget>
#include <QWindow>

#define axisResolution 32768.0
#define axisDeadZone 0.1 * axisResolution
#define axisScaler axisResolution / (axisResolution - axisDeadZone)
#define axisInitialSlope 0.2
#define secondSlope (1 - axisInitialSlope) / axisResolution / 10

///
/// \brief The Cursor class handles the events created by the keyboard arrow keys.
///
class Cursor : public QObject {
    Q_OBJECT
public:
    explicit Cursor(QObject* parent = Q_NULLPTR);
    Q_INVOKABLE void moveCursor(QPoint point);
    Q_INVOKABLE void moveX(float x);
    Q_INVOKABLE void moveY(float y);
    Q_INVOKABLE void press();
    Q_INVOKABLE void release();

    Q_INVOKABLE void keyDownPress();
    Q_INVOKABLE void keyDownRelease();

    Q_INVOKABLE void keyRightPress();
    Q_INVOKABLE void keyRightRelease();

    Q_INVOKABLE void keyUpPress();
    Q_INVOKABLE void keyUpRelease();

    Q_INVOKABLE void keyLeftPress();
    Q_INVOKABLE void keyLeftRelease();

    Q_INVOKABLE void keyEnterPress();
    Q_INVOKABLE void keyEnterRelease();

private:
    QTimer m_moveEventTimer;
    int joyToPosition(float joystick);

    QPointF m_joystick;

private Q_SLOTS:
    void sltMouseEventTimeout();
};

#endif // CURSOR_H
