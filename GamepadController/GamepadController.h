#ifndef SIMORGHGAMEPADCONTROLLER_H
#define SIMORGHGAMEPADCONTROLLER_H

// **********************************************************************
// QT MODULES USED
// **********************************************************************
#include <QTimer>
#include <QDebug>
#include <QObject>
#include <QPointF>
#include <QLoggingCategory>
#include <QtGamepad/QGamepad>
#include <QtGamepad/QGamepadManager>

// **********************************************************************
// MODULES USED
// **********************************************************************
#include "GamepadController_global.h"

///
/// \brief third-party module to add gamepad shortcut feature to project
///
/// An individual module for detecting already connected gamepads to system ,connecting to
/// user desire gamepad ,detecting key events e.g key press or key release, detect shortcuts
/// based on conbination of pressed buttons and creates appropriate command to control oghab
/// platform.
///
class GamepadController : public QObject
{
    Q_OBJECT

public:
    // **********************************************************************
    // PUBLIC METHODS DECLARATION
    // **********************************************************************

    ///
    /// \brief default constructor.
    ///
    /// default constructor of SimorghGamepadController class
    ///
    GamepadController();

    ///
    /// \brief default destructor.S
    ///
    /// defauly destructor of SimorghGamepadController class
    ///
    ~GamepadController();

    ///
    /// \brief detect currently connect devices.
    ///
    /// this method is responsible for uptade list of currently connected joysticks
    ///
    void initialize();

private:
    // **********************************************************************
    // PRIVATE ATTRIBUTES DECLARATION
    // **********************************************************************
    QGamepad *m_gamepad;

    enum Buttons
    {
        //  Unknown = -1,

        Button_VirtualUp,
        Button_VirtualRight,
        Button_VirtualDown,
        Button_VirtualLeft,

        Button_RightAxisX,
        Button_RightAxisY,
        Button_RightAxisClick,

        Button_LeftAxisX,
        Button_LeftAxisY,
        Button_LeftAxisMoves,

        Button_LeftAxisClick,

        Button_A,
        Button_B,
        Button_X,
        Button_Y,

        Button_Up,
        Button_Down,
        Button_Left,
        Button_Right,

        Button_RB,
        Button_RT,

        Button_LB,
        Button_LT,

        Button_Menu, // Button_Start,
        Button_ChangeView, // Button_Select,
        Button_XBoxHome // Button_Guide
    };

    struct AxisValue
    {
        double x;
        double y;
    };

    QList<Buttons> m_pressedButtons;
    QList<Buttons> m_pressedButtonsFinal;

    AxisValue m_leftAxisValue;

    double m_rtValue;

    bool m_isAutoFocusEnabled;

    // **********************************************************************
    // PRIVATE METHODS DECLARATION
    // **********************************************************************
    void initialConnections();
    void removeConnections();

    void keyHandler(const Buttons &button, double &value);
    bool deathBand(const Buttons &button, const double &value);

    void commandCreator();

    bool isBufferContains(const QList<Buttons> &buffer, const Buttons &button) const;

    void finalBufferFiller();

    Buttons virtualButtonDetector(const float &value, const bool &isVertical) const;

Q_SIGNALS:
    void sigExecuteCommandRequested(const CommandData &commandData);

private Q_SLOTS:
    // **********************************************************************
    // PRIVATE SLOTS DECLARATION
    // **********************************************************************
    void sltAxisLeftXChanged(double value);
    void sltAxisLeftYChanged(double value);
    void sltAxisRightXChanged(double value);
    void sltAxisRightYChanged(double value);

    void sltButtonAChanged(bool value);
    void sltButtonBChanged(bool value);
    void sltButtonXChanged(bool value);
    void sltButtonYChanged(bool value);

    void sltButtonStartChanged(bool value);
    void sltButtonSelectChanged(bool value);
    void sltButtonGuideChanged(bool value);

    void sltButtonL1Changed(bool value);
    void sltButtonL2Changed(double value);
    void sltButtonL3Changed(bool value);

    void sltButtonR1Changed(bool value);
    void sltButtonR2Changed(double value);
    void sltButtonR3Changed(bool value);

    void sltButtonUpChanged(bool value);
    void sltButtonDownChanged(bool value);
    void sltButtonRightChanged(bool value);
    void sltButtonLeftChanged(bool value);

    //    void sltTimerFinished();
};



#endif // SIMORGHGAMEPADCONTROLLER_H
