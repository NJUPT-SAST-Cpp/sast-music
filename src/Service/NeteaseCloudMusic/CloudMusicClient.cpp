#include "CloudMusicClient.h"
#include "Deserializer.hpp"
#include "Encryption/Encryption.hpp"
#include "Encryption/WeApi.hpp"
#include <QCryptographicHash>
#include <QNetworkProxy>
#include <Utility/CookieUtils.h>
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
    if (containsCookie(cookies, "MUSIC_U")) {
        return callback(Result<void>());
    }
    if (containsCookie(cookies, "MUSIC_A")) {
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

void NeteaseCloudMusic::CloudMusicClient::getUserSubscriptionCount(
    std::function<void(Result<SubscriptionCountEntity>)> callback) {
    auto url = QUrl("https://music.163.com/weapi/subcount");
    auto data = QJsonDocument(QJsonObject{});
    request<WeApi>("POST", url, data, [callback = std::move(callback)](Result<QJsonObject> result) {
        callback(result.andThen(Deserializer<SubscriptionCountEntity>::from));
    });
}

void NeteaseCloudMusic::CloudMusicClient::getUserPlaylist(UserId uid, int limit, int offset, bool includeVideo,
                                                          std::function<void(Result<ManyPlaylistEntity>)> callback) {
    auto url = QUrl("https://music.163.com/weapi/user/playlist");
    auto data = QJsonDocument(QJsonObject{
        {"uid", static_cast<qint64>(uid)},
        {"limit", limit},
        {"offset", offset},
        {"includeVideo", includeVideo},
    });
    request<WeApi>("POST", url, data, [callback = std::move(callback)](Result<QJsonObject> result) {
        callback(result.andThen(Deserializer<ManyPlaylistEntity>::from));
    });
}

void NeteaseCloudMusic::CloudMusicClient::getPlaylistDetail(
    PlaylistId id, std::function<void(Result<PlaylistDetailEntity>)> callback) {
    auto url = QUrl("https://music.163.com/weapi/v6/playlist/detail");
    auto data = QJsonDocument(QJsonObject{
        {"id", static_cast<qint64>(id)},
        {"n", 100000},
        {"s", 8},
    });
    request<WeApi>("POST", url, data, [callback = std::move(callback)](Result<QJsonObject> result) {
        callback(result.andThen(Deserializer<PlaylistDetailEntity>::from));
    });
}

void NeteaseCloudMusic::CloudMusicClient::getSongsDetail(const QList<SongId>& songIds,
                                                         std::function<void(Result<ManySongInfoEntity>)> callback) {

    auto url = QUrl("https://music.163.com/weapi/v3/song/detail");
    QJsonArray c;
    for (auto& songId : songIds) {
        c.append(QJsonObject{
            {"id", static_cast<qint64>(songId)},
        });
    }
    auto data = QJsonDocument(QJsonObject{
        {"c", QString::fromUtf8(QJsonDocument{c}.toJson(QJsonDocument::Compact))},
    });
    request<WeApi>("POST", url, data, [callback = std::move(callback)](Result<QJsonObject> result) {
        callback(result.andThen(Deserializer<ManySongInfoEntity>::from));
    });
}
