#include "GamepadController.h"

GamepadController::GamepadController()
{
    m_gamepad = Q_NULLPTR;

    initialize();

    m_commandsBuffer = QMap<Commands, quint8>();

    m_processCommandsTimer.setInterval(TIMER_INTERVAL);
    connect(&m_processCommandsTimer, &QTimer::timeout,
            this, [this]()
    {
        processNextCommand();
    });

    m_illuminationTimer.setInterval(5000);
    connect(&m_illuminationTimer, &QTimer::timeout,
            this, [this](){
       m_illuminationTimer.stop();

       if (m_isLB_ButtonPressed == true &&
               m_isRB_ButtonPressed == true)
       {
           checkCommandAndAppendToBuffer(Commands::Command_ToggleIllumination,
                                         1.0);

           if (m_processCommandsTimer.isActive() == false)
           {
               m_processCommandsTimer.start();
           }
       }
    });

    m_isRB_ButtonPressed = false;
    m_isLB_ButtonPressed = false;

}

GamepadController::~GamepadController()
{

    if (m_gamepad != Q_NULLPTR)
    {
        removeConnections();
        delete m_gamepad;
        m_gamepad = Q_NULLPTR;
    }
}

void GamepadController::initialize()
{
    for (auto device : QGamepadManager::instance()->connectedGamepads())
    {
        if (m_gamepad != Q_NULLPTR)
        {
            delete m_gamepad;
            m_gamepad = Q_NULLPTR;
        }

        m_gamepad = new QGamepad(device);
        initialConnections();
    }
}

void GamepadController::initialConnections()
{
    connect(m_gamepad, &QGamepad::axisLeftXChanged,
            this, &GamepadController::sltAxisLeftXChanged);
    connect(m_gamepad, &QGamepad::axisLeftYChanged,
            this, &GamepadController::sltAxisLeftYChanged);

    connect(m_gamepad, &QGamepad::axisRightXChanged,
            this, &GamepadController::sltAxisRightXChanged);
    connect(m_gamepad, &QGamepad::axisRightYChanged,
            this, &GamepadController::sltAxisRightYChanged);

    connect(m_gamepad, &QGamepad::buttonAChanged,
            this, &GamepadController::sltButtonAChanged);
    connect(m_gamepad, &QGamepad::buttonBChanged,
            this, &GamepadController::sltButtonBChanged);
    connect(m_gamepad, &QGamepad::buttonXChanged,
            this, &GamepadController::sltButtonXChanged);
    connect(m_gamepad, &QGamepad::buttonYChanged,
            this, &GamepadController::sltButtonYChanged);

    connect(m_gamepad, &QGamepad::buttonStartChanged,
            this, &GamepadController::sltButtonStartChanged);
    connect(m_gamepad, &QGamepad::buttonSelectChanged,
            this, &GamepadController::sltButtonSelectChanged);
    connect(m_gamepad, &QGamepad::buttonGuideChanged,
            this, &GamepadController::sltButtonGuideChanged);
    connect(m_gamepad, &QGamepad::buttonCenterChanged,
            this, &GamepadController::sltButtonCenterChanged);

    connect(m_gamepad, &QGamepad::buttonL1Changed,
            this, &GamepadController::sltButtonL1Changed);
    connect(m_gamepad, &QGamepad::buttonL2Changed,
            this, &GamepadController::sltButtonL2Changed);
    connect(m_gamepad, &QGamepad::buttonL3Changed,
            this, &GamepadController::sltButtonL3Changed);

    connect(m_gamepad, &QGamepad::buttonR1Changed,
            this, &GamepadController::sltButtonR1Changed);
    connect(m_gamepad, &QGamepad::buttonR2Changed,
            this, &GamepadController::sltButtonR2Changed);
    connect(m_gamepad, &QGamepad::buttonR3Changed,
            this, &GamepadController::sltButtonR3Changed);

    connect(m_gamepad, &QGamepad::buttonUpChanged,
            this, &GamepadController::sltButtonUpChanged);
    connect(m_gamepad, &QGamepad::buttonDownChanged,
            this, &GamepadController::sltButtonDownChanged);
    connect(m_gamepad, &QGamepad::buttonRightChanged,
            this, &GamepadController::sltButtonRightChanged);
    connect(m_gamepad, &QGamepad::buttonLeftChanged,
            this, &GamepadController::sltButtonLeftChanged);
}

