#include "cursor.h"
#include <QMouseEvent>
#include <QProcess>
#include <QStandardPaths>
#include <QtDebug>

Cursor::Cursor( QObject* parent)
    : QObject(parent)
{
    m_moveEventTimer.setInterval(10);

    connect(&m_moveEventTimer, &QTimer::timeout,
            this, &Cursor::sltMouseEventTimeout);

    m_moveEventTimer.start();
}
void Cursor::moveCursor(QPoint point)
{
    QCursor::setPos(point);
}

void Cursor::moveX(float x)
{
    m_joystick.setX(x);
}

void Cursor::moveY(float y)
{
    m_joystick.setY(y);
}

void Cursor::press()
{
    if (qApp->focusWindow())
    {
        QMouseEvent MouseEvent_Press(QEvent::MouseButtonPress,
                                     QPoint(QCursor::pos().x() - qApp->focusWindow()->x(),
                                            QCursor::pos().y() - qApp->focusWindow()->y()),
                                     Qt::LeftButton,
                                     Qt::LeftButton,
                                     0);
    }
    else
    {
        QMouseEvent MouseEvent_Release(QEvent::MouseButtonRelease,
                                       QPoint(QCursor::pos().x(),
                                              QCursor::pos().y()),
                                       Qt::LeftButton,
                                       0,
                                       0);
    }
}

void Cursor::release()
{
    if (qApp->focusWindow()) {
        QMouseEvent MouseEvent_Release(QEvent::MouseButtonRelease,
                                       QPoint(QCursor::pos().x() - qApp->focusWindow()->x(),
                                              QCursor::pos().y() - qApp->focusWindow()->y()),
                                       Qt::LeftButton,
                                       0,
                                       0);
    }
    else
    {
        QMouseEvent MouseEvent_Release(QEvent::MouseButtonRelease,
                                       QPoint(QCursor::pos().x(),
                                              QCursor::pos().y()),
                                       Qt::LeftButton, 0, 0);
    }
}

void Cursor::keyDownPress()
{
    QKeyEvent key(QEvent::KeyPress, Qt::Key_Down, Qt::NoModifier);
}

void Cursor::keyDownRelease()
{
    QKeyEvent key(QEvent::KeyRelease, Qt::Key_Down, Qt::NoModifier);
}

void Cursor::keyRightPress()
{
    QKeyEvent key(QEvent::KeyPress, Qt::Key_Right, Qt::NoModifier);
}

void Cursor::keyRightRelease()
{
    QKeyEvent key(QEvent::KeyRelease, Qt::Key_Right, Qt::NoModifier);
}

void Cursor::keyUpPress()
{
    QKeyEvent key(QEvent::KeyPress, Qt::Key_Up, Qt::NoModifier);
}

void Cursor::keyUpRelease()
{
    QKeyEvent key(QEvent::KeyRelease, Qt::Key_Up, Qt::NoModifier);
}

void Cursor::keyLeftPress()
{
    QKeyEvent key(QEvent::KeyPress, Qt::Key_Left, Qt::NoModifier);
}

void Cursor::keyLeftRelease()
{
    QKeyEvent key(QEvent::KeyRelease, Qt::Key_Left, Qt::NoModifier);
}

void Cursor::keyEnterPress()
{
    QKeyEvent key(QEvent::KeyPress, Qt::Key_Enter, Qt::NoModifier);
}

void Cursor::keyEnterRelease()
{
    QKeyEvent key(QEvent::KeyRelease, Qt::Key_Enter, Qt::NoModifier);
}

int Cursor::joyToPosition(float joystick)
{
    int scaleX = static_cast<int>(joystick * 32768);
    int cursor = 0;

    if (scaleX > axisDeadZone)
    {
        cursor = (scaleX - axisDeadZone) * axisScaler;
        cursor = cursor * (axisInitialSlope + secondSlope * cursor);
    }
    else if (scaleX < -axisDeadZone)
    {
        cursor = (scaleX + axisDeadZone) * axisScaler;
        cursor = cursor * (axisInitialSlope - secondSlope * cursor);
    }

    cursor = (cursor * 100 / 65536);
    return cursor;
}

void Cursor::sltMouseEventTimeout()
{
    QCursor::setPos(QCursor::pos().x() + joyToPosition(m_joystick.x()),
                    QCursor::pos().y() + joyToPosition(m_joystick.y()));
}

