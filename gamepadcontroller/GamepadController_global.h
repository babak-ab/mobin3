#ifndef GAMEPADCONTROLLER_GLOBAL_H
#define GAMEPADCONTROLLER_GLOBAL_H

#include <QtCore/qglobal.h>

enum Commands
{
    Command_Normal                          = -1,

    Command_ZoomIn                          = 0,
    Command_ZoomOut                         = 1,
    Command_ZoomStop                        = 2,

    Command_FocusFar                        = 3,
    Command_FocusNear                       = 4,
    Command_FocusStop                       = 5,
    Command_ToggleFocusMode                 = 6,

    Command_PanLeft                         = 7,
    Command_PanRight                        = 8,
    Command_PanStop                         = 9,

    Command_TiltUp                          = 10,
    Command_TiltDown                        = 11,
    Command_TiltStop                        = 12,

    Command_NextCamera                      = 13,
    Command_NextFilter                      = 14,

    Command_NextDefogLevel                  = 15,
    Command_NextGammaLevel                  = 16,
    Command_NextNoiseReductionLevel         = 17,

    Command_ToggleDigitalZoom               = 18,

    Command_OpenCameraMenu                  = 19,
    Command_MenuItemUp                      = 20,
    Command_MenuItemDown                    = 21,
    Command_MenuItemLeft                    = 22,
    Command_MenuItemRight                   = 23,

    Command_CloseCameraMenu                 = 24,

    Command_ShutdownSystem                  = 25,

    Command_ToggleIlluminator               = 26,
    Command_IncreaseAngleOfIlluminator      = 27,
    Command_IncreaseBrightnessOfIlluminator = 28
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
