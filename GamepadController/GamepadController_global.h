#ifndef GAMEPADCONTROLLER_GLOBAL_H
#define GAMEPADCONTROLLER_GLOBAL_H

#include <QtCore/qglobal.h>

enum Commands
{
    Command_Normal = -1,

    Command_ZoomIn,
    Command_ZoomOut,
    Command_ZoomStop,

    Command_FocusFar,
    Command_FocusNear,
    Command_FocusStop,

    Command_PanLeft,
    Command_PanRight,
    Command_PanStop,

    Command_TiltUp,
    Command_TiltDown,
    Command_TiltStop,
};

struct CommandPacket
{
    CommandPacket(const Commands &com,
                  const quint8 &val)
    {
        command = com;
        value = val;
    }

    Commands command;
    quint8 value;
};

//#if defined(GAMEPADCONTROLLER_LIBRARY)
//#  define GAMEPADCONTROLLER_EXPORT Q_DECL_EXPORT
//#else
//#  define GAMEPADCONTROLLER_EXPORT Q_DECL_IMPORT
//#endif

#endif // GAMEPADCONTROLLER_GLOBAL_H
