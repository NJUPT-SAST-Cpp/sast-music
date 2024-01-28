#include "Encryption.hpp"
#include <QNetworkCookie>
#include <QNetworkCookieJar>
#include <Utility/CookieUtils.h>
#include <Utility/RandomUtils.h>
Result<QNetworkRequest> NeteaseCloudMusic::EncryptionBase::prepareRequest(const QByteArray& verb, const QUrl& url,
                                                                          QNetworkCookieJar* cookieJar,
                                                                          const QJsonDocument& data) {

    QNetworkRequest request;
    request.setRawHeader("Accept", "application/json, text/plain, */*");
    if (url.host().compare(QStringLiteral("music.163.com"), Qt::CaseInsensitive) == 0) {
        request.setRawHeader("Referer", "https://music.163.com");
    }
    QNetworkCookie cookieRememberMe("__remember_me", "true");
    cookieRememberMe.setDomain(QStringLiteral(".music.163.com"));
    cookieRememberMe.setPath(QStringLiteral("/"));
    cookieJar->insertCookie(cookieRememberMe);

    QNetworkCookie cookieChannel("_ntes_nuid", generateRandomHexString(32).toUtf8());
    cookieChannel.setDomain(QStringLiteral(".music.163.com"));
    cookieChannel.setPath(QStringLiteral("/"));
    cookieJar->insertCookie(cookieChannel);

    if (!url.path().contains(QStringLiteral("login"))) {
        QNetworkCookie cookieNMTID("NMTID", generateRandomHexString(32).toUtf8());
        cookieNMTID.setDomain(QStringLiteral(".music.163.com"));
        cookieNMTID.setPath(QStringLiteral("/"));
        cookieJar->insertCookie(cookieNMTID);
    }

    auto cookies = cookieJar->cookiesForUrl(url);
    if (!containsCookie(cookies, QStringLiteral("os"))) {
        QNetworkCookie cookieOs("os", "ios");
        cookieOs.setDomain(QStringLiteral(".music.163.com"));
        cookieOs.setPath(QStringLiteral("/"));
        cookieJar->insertCookie(cookieOs);
    }
    if (!containsCookie(cookies, QStringLiteral("appver"))) {
        QNetworkCookie cookieAppver("appver", "8.20.21");
        cookieAppver.setDomain(QStringLiteral(".music.163.com"));
        cookieAppver.setPath(QStringLiteral("/"));
        cookieJar->insertCookie(cookieAppver);
    }
    return request;
}