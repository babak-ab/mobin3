#include "appcontrol.h"

#include <QQmlContext>
#include <QGuiApplication>
#include <QQmlApplicationEngine>

int main(int argc, char* argv[])
{
    QGamepadManager *manager = QGamepadManager::instance();

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    AppControl app_control;

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));

    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject* obj, const QUrl& objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);

    qmlRegisterType<AppControl>("AppControl", 1, 0, "AppControl");
    qmlRegisterType<SerialControl>("SerialControl", 1, 0, "SerialControl");

    engine.rootContext()->setContextProperty("appControl", &app_control);

    engine.load(url);

    app.setWindowIcon(QIcon("://Images/Icon.png"));

    qApp->processEvents();

    GamepadController *gamepadController = new GamepadController();

    app_control.setGamepadController(gamepadController);
    app_control.setGamepadManager(manager);

    return app.exec();
}
