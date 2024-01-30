#include "PersistentCookieJar.h"

PersistentCookieJar::PersistentCookieJar(QObject* parent) : QNetworkCookieJar(parent) {
    load();
}

PersistentCookieJar::~PersistentCookieJar() {
    save();
}

QList<QNetworkCookie> PersistentCookieJar::cookiesForUrl(const QUrl& url) const {
    QMutexLocker lock(&mutex);
    return QNetworkCookieJar::cookiesForUrl(url);
}

bool PersistentCookieJar::setCookiesFromUrl(const QList<QNetworkCookie>& cookieList, const QUrl& url) {
    QMutexLocker lock(&mutex);
    return QNetworkCookieJar::setCookiesFromUrl(cookieList, url);
}

void PersistentCookieJar::save() {
    QMutexLocker lock(&mutex);
    QList<QNetworkCookie> list = allCookies();
    QString data;
    foreach (QNetworkCookie cookie, list) {
        if (!cookie.isSessionCookie()) {
            data.append(cookie.toRawForm());
            data.append("\n");
        }
    }
    settings.setValue("Cookies", data);
}

void PersistentCookieJar::load() {
    QMutexLocker lock(&mutex);
    QByteArray data = settings.value("Cookies").toByteArray();
    setAllCookies(QNetworkCookie::parseCookies(data));
}
