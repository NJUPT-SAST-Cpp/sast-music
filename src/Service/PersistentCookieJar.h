#pragma once
#include <QMutex>
#include <QMutexLocker>
#include <QNetworkCookie>
#include <QNetworkCookieJar>
#include <QSettings>
class PersistentCookieJar : public QNetworkCookieJar {
public:
    PersistentCookieJar(QObject* parent = nullptr);
    ~PersistentCookieJar();
    virtual QList<QNetworkCookie> cookiesForUrl(const QUrl& url) const;
    virtual bool setCookiesFromUrl(const QList<QNetworkCookie>& cookieList, const QUrl& url);

private:
    void save();
    void load();
    QSettings settings;
    mutable QMutex mutex;
};
