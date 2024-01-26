#pragma once
#include "Encryption.hpp"
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QNetworkCookie>
#include <QNetworkCookieJar>
#include <QNetworkRequest>
#include <QUrl>
#include <QUrlQuery>
#include <Utility/EncryptionUtils.h>
#include <Utility/RandomUtils.h>
namespace NeteaseCloudMusic {
namespace WeApiDetails {
const QByteArray presetKey = "0CoJUm6Qyw8W8jud";
const QByteArray defaultIv = "0102030405060708";
const QString publicKey =
    "-----BEGIN PUBLIC KEY-----\n"
    "MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQDgtQn2JZ34ZC28NWYpAUd98iZ37BUrX/aKzmFbt7clFSs6sXqHauqKWqdtLkF2KexO40H1YTX8z2lSgBBOAxLsvaklV8k4cBFK9snQXE9/DDaFt6Rr7iVZMldczhC0JNgTz+SHXT6CBHuX3e9SdB1Ua44oncaTWz7OBGLbCiK45wIDAQAB\n"
    "-----END PUBLIC KEY-----\n";
} // namespace WeApiDetails
struct WeApi {
public:
    WeApi() = delete;
    ~WeApi() = delete;
    static Result<std::tuple<QNetworkRequest, QByteArray>>
        makeRequest(const QByteArray& verb, const QUrl& url, QNetworkCookieJar* cookieJar, const QJsonDocument& data) {
        auto requestResult = EncryptionBase::prepareRequest(verb, url, cookieJar, data);
        if (requestResult.isErr()) {
            return requestResult.takeErr();
        }
        auto request = requestResult.take();
        request.setRawHeader(
            "User-Agent",
            "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/116.0.0.0 Safari/537.36 Edg/116.0.1938.69");

        auto cookies = cookieJar->cookiesForUrl(url);
        auto csrfCookie = std::find_if(cookies.begin(), cookies.end(),
                                       [](const QNetworkCookie& cookie) { return cookie.name() == "__csrf"; });
        QByteArray csrf;
        if (csrfCookie != cookies.end()) {
            csrf = csrfCookie->value();
        }
        auto dataObject = data.object();
        dataObject.insert("csrf_token", QJsonValue(QString::fromUtf8(csrf)));

        auto dataBytes = QJsonDocument{dataObject}.toJson(QJsonDocument::Compact);

        auto secretKey = generateRandomBase62String(16).toUtf8();

        auto enc1 = aesCbcEncrypt(dataBytes, WeApiDetails::presetKey, WeApiDetails::defaultIv);
        if (enc1.isErr()) {
            return enc1.takeErr();
        }
        auto enc2 = aesCbcEncrypt(enc1.unwrap().toBase64(), secretKey, WeApiDetails::defaultIv);
        if (enc2.isErr()) {
            return enc2.takeErr();
        }

        auto params = enc2.unwrap().toBase64();
        std::reverse(secretKey.begin(), secretKey.end());
        auto encSecKeyResult = encryptWithPublicKey(WeApiDetails::publicKey, secretKey);
        if (encSecKeyResult.isErr()) {
            return encSecKeyResult.takeErr();
        }
        auto encSecKey = encSecKeyResult.unwrap().toHex();
        if (verb.compare("POST", Qt::CaseInsensitive) == 0) {
            QUrlQuery query;
            request.setUrl(url);
            request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
            query.addQueryItem("params", params);
            query.addQueryItem("encSecKey", encSecKey);
            return std::make_tuple(request, query.toString(QUrl::FullyEncoded).toUtf8());
        } else {
            QUrlQuery query(url);
            query.addQueryItem("params", params);
            query.addQueryItem("encSecKey", encSecKey);
            request.setUrl(url.toString(QUrl::RemoveQuery) + "?" + query.toString(QUrl::FullyEncoded));
            return std::make_tuple(request, QByteArray());
        }
    }

    static Result<QByteArray> decryptResponse(QByteArray&& data) {
        return data;
    }
};
} // namespace NeteaseCloudMusic