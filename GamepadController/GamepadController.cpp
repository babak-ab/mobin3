#include "GamepadController.h"

GamepadController::GamepadController()
{
    m_gamepad = Q_NULLPTR;

    //    m_command = Commands::Unknown;

    //    m_timer = new QTimer;
    //    m_timer->setInterval(50);

    m_isAutoFocusEnabled = true;

    //    connect(m_timer, &QTimer::timeout,
    //            this, &GamepadController::sltTimerFinished);

    QTimer::singleShot(5000, this, [this]() {
        initialize();
    });

}

GamepadController::~GamepadController()
{

    if (m_gamepad != Q_NULLPTR)
    {
        removeConnections();
        delete m_gamepad;
        m_gamepad = Q_NULLPTR;
    }
    //    if (m_timer != Q_NULLPTR)
    //    {
    //        delete m_timer;
    //        m_timer = Q_NULLPTR;
    //    }
}

void GamepadController::initialize()
{
    qDebug() << " controller -- " << QGamepadManager::instance()->connectedGamepads().count();
    for (auto device : QGamepadManager::instance()->connectedGamepads())
    {
        qDebug() << " controller loop ";
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
    std::cerr << " game pad key handler " << std::endl;
    return;
    if (deathBand(button,  value))
    {
        if (m_pressedButtons.indexOf(button) == -1)
        {
            m_pressedButtons.append(button);
        }
    }
    else
    {
        if (m_pressedButtons.indexOf(button) != -1)
        {
            m_pressedButtons.removeAt(m_pressedButtons.indexOf(button));
        }
    }

    commandCreator();
}

bool GamepadController::deathBand(const Buttons &button,
                                  const double &value)
{
    if (button >= Buttons::Button_VirtualUp &&
            button <= Buttons::Button_VirtualLeft)
    {
        if (abs(value) > 0.9)
        {
            return true;
        }
    }
    else if (button == Buttons::Button_LT)
    {
        if (abs(value) > 0.9)
        {
            return true;
        }
    }
    else
    {
        if (button != Buttons::Button_RT)
        {
            if (abs(value) > 0.1)
            {
                return true;
            }
        }
        else
        {
            return true;
        }
    }

    return false;
}

void GamepadController::commandCreator()
{
    finalBufferFiller();

    CommandData data;

    /*
    * this code block wil fill, "pan" and "tilt" bytes of oghab's serial data protocol.
    *
    * .----------------------------------------------------------------------.
    * |  header   |    pan    |   tilt   | command 1 | command 2 | checksum  |
    * `----------------------------------------------------------------------`
    * `-----------------------------. 6 bytes .------------------------------`
    *
    */
    {
        if (isBufferContains(m_pressedButtonsFinal, Buttons::Button_LeftAxisMoves))
        {
            data.pan = m_leftAxisValue.x;
            data.tilt = m_leftAxisValue.y;
        }
    }

    /*
    * this code block will fill, "control 1 section of oghab's serial data protocol.
    *
    * .---------------------------------------.
    * |   lrf   | repeat  |  shift  | command |
    * `---------------------------------------`
    * | 1 bit   | 1 bit   | 1 bit   | 5 bits  |
    * `---------------------------------------`
    * `-------------. control 1 .-------------`
    *
    */
    {
        /*
        * this code block will fill, "shift" bit in control 1 section of oghab's serial data protocol.
        *
        * .-----------.
        * |   shift   |
        * `-----------`
        *
        */
        if (isBufferContains(m_pressedButtonsFinal, Buttons::Button_LT))
        {
            data.shift = ShiftButtonStates::ShiftButtonState_Enable;
        }

        /*
        * this code block will fill, "command" bits in control 1 section of oghab's serial data protocol.
        *
        * .-----------.
        * |  command  |
        * `-----------`
        *
        */
        if (isBufferContains(m_pressedButtonsFinal, Buttons::Button_VirtualUp))
        {
            data.command1 = Control1_Commands::Control1_Command_ZoomIn;
        }
        else if (isBufferContains(m_pressedButtonsFinal, Buttons::Button_VirtualDown))
        {
            data.command1 = Control1_Commands::Control1_Command_ZoomOut;
        }
        else if (isBufferContains(m_pressedButtonsFinal, Buttons::Button_VirtualRight))
        {
            data.command1 = Control1_Commands::Control1_Command_FocusNear;
        }
        else if (isBufferContains(m_pressedButtonsFinal, Buttons::Button_VirtualLeft))
        {
            data.command1 = Control1_Commands::Control1_Command_FocusFar;
        }
        else if (isBufferContains(m_pressedButtonsFinal, Buttons::Button_Left))
        {
            data.command1 = Control1_Commands::Control1_Command_GoPreset1;
        }
        else if (isBufferContains(m_pressedButtonsFinal, Buttons::Button_Right))
        {
            data.command1 = Control1_Commands::Control1_Command_GoPreset2;
        }
        else if (isBufferContains(m_pressedButtonsFinal, Buttons::Button_Y))
        {
            data.command1 = Control1_Commands::Control1_Command_StopTracking;
        }
        else if (isBufferContains(m_pressedButtonsFinal, Buttons::Button_B))
        {
            data.command1 = Control1_Commands::Control1_Command_StartTracking;
        }
        else if (isBufferContains(m_pressedButtonsFinal, Buttons::Button_RB) &&
                 isBufferContains(m_pressedButtonsFinal, Buttons::Button_LB))
        {
            data.command1 = Control1_Commands::Control1_Command_Measure;
        }
        else if (isBufferContains(m_pressedButtonsFinal, Buttons::Button_A))
        {
            data.command1 = Control1_Commands::Control1_Command_IR_Polarity1;
        }
        else if (isBufferContains(m_pressedButtonsFinal, Buttons::Button_X))
        {
            data.command1 = Control1_Commands::Control1_Command_IR_Polarity2;
        }
        else if (isBufferContains(m_pressedButtonsFinal, Buttons::Button_Up))
        {
            data.command1 = Control1_Commands::Control1_Command_GoZero;
        }
        else
        {
            data.command1 = Control1_Commands::Control1_Command_Unknown;
        }
    }

    /*
    * this block will fill, control 2 section of oghab's serial data protocol.
    *
    * .-------------------------------------------------------.
    * | camera type |    fine     |   reserve   | search type |
    * `-------------------------------------------------------`
    * | 1 bit       | 3 bits      | 1 bit       | 3 bits      |
    * `-------------------------------------------------------`
    * `---------------------. control 2 .---------------------`
    *
    */
    {

        /*
         *
         * this condition will fill "camera type" bit in control 2 section of oghab's serial data protocol.
         *
         * .-------------.
         * | camera type |
         * `-------------`
         *
        */
        if (isBufferContains(m_pressedButtonsFinal, Buttons::Button_Down))
        {
            data.switchCamera = true;
        }

        /*
         *
         * this condition will fill "fine" bits in control 2 section of oghab's serial data protocol.
         *
         * .-------------.
         * |    fine     |
         * `-------------`
         *
        */
        if (isBufferContains(m_pressedButtonsFinal, Buttons::Button_RT))
        {
            int value;
            value = static_cast<int>((m_rtValue * 30) / 6);

            data.fine = Fine(value);
        }
    }

    Q_EMIT sigExecuteCommandRequested(data);
}

bool GamepadController::isBufferContains(const QList<Buttons> &buffer,
                                         const GamepadController::Buttons &button) const
{
    if (buffer.indexOf(button) == -1)
    {
        return false;
    }
    else
    {
        return true;
    }
}

void GamepadController::finalBufferFiller()
{
    m_pressedButtonsFinal.clear();

    for (auto key : m_pressedButtons)
    {
        if (key == Buttons::Button_LeftAxisX || key == Buttons::Button_LeftAxisY)
        {
            if (!isBufferContains(m_pressedButtonsFinal, Buttons::Button_LeftAxisMoves))
            {
                m_pressedButtonsFinal.append(Buttons::Button_LeftAxisMoves);
            }
        }
        else
        {
            m_pressedButtonsFinal.append(key);
        }
    }
}

GamepadController::Buttons GamepadController::virtualButtonDetector(const float &value, const bool &isVertical) const
{
    if (isVertical)
    {
        if (value < 0.0)
        {
            return Buttons::Button_VirtualUp;
        }
        else
        {
            return Buttons::Button_VirtualDown;
        }
    }
    else
    {
        if (value > 0.0)
        {
            return Buttons::Button_VirtualRight;
        }
        else
        {
            return Buttons::Button_VirtualLeft;
        }
    }
}

void GamepadController::sltAxisLeftXChanged(double value)
{
    m_leftAxisValue.x = value;
    keyHandler(GamepadController::Buttons::Button_LeftAxisX, m_leftAxisValue.x);
}

void GamepadController::sltAxisLeftYChanged(double value)
{
    m_leftAxisValue.y = value;
    keyHandler(GamepadController::Buttons::Button_LeftAxisY, m_leftAxisValue.y);
}

void GamepadController::sltAxisRightXChanged(double value)
{   
    if (value != 0.0)
    {
        Buttons button;
        button = virtualButtonDetector(value, false);

        keyHandler(button, value);
    }
}

void GamepadController::sltAxisRightYChanged(double value)
{    
    if (value != 0.0)
    {
        Buttons button;
        button = virtualButtonDetector(value, true);

        keyHandler(button, value);
    }
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
    m_rtValue = value;

    keyHandler(GamepadController::Buttons::Button_RT, m_rtValue);
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

//void GamepadController::sltTimerFinished()
//{
//    Q_EMIT sigExecuteCommandRequested(m_command, m_arg);

//    if (m_command == Commands::Unknown)
//    {
//        m_timer->stop();
//    }
//}
