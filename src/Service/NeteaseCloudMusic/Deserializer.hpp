#pragma once
#include "Response/AccoutInfoEntity.h"
#include "Response/LoginQRCodeEntity.h"
#include "Response/LoginQRCodePollingEntity.h"
#include "Response/LoginStatusEntity.h"
#include "Response/ProfileInfoEntity.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <Utility/Result.hpp>
namespace NeteaseCloudMusic {

template <typename T>
struct Deserializer {};

template <>
struct Deserializer<LoginQRCodeEntity> {
    static Result<LoginQRCodeEntity> from(const QJsonValue& value) {
        if (!value.isObject()) {
            return ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type"};
        }
        auto obj = value.toObject();
        if (!obj.contains("unikey") || !obj["unikey"].isString()) {
            return ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type: unikey is not string"};
        }
        auto key = obj["unikey"].toString();
        auto qrCodeData = QString("https://music.163.com/login?codekey=" + key);
        return LoginQRCodeEntity{
            std::move(key),
            std::move(qrCodeData),
        };
    }
};

template <>
struct Deserializer<LoginQRCodePollingEntity> {
    static Result<LoginQRCodePollingEntity> from(const QJsonValue& value) {
        if (!value.isObject()) {
            return ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type"};
        }
        auto obj = value.toObject();
        if (!obj.contains("code") || !obj["code"].isDouble()) {
            return ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type: code is not number"};
        }
        auto code = obj["code"].toInt();
        LoginQRCodePollingStatus status;
        switch (code) {
        case 800:
            status = LoginQRCodePollingStatus::Timeout;
            break;
        case 801:
            status = LoginQRCodePollingStatus::WaitingForScan;
            break;
        case 802:
            status = LoginQRCodePollingStatus::WaitingForConfirm;
            break;
        case 803:
            status = LoginQRCodePollingStatus::Success;
            break;
        default:
            status = LoginQRCodePollingStatus::Unknown;
            break;
        }
        return LoginQRCodePollingEntity{
            status,
        };
    }
};

template <>
struct Deserializer<AccoutInfoEntity> {
    static Result<AccoutInfoEntity> from(const QJsonValue& value) {
        // only a few fields are deserialized
        if (!value.isObject()) {
            return ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type"};
        }
        auto obj = value.toObject();
        if (!obj.contains("anonimousUser") || !obj["anonimousUser"].isBool()) {
            return ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type: anonimousUser is not bool"};
        }
        if (!obj.contains("id") || !obj["id"].isDouble()) {
            return ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type: id is not number"};
        }
        return AccoutInfoEntity{
            obj["anonimousUser"].toBool(),
            static_cast<UserId>(obj["id"].toInteger()),
        };
    }
};

template <>
struct Deserializer<ProfileInfoEntity> {
    static Result<ProfileInfoEntity> from(const QJsonValue& value) {
        // only a few fields are deserialized
        if (!value.isObject()) {
            return ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type"};
        }
        auto obj = value.toObject();
        if (!obj.contains("userId") || !obj["userId"].isDouble()) {
            return ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type: userId is not number"};
        }
        if (!obj.contains("nickname") || !obj["nickname"].isString()) {
            return ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type: nickname is not string"};
        }
        if (!obj.contains("avatarUrl") || !obj["avatarUrl"].isString()) {
            return ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type: avatarUrl is not string"};
        }
        if (!obj.contains("backgroundUrl") || !obj["backgroundUrl"].isString()) {
            return ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type: backgroundUrl is not string"};
        }
        if (!obj.contains("defaultAvatar") || !obj["defaultAvatar"].isBool()) {
            return ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type: defaultAvatar is not bool"};
        }
        if (!obj.contains("birthday") || !obj["birthday"].isDouble()) {
            return ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type: birthday is not number"};
        }
        if (!obj.contains("createTime") || !obj["createTime"].isDouble()) {
            return ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type: createTime is not number"};
        }
        return ProfileInfoEntity{
            static_cast<UserId>(obj["userId"].toInteger()),
            obj["nickname"].toString(),
            obj["avatarUrl"].toString(),
            obj["backgroundUrl"].toString(),
            obj["defaultAvatar"].toBool(),
            QDateTime::fromMSecsSinceEpoch(obj["birthday"].toInteger()),
            QDateTime::fromMSecsSinceEpoch(obj["createTime"].toInteger()),
        };
    }
};

template <>
struct Deserializer<LoginStatusEntity> {
    static Result<LoginStatusEntity> from(const QJsonValue& value) {
        if (!value.isObject()) {
            return ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type"};
        }
        auto obj = value.toObject();
        std::optional<AccoutInfoEntity> account;
        std::optional<ProfileInfoEntity> profile;
        if (!obj.contains("account") || obj["account"].isNull()) {
            account = std::nullopt;
        } else if (obj["account"].isObject()) {
            auto t = Deserializer<AccoutInfoEntity>::from(obj["account"]);
            if (t.isOk()) {
                account = t.take();
            } else {
                return t.takeErr();
            }
        } else {
            return ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type: account is not object"};
        }
        if (!obj.contains("profile") || obj["profile"].isNull()) {
            profile = std::nullopt;
        } else if (obj["profile"].isObject()) {
            auto t = Deserializer<ProfileInfoEntity>::from(obj["profile"]);
            if (t.isOk()) {
                profile = t.take();
            } else {
                return t.takeErr();
            }
        } else {
            return ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type: profile is not object"};
        }
        return LoginStatusEntity{
            std::move(account),
            std::move(profile),
        };
    }
};

}; // namespace NeteaseCloudMusic