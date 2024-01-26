#pragma once
#include "Encryption.hpp"
namespace NeteaseCloudMusic {
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
        throw std::runtime_error("Not implemented");
    }
    static Result<QByteArray> decryptResponse(QByteArray&& content) {
        throw std::runtime_error("Not implemented");
    }
};
} // namespace NeteaseCloudMusic