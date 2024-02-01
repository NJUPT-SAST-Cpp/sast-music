#pragma once
#include <QByteArray>
#include <QJsonDocument>
#include <QNetworkCookieJar>
#include <QNetworkRequest>
#include <QUrl>
#include <Utility/Result.hpp>
namespace NeteaseCloudMusic {

struct EncryptionBase {
    EncryptionBase() = delete;
    ~EncryptionBase() = delete;

    static Result<QNetworkRequest> prepareRequest(const QByteArray& verb, const QUrl& url, QNetworkCookieJar* cookieJar,
                                                  const QJsonDocument& data);
};

} // namespace NeteaseCloudMusic