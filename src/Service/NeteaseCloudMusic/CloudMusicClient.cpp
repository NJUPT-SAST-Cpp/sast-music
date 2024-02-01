#include "CloudMusicClient.h"
#include "Deserializer.hpp"
#include "Encryption/EApi.hpp"
#include "Encryption/WeApi.hpp"
#include <QCryptographicHash>
#include <QNetworkProxy>
#include <Utility/CookieUtils.h>
#include <Utility/SettingsUtils.h>

NeteaseCloudMusic::CloudMusicClient* NeteaseCloudMusic::CloudMusicClient::getInstance() {
    static CloudMusicClient instance;
    return &instance;
}

NeteaseCloudMusic::CloudMusicClient::CloudMusicClient() {
    // FIXME: proxy for debug
    // QNetworkProxy proxy;
    // proxy.setType(QNetworkProxy::HttpProxy);
    // proxy.setHostName("127.0.0.1");
    // proxy.setPort(8888);
    // manager.setProxy(proxy);
    manager.setProxy(QNetworkProxy::NoProxy);
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

void NeteaseCloudMusic::CloudMusicClient::getSongsUrl(const QList<SongId>& songIds, QStringView level,
                                                      std::function<void(Result<ManySongUrlInfoEntity>)> callback) {
    auto url = QUrl("https://interface.music.163.com/eapi/song/enhance/player/url/v1");
    QJsonArray ids;
    for (auto& songId : songIds) {
        ids.append(static_cast<qint64>(songId));
    }
    auto data = QJsonDocument(QJsonObject{
        {"ids", QString::fromUtf8(QJsonDocument{ids}.toJson(QJsonDocument::Compact))},
        {"level", level.toString()},
        {"encodeType", "aac"},
    });
    request<EApi>("POST", url, data, [callback = std::move(callback)](Result<QJsonObject> result) {
        callback(result.andThen(Deserializer<ManySongUrlInfoEntity>::from));
    });
}

void NeteaseCloudMusic::CloudMusicClient::getDailySongs(std::function<void(Result<DailySongsEntity>)> callback) {
    auto url = QUrl("https://music.163.com/eapi/v3/discovery/recommend/songs");
    auto data = QJsonDocument(QJsonObject{});
    request<EApi>("POST", url, data, [callback = std::move(callback)](Result<QJsonObject> result) {
        callback(result.andThen(Deserializer<DailySongsEntity>::from));
    });
}

void NeteaseCloudMusic::CloudMusicClient::getSongLyric(SongId id,
                                                       std::function<void(Result<SongLyricEntity>)> callback) {
    auto url = QUrl("https://interface3.music.163.com/eapi/song/lyric/v1");
    auto data = QJsonDocument(QJsonObject{
        {"id", static_cast<qint64>(id)},
        {"cp", false},
        {"tv", 0},
        {"lv", 0},
        {"rv", 0},
        {"kv", 0},
        {"yv", 0},
        {"ytv", 0},
        {"yrv", 0},
    });
    request<EApi>("POST", url, data, [callback = std::move(callback)](Result<QJsonObject> result) {
        callback(result.andThen(Deserializer<SongLyricEntity>::from));
    });
}

void NeteaseCloudMusic::CloudMusicClient::cloudsearch(QStringView keywords, SearchType type, int limit, int offset,
                                                      std::function<void(Result<SearchResultEntity>)> callback) {
    auto url = QUrl("https://interface.music.163.com/eapi/cloudsearch/pc");
    auto data = QJsonDocument(QJsonObject{
        {"s", keywords.toString()},
        {"type", static_cast<int>(type)},
        {"limit", limit},
        {"offset", offset},
    });
    request<EApi>("POST", url, data, [callback = std::move(callback)](Result<QJsonObject> result) {
        callback(result.andThen(Deserializer<SearchResultEntity>::from));
    });
}

void NeteaseCloudMusic::CloudMusicClient::logout(std::function<void(Result<QJsonObject>)> callback) {
    auto url = QUrl("https://interface.music.163.com/weapi/logout");
    auto data = QJsonDocument(QJsonObject{});
    request<WeApi>("POST", url, data,
                   [callback = std::move(callback)](Result<QJsonObject> result) { callback(result.unwrap()); });
}
