#include "appcontrol.h"
#include "reticle.h"

#include <QQmlContext>
#include <QFontDatabase>
#include <QGuiApplication>
#include <QQmlApplicationEngine>

int main(int argc, char* argv[])
{
#ifdef WIN_32
    QGamepadManager *manager = QGamepadManager::instance();
#endif
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    int fontIndex = QFontDatabase::
            addApplicationFont(":/font/Helvetica.ttf");

    if (fontIndex <= 0)
    {
        std::cerr << "Cannot add application font.\n";
    }

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
    qmlRegisterType<Reticle>("Reticle", 1, 0, "Reticle");

    engine.rootContext()->setContextProperty("appControl", &app_control);

    engine.load(url);

    app.setWindowIcon(QIcon("://Images/Icon.png"));

    qApp->processEvents();

    GamepadController *gamepadController = new GamepadController();

    app_control.setGamepadController(gamepadController);

#ifdef WIN_32
    app_control.setGamepadManager(manager);
#endif
    return app.exec();
}
