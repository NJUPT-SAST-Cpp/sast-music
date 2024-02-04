#include <QFontDatabase>
#include <QGuiApplication>
#include <QIcon>
#include <QQmlApplicationEngine>
#include <Service/NeteaseCloudMusic/CloudMusicClient.h>
#include <Service/PersistentCookieJar.h>
#include <Utility/SettingsUtils.h>
#include <qguiapplication.h>

int main(int argc, char* argv[]) {
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
    qputenv("QT_QUICK_CONTROLS_STYLE", "Basic");
    QGuiApplication::setOrganizationName("NJUPT-SAST-C++");
    QGuiApplication::setOrganizationDomain("https://github.com/NJUPT-SAST");
    QGuiApplication::setApplicationName("SAST Music");

    QGuiApplication app(argc, argv);
    QGuiApplication::setWindowIcon(QIcon(QStringLiteral(":/res/icon/app.ico")));

    auto fontId = QFontDatabase::addApplicationFont(":/res/fonts/MiSans-Regular.ttf");
    QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);
    if (!fontFamilies.empty())
        QGuiApplication::setFont(QFont(fontFamilies[0]));

    // Construct cloud music client after setting organization name and domain
    // to make sure QSettings can work properly
    NeteaseCloudMusic::CloudMusicClient::getInstance()->setCookieJar(new PersistentCookieJar());
    NeteaseCloudMusic::CloudMusicClient::getInstance()->checkAnonimousToken([](Result<void>) {});

    auto settings = SettingsUtils::getInstance();
    if (settings->value("SongId").isNull())
        settings->setValue("SongId", 0);
    if (settings->value("Name").isNull())
        settings->setValue("Name", "");
    if (settings->value("Alias").isNull())
        settings->setValue("Alias", "");
    if (settings->value("Artists").isNull())
        settings->setValue("Artists", "");
    if (settings->value("Album").isNull())
        settings->setValue("Album", "");
    if (settings->value("ImgUrl").isNull())
        settings->setValue("ImgUrl", "qrc:///res/img/background.png");
    if (settings->value("Duration").isNull())
        settings->setValue("Duration", 0);
    if (settings->value("TimeStamp").isNull())
        settings->setValue("TimeStamp", 0);

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