void GamepadController::removeConnections()
{
    disconnect(m_gamepad, &QGamepad::axisLeftXChanged,
               this, &GamepadController::sltAxisLeftXChanged);
    disconnect(m_gamepad, &QGamepad::axisLeftYChanged,
               this, &GamepadController::sltAxisLeftYChanged);

    disconnect(m_gamepad, &QGamepad::axisRightXChanged,
               this, &GamepadController::sltAxisRightXChanged);
    disconnect(m_gamepad, &QGamepad::axisRightYChanged,
               this, &GamepadController::sltAxisRightYChanged);

    disconnect(m_gamepad, &QGamepad::buttonAChanged,
               this, &GamepadController::sltButtonAChanged);
    disconnect(m_gamepad, &QGamepad::buttonBChanged,
               this, &GamepadController::sltButtonBChanged);
    disconnect(m_gamepad, &QGamepad::buttonXChanged,
               this, &GamepadController::sltButtonXChanged);
    disconnect(m_gamepad, &QGamepad::buttonYChanged,
               this, &GamepadController::sltButtonYChanged);

    disconnect(m_gamepad, &QGamepad::buttonStartChanged,
               this, &GamepadController::sltButtonStartChanged);
    disconnect(m_gamepad, &QGamepad::buttonSelectChanged,
               this, &GamepadController::sltButtonSelectChanged);
    disconnect(m_gamepad, &QGamepad::buttonGuideChanged,
               this, &GamepadController::sltButtonGuideChanged);

    disconnect(m_gamepad, &QGamepad::buttonL1Changed,
               this, &GamepadController::sltButtonL1Changed);
    disconnect(m_gamepad, &QGamepad::buttonL2Changed,
               this, &GamepadController::sltButtonL2Changed);
    disconnect(m_gamepad, &QGamepad::buttonL3Changed,
               this, &GamepadController::sltButtonL3Changed);

    disconnect(m_gamepad, &QGamepad::buttonR1Changed,
               this, &GamepadController::sltButtonR1Changed);
    disconnect(m_gamepad, &QGamepad::buttonR2Changed,
               this, &GamepadController::sltButtonR2Changed);
    disconnect(m_gamepad, &QGamepad::buttonR3Changed,
               this, &GamepadController::sltButtonR3Changed);

    disconnect(m_gamepad, &QGamepad::buttonUpChanged,
               this, &GamepadController::sltButtonUpChanged);
    disconnect(m_gamepad, &QGamepad::buttonDownChanged,
               this, &GamepadController::sltButtonDownChanged);
    disconnect(m_gamepad, &QGamepad::buttonRightChanged,
               this, &GamepadController::sltButtonRightChanged);
    disconnect(m_gamepad, &QGamepad::buttonLeftChanged,
               this, &GamepadController::sltButtonLeftChanged);
}

void GamepadController::keyHandler(const GamepadController::Buttons &button, double &value)
{
    bool shouldHandleCommand = deathBandMechanism(button, value);

    if (shouldHandleCommand == true)
    {
        commandCreator(button, value);
    }
}

