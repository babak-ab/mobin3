#ifndef GAMEPADCONTROLLER_GLOBAL_H
#define GAMEPADCONTROLLER_GLOBAL_H

#include <QtCore/qglobal.h>

enum ShiftButtonStates
{
    ShiftButtonState_Disable = 0x00,
    ShiftButtonState_Enable = 0x20
};

enum Control1_Commands
{
    Control1_Command_Normal = 0x00,
    Control1_Command_Unknown = 0x00,
    Control1_Command_ZoomIn = 0x01,
    Control1_Command_ZoomOut = 0x02,
    Control1_Command_FocusNear = 0x03,
    Control1_Command_FocusFar = 0x04,
    Control1_Command_OsdMenu = 0x05,
    Control1_Command_ToggleOsd = 0x06,
    Control1_Command_Calibration = 0x07,
    Control1_Command_ThermalOnOff = 0x08,
    Control1_Command_CameraDirect = 0x09,
    Control1_Command_GoPreset1 = 0x0A,
    Control1_Command_GoPreset2 = 0x0B,
    Control1_Command_ParkEnable = 0x0C,
    Control1_Command_ParkDisable = 0x0D,
    Control1_Command_GoZero = 0x0D,
    Control1_Command_StartTracking = 0x0E,
    Control1_Command_StopTracking = 0x0F,
    Control1_Command_IncreaseReticleSize = 0x10,
    Control1_Command_DecreaseReticleSize = 0x11,
    Control1_Command_IR_Polarity1 = 0x12,
    Control1_Command_PolarityBlackHot = 0x12,
    Control1_Command_IR_Polarity2 = 0x13,
    Control1_Command_PolarityWhiteHot = 0x13,
    Control1_Command_GainIncrease = 0x14,
    Control1_Command_GainDecrease = 0x15,
    Control1_Command_IrisIncrease = 0x16,
    Control1_Command_IrisDecrease = 0x17,
    Control1_Command_StabilizeEnable = 0x18,
    Control1_Command_StabilizeDisable = 0x19,
    Control1_Command_Measure = 0x1A,
};

enum TrackerType
{
    TrackerType_Unknown = 0x00,
    TrackerType_Correlation = 0x10,
    TrackerType_WhiteCentroid = 0x20,
    TrackerType_BlackCentroid = 0x30,
    TrackerType_MCorrlation = 0x40,
    TrackerType_DCentroid = 0x50,
    TrackerType_Hist = 0x60,
    TrackerType_Release = 0x70
};

enum Fine
{
    Fine_Unknown = -1,
    Fine_1 = 0x00,
    Fine_2 = 0x01,
    Fine_3 = 0x02,
    Fine_4 = 0x03,
    Fine_5 = 0x04,
    Fine_6 = 0x05
};

struct CommandData
{
    float pan = 0.0;
    float tilt = 0.0;

    ShiftButtonStates shift = ShiftButtonStates::ShiftButtonState_Disable;
    Control1_Commands command1 = Control1_Commands::Control1_Command_Unknown;

    bool switchCamera = false;
    Fine fine = Fine::Fine_Unknown;
    TrackerType trackerType = TrackerType::TrackerType_Unknown;
};

//#if defined(GAMEPADCONTROLLER_LIBRARY)
//#  define GAMEPADCONTROLLER_EXPORT Q_DECL_EXPORT
//#else
//#  define GAMEPADCONTROLLER_EXPORT Q_DECL_IMPORT
//#endif

#endif // GAMEPADCONTROLLER_GLOBAL_H
