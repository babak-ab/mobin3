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

    m_isRB_ButtonPressed = false;
    m_isLB_ButtonPressed = false;
    m_isToggleIlluminatorCommandSent = false;

    previousRightAxisX = 0.0;
    previousRightAxisY = 0.0;
    previousLeftAxisX = 0.0;
    previousLeftAxisY = 0.0;
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

    quint8 mappedValue = 0;

    switch (button)
    {
    case Button_RightAxisX:
    {
        if (value > 0
                && qAbs(value - previousRightAxisX) > MIN_DIFF_VALUE)
        {
            command = Commands::Command_PanRight;

            previousRightAxisX = value;
        }
        else if (value < 0
                 && qAbs(value - previousRightAxisX) > MIN_DIFF_VALUE)
        {
            command = Commands::Command_PanLeft;

            previousRightAxisX = value;
        }
        else if (value == 0)
        {
            if (qAbs(m_gamepad->axisRightX()) <= DEATH_BAND_VALUE) {
                command = Commands::Command_PanStop;
            }
        }

        mappedValue = analogValueMapper(button, value);

        break;
    }
    case Button_RightAxisY:
    {
        if (value > 0
                && qAbs(value - previousRightAxisY) > MIN_DIFF_VALUE)
        {
            command = Commands::Command_TiltDown;

            previousRightAxisY = value;
        }
        else if (value < 0
                 && qAbs(value - previousRightAxisY) > MIN_DIFF_VALUE)
        {
            command = Commands::Command_TiltUp;

            previousRightAxisY = value;
        }
        else if (value == 0)
        {
            if (qAbs(m_gamepad->axisRightY()) <= DEATH_BAND_VALUE) {
                command = Commands::Command_TiltStop;
            }
        }

        mappedValue = analogValueMapper(button, value);

        break;
    }
    case Button_RightAxisClick:
    {
        mappedValue = analogValueMapper(button, value);

        break;
    }
        // ==============================================
    case Button_LeftAxisX:
    {

        if (value > 0
                && qAbs(value - previousLeftAxisX) > MIN_DIFF_VALUE)
        {
            // Make sure that the major axis move will be applied
            if (qAbs(m_gamepad->axisLeftX()) > qAbs(m_gamepad->axisLeftY()))
            {
                command = Commands::Command_FocusNear;

                previousLeftAxisX = value;
            }
        }
        else if (value < 0
                 && qAbs(value - previousLeftAxisX) > MIN_DIFF_VALUE)
        {
            // Make sure that the major axis move will be applied
            if (qAbs(m_gamepad->axisLeftX()) > qAbs(m_gamepad->axisLeftY()))
            {
                command = Commands::Command_FocusFar;

                previousLeftAxisX = value;
            }
        }
        else if (value == 0)
        {
            if (qAbs(m_gamepad->axisLeftX()) <= DEATH_BAND_VALUE
                    && qAbs(m_gamepad->axisLeftY()) <= DEATH_BAND_VALUE) {
                command = Commands::Command_FocusStop;
            }
        }

        mappedValue = analogValueMapper(button, value);

        break;
    }
    case Button_LeftAxisY:
    {

        if (value > 0
                && qAbs(value - previousLeftAxisY) > MIN_DIFF_VALUE)
        {
            // Make sure that the major axis move will be applied
            if (qAbs(m_gamepad->axisLeftY()) > qAbs(m_gamepad->axisLeftX()))
            {
                command = Commands::Command_ZoomOut;

                previousLeftAxisY = value;
            }
        }
        else if (value < 0
                 && qAbs(value - previousLeftAxisY) > MIN_DIFF_VALUE)
        {
            // Make sure that the major axis move will be applied
            if (qAbs(m_gamepad->axisLeftY()) > qAbs(m_gamepad->axisLeftX()))
            {
                command = Commands::Command_ZoomIn;

                previousLeftAxisY = value;
            }
        }
        else if (value == 0)
        {
            if (qAbs(m_gamepad->axisLeftY()) <= DEATH_BAND_VALUE
                    && qAbs(m_gamepad->axisLeftX()) <= DEATH_BAND_VALUE) {
                command = Commands::Command_ZoomStop;
            }
        }

        mappedValue = analogValueMapper(button, value);

        break;
    }
    case Button_LeftAxisClick:
    {
        command = Commands::Command_ToggleFocusMode;

        mappedValue = analogValueMapper(button, value);

        break;
    }
        // ==============================================
    case Button_A:
    {
        command = Commands::Command_NextNoiseReductionLevel;

        mappedValue = analogValueMapper(button, value);

        break;
    }
    case Button_B:
    {
        command = Commands::Command_NextGammaLevel;

        mappedValue = analogValueMapper(button, value);

        break;
    }
    case Button_X:
    {
        command = Commands::Command_NextDefogLevel;

        mappedValue = analogValueMapper(button, value);

        break;
    }
    case Button_Y:
    {
        command = Commands::Command_NextCamera;

        mappedValue = analogValueMapper(button, value);

        break;
    }
        // ==============================================
    case Button_Up:
    {
        command = Commands::Command_MenuItemUp;

        mappedValue = analogValueMapper(button, value);

        break;
    }
    case Button_Down:
    {
        command = Commands::Command_MenuItemDown;

        mappedValue = analogValueMapper(button, value);

        break;
    }
    case Button_Left:
    {
        command = Commands::Command_MenuItemLeft;

        mappedValue = analogValueMapper(button, value);

        break;
    }
    case Button_Right:
    {
        command = Commands::Command_MenuItemRight;

        mappedValue = analogValueMapper(button, value);

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

        if (m_isRB_ButtonPressed == true && m_isLB_ButtonPressed == true)
        {
            if (m_isToggleIlluminatorCommandSent == false)
            {
                command = Commands::Command_ToggleIlluminator;
            }
            else
            {
                return;
            }
        }
        else if (m_isRB_ButtonPressed == false || m_isLB_ButtonPressed == false)
        {
            if (m_isToggleIlluminatorCommandSent == true)
            {
                command = Commands::Command_ToggleIlluminator;
            }
            else
            {
                return;
            }
        }

        mappedValue = analogValueMapper(button, value);

        break;
    }
    case Button_RT:
    {
        if (m_isLB_ButtonPressed == true &&
                m_isRB_ButtonPressed == false)
        {
            command = Commands::Command_IncreaseAngleOfIlluminator;
        }

        mappedValue = analogValueMapper(button, value);

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

        if (m_isRB_ButtonPressed == true && m_isLB_ButtonPressed == true)
        {
            if (m_isToggleIlluminatorCommandSent == false)
            {
                command = Commands::Command_ToggleIlluminator;
            }
            else
            {
                return;
            }
        }
        else if (m_isRB_ButtonPressed == false || m_isLB_ButtonPressed == false)
        {
            if (m_isToggleIlluminatorCommandSent == true)
            {
                command = Commands::Command_ToggleIlluminator;
            }
            else
            {
                return;
            }
        }

        mappedValue = analogValueMapper(button, value);

        break;
    }
    case Button_LT:
    {
        if (m_isLB_ButtonPressed == false &&
                m_isRB_ButtonPressed == true)
        {
            command = Commands::Command_IncreaseBrightnessOfIlluminator;
        }

        mappedValue = analogValueMapper(button, value);

        break;
    }
        // ==============================================
    case Button_Menu:
    {
        command = Commands::Command_OpenCameraMenu;

        mappedValue = analogValueMapper(button, value);

        break;
    }
    case Button_ChangeView:
    {
        command = Commands::Command_CloseCameraMenu;

        mappedValue = analogValueMapper(button, value);

        break;
    }
    case Button_XBoxHome:
    {
        qDebug() << "shutdown system req";
        command = Commands::Command_ShutdownSystem;

        mappedValue = analogValueMapper(button, value);

        break;
    }
    case Button_Center:
    {
        mappedValue = analogValueMapper(button, value);

        break;
    }
    }

    if (command == Commands::Command_Normal)
    {
        return;
    }

    if (command == Commands::Command_ShutdownSystem)
    {
        qDebug() << button << value << mappedValue;
    }

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
            button == GamepadController::Buttons::Button_RightAxisY /*||
                                    button == GamepadController::Buttons::Button_LT ||
                                    button == GamepadController::Buttons::Button_RT*/)
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
            button == GamepadController::Buttons::Button_RightAxisY)
    {
        if (qAbs(value) > DEATH_BAND_VALUE)
        {
            return static_cast<quint8>(qAbs(value * 255));
        }
        else
        {
            return static_cast<quint8>(0);
        }
    }
    else if (button == GamepadController::Buttons::Button_LB ||
             button == GamepadController::Buttons::Button_RB)
    {
        if (m_isRB_ButtonPressed == true &&
                m_isLB_ButtonPressed == true)
        {
            if (m_isToggleIlluminatorCommandSent == false)
            {
                m_isToggleIlluminatorCommandSent = true;
                return static_cast<quint8>(true);
            }
        }

        if (m_isRB_ButtonPressed == false ||
                m_isLB_ButtonPressed == false)
        {
            if (m_isToggleIlluminatorCommandSent == true)
            {
                m_isToggleIlluminatorCommandSent = false;
                return static_cast<quint8>(false);
            }
        }
    }
    else if (button == GamepadController::Buttons::Button_LT)
    {
        return static_cast<quint8>(value * 255);
    }
    else if (button == GamepadController::Buttons::Button_RT)
    {
        // We would like to have values between 50~100
        return static_cast<quint8>((value * 50) + 50);
    }


    return static_cast<quint8>(value);
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


