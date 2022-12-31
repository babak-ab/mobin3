#include "buttonmodel.h"
#include "qqmlcontext.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>

int main(int argc, char* argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    ButtonModel* _buttons = new ButtonModel;
    _buttons->addButton(new Button("Button1", QPoint(500, 5)));
    _buttons->addButton(new Button("Button2", QPoint(900, 5)));
    _buttons->addButton(new Button("Button3", QPoint(1300, 5)));

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject* obj, const QUrl& objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);

    engine.rootContext()->setContextProperty("buttons", _buttons);

    engine.load(url);

    return app.exec();
}
