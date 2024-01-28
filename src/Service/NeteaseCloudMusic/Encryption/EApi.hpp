#pragma once
#include "Encryption.hpp"
#include <QByteArray>
#include <QCryptographicHash>
#include <QJsonDocument>
#include <QJsonObject>
#include <Utility/CookieUtils.h>
#include <Utility/EncryptionUtils.h>
#include <Utility/RandomUtils.h>
namespace NeteaseCloudMusic {
namespace EApiDetails {
QByteArray eapiKey = "e82ckenh8dichen8";
}
struct EApi {
public:
    EApi() = delete;
    ~EApi() = delete;
    static Result<std::tuple<QNetworkRequest, QByteArray>>
        makeRequest(const QByteArray& verb, const QUrl& url, QNetworkCookieJar* cookieJar, const QJsonDocument& data) {
        auto requestResult = EncryptionBase::prepareRequest(verb, url, cookieJar, data);
        if (requestResult.isErr()) {
            return requestResult.takeErr();
        }
        auto request = requestResult.unwrap();

        auto cookies = cookieJar->cookiesForUrl(url);
        auto dataObject = data.object();
        QString requestId = QString::number(QDateTime::currentMSecsSinceEpoch()) + "_" +
                            QString::number(randomInt(0, 9999)).rightJustified(4, '0');
        QJsonObject header = QJsonObject{
            {"osver", getCookieOrDefault(cookies, "osver", "17,1,2")},
            {"deviceId", getCookieOrDefault(cookies, "deviceId", "")},
            {"appver", getCookieOrDefault(cookies, "appver", "8.9.70")},
            {"versioncode", getCookieOrDefault(cookies, "versioncode", "140")},
            {"mobilename", getCookieOrDefault(cookies, "mobilename", "iPhone")},
            {"buildver", getCookieOrDefault(cookies, "buildver", QString::number(QDateTime::currentSecsSinceEpoch()))},
            {"resolution", getCookieOrDefault(cookies, "resolution", "1920x1080")},
            {"__csrf", getCookieOrDefault(cookies, "__csrf", "")},
            {"os", getCookieOrDefault(cookies, "os", "ios")},
            {"channel", getCookieOrDefault(cookies, "channel", "")},
            {"requestId", requestId},
        };
        if (containsCookie(cookies, "MUSIC_U")) {
            header.insert("MUSIC_U", getCookieOrDefault(cookies, "MUSIC_U", ""));
        }
        if (containsCookie(cookies, "MUSIC_A")) {
            header.insert("MUSIC_A", getCookieOrDefault(cookies, "MUSIC_A", ""));
        }
        dataObject.insert("header", header);

        auto urlPath = url.path(QUrl::FullyEncoded).toUtf8();
        urlPath.replace("/eapi", "/api");
        auto dataBytes = QJsonDocument{dataObject}.toJson(QJsonDocument::Compact);
        auto message = "nobody" + urlPath + "use" + dataBytes + "md5forencrypt";
        auto digest = QCryptographicHash::hash(message, QCryptographicHash::Md5).toHex();
        auto encodedData = urlPath + "-36cd479b6b5-" + dataBytes + "-36cd479b6b5-" + digest;
        auto params = aesEcbEncrypt(encodedData, EApiDetails::eapiKey);
        if (params.isErr()) {
            return params.takeErr();
        }

        // update cookies
        for (auto it = header.begin(); it != header.end(); ++it) {
            if (!containsCookie(cookies, it.key())) {
                QNetworkCookie cookieItem(it.key().toUtf8(), it.value().toString().toUtf8());
                cookieItem.setDomain(QStringLiteral(".music.163.com"));
                cookieItem.setPath(QStringLiteral("/"));
                cookieJar->insertCookie(cookieItem);
            }
        }

        if (verb.compare("POST", Qt::CaseInsensitive) == 0) {
            request.setUrl(url);
            request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
            return std::make_tuple(request, "params=" + params.take().toHex());
        } else {
            request.setUrl(QStringLiteral("%1?params=%2").arg(url.toString(QUrl::RemoveQuery), params.take().toHex()));
            return std::make_tuple(request, QByteArray());
        }
    }
    static Result<QByteArray> decryptResponse(const QByteArray& contentType, QByteArray&& content) {
        if (contentType.contains("application/json")) {
            return content;
        }
        return aesEcbDecrypt(QByteArray::fromHex(content), EApiDetails::eapiKey);
    }
};
} // namespace NeteaseCloudMusic