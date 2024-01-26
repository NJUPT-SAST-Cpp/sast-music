#include <QGuiApplication>
#include <QIcon>
#include <QQmlApplicationEngine>
#include <Service/NeteaseCloudMusic/CloudMusicClient.h>
#include <Service/PersistentCookieJar.h>
int main(int argc, char* argv[]) {
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
    qputenv("QT_QUICK_CONTROLS_STYLE", "Basic");
    QGuiApplication::setOrganizationName("NJUPT-SAST-C++");
    QGuiApplication::setOrganizationDomain("https://github.com/NJUPT-SAST");
    QGuiApplication::setApplicationName("SAST Music");

    QGuiApplication app(argc, argv);
    QGuiApplication::setWindowIcon(QIcon(QStringLiteral(":/res/icon/app.ico")));

    // Construct cloud music client after setting organization name and domain
    // to make sure QSerttings can work properly
    NeteaseCloudMusic::CloudMusicClient::getInstance()->setCookieJar(new PersistentCookieJar());
    NeteaseCloudMusic::CloudMusicClient::getInstance()->checkAnonimousToken([](Result<void>) {});

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/ui/MainWindow.qml"));
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreated, &app,
        [url](QObject* obj, const QUrl& objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load(url);
    return app.exec();
}