void GamepadController::commandCreator(const GamepadController::Buttons &button,
                                       const double &value)
{
    Commands command = Commands::Command_Normal;

    switch (button)
    {
    case Button_RightAxisX:
    {
        if (value > 0)
        {
            command = Commands::Command_PanRight;
        }
        else if (value < 0)
        {
            command = Commands::Command_PanLeft;
        }
        else if (value == 0)
        {

            if (qAbs(m_gamepad->axisRightX()) <= DEATH_BAND_VALUE) {
                command = Commands::Command_PanStop;
            }
        }
        break;
    }
    case Button_RightAxisY:
    {
        if (value > 0)
        {
            command = Commands::Command_TiltDown;
        }
        else if (value < 0)
        {
            command = Commands::Command_TiltUp;
        }
        else if (value == 0)
        {

            if (qAbs(m_gamepad->axisRightY()) <= DEATH_BAND_VALUE) {
                command = Commands::Command_TiltStop;
            }
        }
        break;
    }
    case Button_RightAxisClick:
    {
        break;
    }
        // ==============================================
    case Button_LeftAxisX:
    {
        if (value > 0)
        {
            command = Commands::Command_FocusNear;
        }
        else if (value < 0)
        {
            command = Commands::Command_FocusFar;
        }
        else if (value == 0)
        {
            if (qAbs(m_gamepad->axisRightX()) <= DEATH_BAND_VALUE) {
                command = Commands::Command_FocusStop;
            }
        }
        break;
    }
    case Button_LeftAxisY:
    {
        if (value > 0)
        {
            command = Commands::Command_ZoomOut;
        }
        else if (value < 0)
        {
            command = Commands::Command_ZoomIn;
        }
        else if (value == 0)
        {
            if (qAbs(m_gamepad->axisRightY()) <= DEATH_BAND_VALUE) {
                command = Commands::Command_ZoomStop;
            }
        }
        break;
    }
    case Button_LeftAxisClick:
    {
        command = Commands::Command_ToggleFocusMode;
        break;
    }
        // ==============================================
    case Button_A:
    {
        command = Commands::Command_NextNoiseReductionLevel;
        break;
    }
    case Button_B:
    {
        command = Commands::Command_NextGammaLevel;
        break;
    }
    case Button_X:
    {
        command = Commands::Command_NextDefogLevel;
        break;
    }
    case Button_Y:
    {
        command = Commands::Command_NextCamera;
        break;
    }
        // ==============================================
    case Button_Up:
    {
        command = Commands::Command_MenuItemUp;
        break;
    }
    case Button_Down:
    {
        command = Commands::Command_MenuItemDown;
        break;
    }
    case Button_Left:
    {
        command = Commands::Command_MenuItemLeft;
        break;
    }
    case Button_Right:
    {
        command = Commands::Command_MenuItemRight;
        break;
    }
        // ==============================================
    case Button_RB:
    {
        if (value > 0)
        {
            m_isRB_ButtonPressed = true;
        }
        else
        {
            m_isRB_ButtonPressed = false;
        }

        break;
    }
    case Button_RT:
    {
        break;
    }
        // ==============================================
    case Button_LB:
    {
        if (value > 0)
        {
            m_isLB_ButtonPressed = true;
        }
        else
        {
            m_isLB_ButtonPressed = false;
        }

        break;
    }
    case Button_LT:
    {
        break;
    }
        // ==============================================
    case Button_Menu:
    {
        command = Commands::Command_OpenCameraMenu;
        break;
    }
    case Button_ChangeView:
    {
        command = Commands::Command_CloseCameraMenu;
        break;
    }
    case Button_XBoxHome:
    {
        command = Commands::Command_ShutdownSystem;
        break;
    }
    case Button_Center:
    {
        break;
    }
    }

    quint8 mappedValue = analogValueMapper(button, value);

    checkCommandAndAppendToBuffer(command, mappedValue);

    if (m_processCommandsTimer.isActive() == false)
    {
        m_processCommandsTimer.start();
    }
}

bool GamepadController::deathBandMechanism(const GamepadController::Buttons &button,
                                           double &value) const
{
    // death band for analog buttons
    if (button == GamepadController::Buttons::Button_LeftAxisX ||
            button == GamepadController::Buttons::Button_LeftAxisY ||
            button == GamepadController::Buttons::Button_RightAxisX ||
            button == GamepadController::Buttons::Button_RightAxisY ||
            button == GamepadController::Buttons::Button_LT ||
            button == GamepadController::Buttons::Button_RT)
    {
        if (qAbs(value) < DEATH_BAND_VALUE)
        {
            value = 0.0;
        }
    }

    return true;
}

void GamepadController::checkCommandAndAppendToBuffer(const Commands &command,
                                                      const quint8 &value)
{  
    m_commandsBuffer.insert(command, value);
}

quint8 GamepadController::analogValueMapper(const GamepadController::Buttons &button,
                                            const double &value)
{
    // death band for analog buttons
    if (button == GamepadController::Buttons::Button_LeftAxisX ||
            button == GamepadController::Buttons::Button_LeftAxisY ||
            button == GamepadController::Buttons::Button_RightAxisX ||
            button == GamepadController::Buttons::Button_RightAxisY ||
            button == GamepadController::Buttons::Button_LT ||
            button == GamepadController::Buttons::Button_RT)
    {
        if (qAbs(value) > DEATH_BAND_VALUE)
        {
            // value range is between 0.3 ~ 1.0
            // and, this code will change range
            // to between 0 ~ 255
            // using this formula:
            // m = ([255 / (1.0 - 0.3)] / 10)
            // newValue = (value - 0.3) * 10 * m -> (value - 0.3) * [(255 * 10) / 6]
            return static_cast<quint8>(qRound((qAbs(value) - DEATH_BAND_VALUE) * ((255 * 10) / 6)));
        }
        else
        {
            return static_cast<quint8>(0);
        }
    }
    else
    {
        return static_cast<quint8>(value);
    }
}

