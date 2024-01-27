#include "CloudMusicClient.h"
#include "Deserializer.hpp"
#include "Encryption/Encryption.hpp"
#include "Encryption/WeApi.hpp"
#include <QCryptographicHash>
#include <QNetworkProxy>
NeteaseCloudMusic::CloudMusicClient* NeteaseCloudMusic::CloudMusicClient::getInstance() {
    static CloudMusicClient instance;
    return &instance;
}

NeteaseCloudMusic::CloudMusicClient::CloudMusicClient() {
    // FIXME: proxy for debug
    QNetworkProxy proxy;
    proxy.setType(QNetworkProxy::HttpProxy);
    proxy.setHostName("127.0.0.1");
    proxy.setPort(8888);
    manager.setProxy(proxy);
}

NeteaseCloudMusic::CloudMusicClient::~CloudMusicClient() {}

QNetworkReply* NeteaseCloudMusic::CloudMusicClient::createReply(const QByteArray& verb,
                                                                const QNetworkRequest& requestInfo,
                                                                const QByteArray& body) {
    if (verb.compare("POST", Qt::CaseInsensitive) == 0) {
        return manager.post(requestInfo, body);
    } else if (verb.compare("PUT", Qt::CaseInsensitive) == 0) {
        return manager.put(requestInfo, body);
    } else if (verb.compare("DELETE", Qt::CaseInsensitive) == 0) {
        assert(body.isEmpty());
        return manager.deleteResource(requestInfo);
    } else if (verb.compare("GET", Qt::CaseInsensitive) == 0) {
        assert(body.isEmpty());
        return manager.get(requestInfo);
    } else {
        return manager.sendCustomRequest(requestInfo, verb, body);
    }
}

void NeteaseCloudMusic::CloudMusicClient::setCookieJar(QNetworkCookieJar* cookieJar) {
    manager.setCookieJar(cookieJar);
}

void NeteaseCloudMusic::CloudMusicClient::newLoginQRCode(std::function<void(Result<LoginQRCodeEntity>)> callback) {
    auto url = QUrl("https://music.163.com/weapi/login/qrcode/unikey");
    auto data = QJsonDocument(QJsonObject{
        {"type", 1},
    });
    request<WeApi>("POST", url, data, [callback = std::move(callback)](Result<QJsonObject> result) {
        callback(result.andThen(Deserializer<LoginQRCodeEntity>::from));
    });
}

void NeteaseCloudMusic::CloudMusicClient::loginQRCodePolling(
    QStringView key, std::function<void(Result<LoginQRCodePollingEntity>)> callback) {
    auto url = QUrl("https://music.163.com/weapi/login/qrcode/client/login");
    auto data = QJsonDocument(QJsonObject{
        {"key", key.toString()},
        {"type", 1},
    });
    request<WeApi>("POST", url, data, [callback = std::move(callback)](Result<QJsonObject> result) {
        callback(result.andThen(Deserializer<LoginQRCodePollingEntity>::from));
    });
}

namespace NeteaseCloudMusic {
static QString encodeDeviceId(QByteArray deviceId) {
    QByteArray key = "3go8&$833h0k(2)2";
    QByteArray encodedId;
    for (size_t i = 0; i < deviceId.size(); i++) {
        encodedId += deviceId[i] ^ key[i % key.size()];
    }
    // calculate md5
    QCryptographicHash hash(QCryptographicHash::Md5);
    hash.addData(encodedId);
    encodedId = hash.result().toBase64();

    return QString((deviceId + " " + encodedId).toBase64());
}
} // namespace NeteaseCloudMusic

void NeteaseCloudMusic::CloudMusicClient::checkAnonimousToken(std::function<void(Result<void>)> callback) {
    auto url = QUrl("https://music.163.com/weapi/register/anonimous");
    auto cookies = manager.cookieJar()->cookiesForUrl(url);
    auto musicUCookie = std::find_if(cookies.begin(), cookies.end(),
                                     [](const QNetworkCookie& cookie) { return cookie.name() == "MUSIC_U"; });
    if (musicUCookie != cookies.end()) {
        return callback(Result<void>());
    }
    auto musicACookie = std::find_if(cookies.begin(), cookies.end(),
                                     [](const QNetworkCookie& cookie) { return cookie.name() == "MUSIC_A"; });
    if (musicACookie != cookies.end()) {
        return callback(Result<void>());
    }
    QByteArray deviceId = "NMUSIC";
    auto data = QJsonDocument(QJsonObject{
        {"username", encodeDeviceId(deviceId)},
    });
    request<WeApi>("POST", url, data, [callback = std::move(callback)](Result<QJsonObject> result) {
        callback(result.map([](QJsonObject) {}));
    });
}

void NeteaseCloudMusic::CloudMusicClient::getLoginStatus(std::function<void(Result<LoginStatusEntity>)> callback) {
    auto url = QUrl("https://music.163.com/weapi/w/nuser/account/get");
    auto data = QJsonDocument(QJsonObject{});
    request<WeApi>("POST", url, data, [callback = std::move(callback)](Result<QJsonObject> result) {
        callback(result.andThen(Deserializer<LoginStatusEntity>::from));
    });
}
