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
    QNetworkReply* createReply(const QByteArray& verb, const QNetworkRequest& requestInfo, const QByteArray& body);

public:
    static CloudMusicClient* getInstance();
    CloudMusicClient();
    ~CloudMusicClient();
    template <typename TEncryption, typename TCallback>
    void request(const QByteArray& verb, const QUrl& url, const QJsonDocument& data, TCallback callback) {
        auto requestResult = TEncryption::makeRequest(verb, url, manager.cookieJar(), data);
        if (requestResult.isErr()) {
            callback(Result<QJsonObject>(requestResult.takeErr()));
            return;
        }
        auto [requestInfo, body] = requestResult.unwrap();
        QNetworkReply* reply;
        if (manager.thread() == QThread::currentThread()) {
            reply = createReply(verb, requestInfo, body);
        } else {
            QMetaObject::invokeMethod(
                &manager,
                [this, &verb, requestInfo = std::move(requestInfo), body = std::move(body)] {
                    return createReply(verb, requestInfo, body);
                },
                Qt::BlockingQueuedConnection, &reply);
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

    // Note: CloudMusicClient takes ownership of the cookieJar object.
    void setCookieJar(QNetworkCookieJar* cookieJar);
    void newLoginQRCode(std::function<void(Result<LoginQRCodeEntity>)> callback);
    void loginQRCodePolling(QStringView key, std::function<void(Result<LoginQRCodePollingEntity>)> callback);
    void checkAnonimousToken(std::function<void(Result<void>)> callback);
};
} // namespace NeteaseCloudMusic