void GamepadController::processNextCommand()
{
    if (m_commandsBuffer.isEmpty() == false)
    {
        QMap<Commands, quint8>::iterator firstItem = m_commandsBuffer.begin();
        m_commandsBuffer.erase(firstItem);

        CommandPacket packet(firstItem.key(), firstItem.value());

        if (firstItem.key() == Command_PanStop
                && qAbs(m_gamepad->axisRightX()) > DEATH_BAND_VALUE)
            return;

        if (firstItem.key() == Command_TiltStop
                && qAbs(m_gamepad->axisRightY()) > DEATH_BAND_VALUE)
            return;

        if (firstItem.key() == Command_ZoomStop
                && qAbs(m_gamepad->axisLeftY()) > DEATH_BAND_VALUE)
            return;

        if (firstItem.key() == Command_FocusStop
                && qAbs(m_gamepad->axisLeftX()) > DEATH_BAND_VALUE)
            return;


        Q_EMIT sigExecuteCommandRequested(packet);
    }
    else
    {
        m_processCommandsTimer.stop();
    }
}

void GamepadController::sltAxisLeftXChanged(double value)
{
    keyHandler(GamepadController::Buttons::Button_LeftAxisX, value);
}

void GamepadController::sltAxisLeftYChanged(double value)
{
    keyHandler(GamepadController::Buttons::Button_LeftAxisY, value);
}

void GamepadController::sltAxisRightXChanged(double value)
{   
    keyHandler(GamepadController::Buttons::Button_RightAxisX, value);
}

void GamepadController::sltAxisRightYChanged(double value)
{    
    keyHandler(GamepadController::Buttons::Button_RightAxisY, value);
}

void GamepadController::sltButtonAChanged(bool value)
{
    double val;
    val = value;

    keyHandler(GamepadController::Buttons::Button_A, val);
}

void GamepadController::sltButtonBChanged(bool value)
{
    double val;
    val = value;

    keyHandler(GamepadController::Buttons::Button_B, val);
}

void GamepadController::sltButtonXChanged(bool value)
{
    double val;
    val = value;

    keyHandler(GamepadController::Buttons::Button_X, val);
}

void GamepadController::sltButtonYChanged(bool value)
{
    double val;
    val = value;

    keyHandler(GamepadController::Buttons::Button_Y, val);
}

void GamepadController::sltButtonStartChanged(bool value)
{
    double val;
    val = value;

    keyHandler(GamepadController::Buttons::Button_Menu, val);
}

void GamepadController::sltButtonSelectChanged(bool value)
{
    double val;
    val = value;

    keyHandler(GamepadController::Buttons::Button_ChangeView, val);
}

void GamepadController::sltButtonGuideChanged(bool value)
{
    double val;
    val = value;

    keyHandler(GamepadController::Buttons::Button_XBoxHome, val);
}

void GamepadController::sltButtonCenterChanged(bool value)
{
    double val;
    val = value;

    keyHandler(GamepadController::Buttons::Button_Center, val);
}

void GamepadController::sltButtonL1Changed(bool value)
{
    double val;
    val = value;

    keyHandler(GamepadController::Buttons::Button_LB, val);
}

void GamepadController::sltButtonL2Changed(double value)
{
    keyHandler(GamepadController::Buttons::Button_LT, value);
}

void GamepadController::sltButtonL3Changed(bool value)
{
    double val;
    val = value;

    keyHandler(GamepadController::Buttons::Button_LeftAxisClick, val);
}

void GamepadController::sltButtonR1Changed(bool value)
{
    double val;
    val = value;

    keyHandler(GamepadController::Buttons::Button_RB, val);
}

void GamepadController::sltButtonR2Changed(double value)
{
    keyHandler(GamepadController::Buttons::Button_RT, value);
}

void GamepadController::sltButtonR3Changed(bool value)
{
    double val;
    val = value;

    keyHandler(GamepadController::Buttons::Button_RightAxisClick, val);
}

void GamepadController::sltButtonUpChanged(bool value)
{
    double val;
    val = value;

    keyHandler(GamepadController::Buttons::Button_Up, val);
}

void GamepadController::sltButtonDownChanged(bool value)
{
    double val;
    val = value;

    keyHandler(GamepadController::Buttons::Button_Down, val);
}

void GamepadController::sltButtonRightChanged(bool value)
{
    double val;
    val = value;

    keyHandler(GamepadController::Buttons::Button_Right, val);
}

void GamepadController::sltButtonLeftChanged(bool value)
{
    double val;
    val = value;

    keyHandler(GamepadController::Buttons::Button_Left, val);
}

