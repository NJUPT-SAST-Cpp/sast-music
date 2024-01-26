#pragma once
#include "Response/LoginQRCodeEntity.h"
#include "Response/LoginQRCodePollingEntity.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <QStringView>
#include <QThread>
#include <QUrl>
#include <Utility/Result.hpp>
#include <functional>
namespace NeteaseCloudMusic {
namespace CloudMusicClientDetails {
const int SUCCESS_CODES[] = {200, 201, 302, 400, 502, 800, 801, 802, 803};
}
class CloudMusicClient : public QObject {
    Q_OBJECT
private:
    QNetworkAccessManager manager;

public:
    CloudMusicClient();
    ~CloudMusicClient();
    template <typename TEncryption, typename TCallback>
    void request(const QByteArray& verb, const QUrl& url, const QJsonDocument& data, TCallback callback) {
        auto requestResult = TEncryption::makeRequest(verb, url, manager.cookieJar(), data);
        if (requestResult.isErr()) {
            callback(Result<QJsonObject>(requestResult.takeErr()));
            return;
        }
        auto [request, body] = requestResult.unwrap();
        QNetworkReply* reply;
        auto createReply = [&]() -> QNetworkReply* {
            if (verb.compare("POST", Qt::CaseInsensitive) == 0) {
                return manager.post(request, body);
            } else if (verb.compare("PUT", Qt::CaseInsensitive) == 0) {
                return manager.put(request, body);
            } else if (verb.compare("DELETE", Qt::CaseInsensitive) == 0) {
                assert(body.isEmpty());
                return manager.deleteResource(request);
            } else if (verb.compare("GET", Qt::CaseInsensitive) == 0) {
                assert(body.isEmpty());
                return manager.get(request);
            } else {
                return manager.sendCustomRequest(request, verb, body);
            }
        };
        if (manager.thread() == QThread::currentThread()) {
            reply = createReply();
        } else {
            QMetaObject::invokeMethod(&manager, createReply, Qt::BlockingQueuedConnection, &reply);
        }
        connect(reply, &QNetworkReply::finished, [=] {
            auto content = reply->readAll();
            auto networkError = reply->error();

            if (networkError != QNetworkReply::NoError) {
                callback(Result<QJsonObject>(ErrorInfo{ErrorKind::NetworkError, reply->errorString()}));
            }
            Result<QByteArray> decryptedResult = TEncryption::decryptResponse(std::move(content));
            if (decryptedResult.isErr()) {
                callback(Result<QJsonObject>(decryptedResult.takeErr()));
                return;
            }
            auto jsonResult = QJsonDocument::fromJson(decryptedResult.unwrap());
            if (jsonResult.isNull()) {
                callback(Result<QJsonObject>(ErrorInfo{ErrorKind::JsonParseError, "Failed to parse JSON"}));
                return;
            }
            auto json = jsonResult.object();
            if (json.contains("code")) {
                int code = json["code"].toInt();
                auto found =
                    std::find_if(std::begin(CloudMusicClientDetails::SUCCESS_CODES),
                                 std::end(CloudMusicClientDetails::SUCCESS_CODES), [code](int x) { return x == code; });
                if (found == std::end(CloudMusicClientDetails::SUCCESS_CODES)) {
                    QString message;
                    if (json.contains("message")) {
                        message = json["message"].toString();
                    } else {
                        message = QStringLiteral("API error code = %1").arg(code);
                    }
                    callback(Result<QJsonObject>(ErrorInfo{ErrorKind::ApiError, std::move(message)}));
                    return;
                }
            }
            callback(Result<QJsonObject>(json));
            reply->deleteLater();
        });
    }

    void newLoginQRCode(std::function<void(Result<LoginQRCodeEntity>)> callback);
    void loginQRCodePolling(QStringView key, std::function<void(Result<LoginQRCodePollingEntity>)> callback);
};
} // namespace NeteaseCloudMusic