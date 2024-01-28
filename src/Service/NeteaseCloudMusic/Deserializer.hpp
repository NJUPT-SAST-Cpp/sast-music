#pragma once
#include "Response/AccoutInfoEntity.h"
#include "Response/AlbumInfoEntity.h"
#include "Response/ArtistInfoEntity.h"
#include "Response/DailySongsEntity.h"
#include "Response/LoginQRCodeEntity.h"
#include "Response/LoginQRCodePollingEntity.h"
#include "Response/LoginStatusEntity.h"
#include "Response/ManyPlaylistEntity.h"
#include "Response/ManySongInfoEntity.h"
#include "Response/ManySongUrlInfoEntity.h"
#include "Response/PlaylistDetailEntity.h"
#include "Response/PlaylistEntity.h"
#include "Response/ProfileInfoEntity.h"
#include "Response/SongInfoEntity.h"
#include "Response/SongUrlInfoEntity.h"
#include "Response/SubscriptionCountEntity.h"
#include "Response/TrackIdEntity.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <Utility/Result.hpp>
namespace NeteaseCloudMusic {

template <typename T>
struct Deserializer {};

template <typename T>
struct Deserializer<QList<T>> {
    static Result<QList<T>> from(const QJsonValue& value) {
        if (!value.isArray()) {
            return ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type"};
        }
        auto arr = value.toArray();
        QList<T> list;
        list.reserve(arr.size());
        for (const auto& v : arr) {
            auto t = Deserializer<T>::from(v);
            if (t.isOk()) {
                list.push_back(std::move(t).unwrap());
            } else {
                return std::move(t).unwrapErr();
            }
        }
        return list;
    }
};

template <>
struct Deserializer<QStringList> {
    static Result<QStringList> from(const QJsonValue& value) {
        if (!value.isArray()) {
            return ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type"};
        }
        auto arr = value.toArray();
        QStringList list;
        list.reserve(arr.size());
        for (const auto& v : arr) {
            if (!v.isString()) {
                return ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type: element is not string"};
            }
            list.push_back(v.toString());
        }
        return list;
    }
};

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
        QDateTime createTime;
        if (!obj.contains("createTime") || obj["createTime"].isNull()) {
            // do nothing
        } else if (obj["createTime"].isDouble()) {
            createTime = QDateTime::fromMSecsSinceEpoch(obj["createTime"].toInteger());
        } else {
            return ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type: createTime is not number"};
        }
        return ProfileInfoEntity{
            static_cast<UserId>(obj["userId"].toInteger()),
            obj["nickname"].toString(),
            obj["avatarUrl"].toString(),
            obj["backgroundUrl"].toString(),
            obj["defaultAvatar"].toBool(),
            QDateTime::fromMSecsSinceEpoch(obj["birthday"].toInteger()),
            std::move(createTime),
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
                account = std::move(t).unwrap();
            } else {
                return std::move(t).unwrapErr();
            }
        } else {
            return ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type: account is not object"};
        }
        if (!obj.contains("profile") || obj["profile"].isNull()) {
            profile = std::nullopt;
        } else if (obj["profile"].isObject()) {
            auto t = Deserializer<ProfileInfoEntity>::from(obj["profile"]);
            if (t.isOk()) {
                profile = std::move(t).unwrap();
            } else {
                return std::move(t).unwrapErr();
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

template <>
struct Deserializer<SubscriptionCountEntity> {
    static Result<SubscriptionCountEntity> from(const QJsonValue& value) {
        if (!value.isObject()) {
            return ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type"};
        }
        auto obj = value.toObject();
        if (!obj.contains("programCount") || !obj["programCount"].isDouble()) {
            return ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type: programCount is not number"};
        }
        if (!obj.contains("djRadioCount") || !obj["djRadioCount"].isDouble()) {
            return ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type: djRadioCount is not number"};
        }
        if (!obj.contains("artistCount") || !obj["artistCount"].isDouble()) {
            return ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type: artistCount is not number"};
        }
        if (!obj.contains("newProgramCount") || !obj["newProgramCount"].isDouble()) {
            return ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type: newProgramCount is not number"};
        }
        if (!obj.contains("createDjRadioCount") || !obj["createDjRadioCount"].isDouble()) {
            return ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type: createDjRadioCount is not number"};
        }
        if (!obj.contains("createdPlaylistCount") || !obj["createdPlaylistCount"].isDouble()) {
            return ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type: createdPlaylistCount is not number"};
        }
        if (!obj.contains("subPlaylistCount") || !obj["subPlaylistCount"].isDouble()) {
            return ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type: subPlaylistCount is not number"};
        }
        return SubscriptionCountEntity{
            obj["programCount"].toInt(),     obj["djRadioCount"].toInt(),       obj["artistCount"].toInt(),
            obj["newProgramCount"].toInt(),  obj["createDjRadioCount"].toInt(), obj["createdPlaylistCount"].toInt(),
            obj["subPlaylistCount"].toInt(),
        };
    }
};

template <>
struct Deserializer<PlaylistEntity> {
    static Result<PlaylistEntity> from(const QJsonValue& value) {
        // only a few fields are deserialized
        if (!value.isObject()) {
            return ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type"};
        }
        auto obj = value.toObject();
        if (!obj.contains("subscribed") || !obj["subscribed"].isBool()) {
            return ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type: subscribed is not bool"};
        }
        if (!obj.contains("creator") || !obj["creator"].isObject()) {
            return ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type: creator is not object"};
        }
        if (!obj.contains("subscribedCount") || !obj["subscribedCount"].isDouble()) {
            return ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type: subscribedCount is not number"};
        }
        if (!obj.contains("cloudTrackCount") || !obj["cloudTrackCount"].isDouble()) {
            return ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type: cloudTrackCount is not number"};
        }
        if (!obj.contains("trackUpdateTime") || !obj["trackUpdateTime"].isDouble()) {
            return ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type: trackUpdateTime is not number"};
        }
        if (!obj.contains("trackCount") || !obj["trackCount"].isDouble()) {
            return ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type: trackCount is not number"};
        }
        if (!obj.contains("updateTime") || !obj["updateTime"].isDouble()) {
            return ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type: updateTime is not number"};
        }
        if (!obj.contains("coverImgUrl") || !obj["coverImgUrl"].isString()) {
            return ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type: coverImgUrl is not string"};
        }
        if (!obj.contains("createTime") || !obj["createTime"].isDouble()) {
            return ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type: createTime is not number"};
        }
        if (!obj.contains("trackNumberUpdateTime") || !obj["trackNumberUpdateTime"].isDouble()) {
            return ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type: trackNumberUpdateTime is not number"};
        }
        if (!obj.contains("playCount") || !obj["playCount"].isDouble()) {
            return ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type: playCount is not number"};
        }
        QString description;
        if (!obj.contains("description") || obj["description"].isNull()) {
            description = "";
        } else if (obj["description"].isString()) {
            description = obj["description"].toString();
        } else {
            return ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type: description is not string"};
        }
        if (!obj.contains("userId") || !obj["userId"].isDouble()) {
            return ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type: userId is not number"};
        }
        if (!obj.contains("name") || !obj["name"].isString()) {
            return ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type: name is not string"};
        }
        if (!obj.contains("id") || !obj["id"].isDouble()) {
            return ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type: id is not number"};
        }
        if (!obj.contains("tags") || !obj["tags"].isArray()) {
            return ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type: tags is not array"};
        }
        auto creator = Deserializer<ProfileInfoEntity>::from(obj["creator"]);
        if (creator.isErr()) {
            return std::move(creator).unwrapErr();
        }
        auto tags = Deserializer<QStringList>::from(obj["tags"]);
        if (tags.isErr()) {
            return std::move(tags).unwrapErr();
        }
        return PlaylistEntity{
            obj["subscribed"].toBool(),
            std::move(creator).unwrap(),
            obj["subscribedCount"].toInteger(),
            obj["cloudTrackCount"].toInteger(),
            QDateTime::fromMSecsSinceEpoch(obj["trackUpdateTime"].toInteger()),
            obj["trackCount"].toInteger(),
            QDateTime::fromMSecsSinceEpoch(obj["updateTime"].toInteger()),
            obj["coverImgUrl"].toString(),
            QDateTime::fromMSecsSinceEpoch(obj["createTime"].toInteger()),
            QDateTime::fromMSecsSinceEpoch(obj["trackNumberUpdateTime"].toInteger()),
            obj["playCount"].toInteger(),
            std::move(description),
            static_cast<UserId>(obj["userId"].toInteger()),
            obj["name"].toString(),
            static_cast<PlaylistId>(obj["id"].toInteger()),
            std::move(tags).unwrap(),
        };
    }
};

template <>
struct Deserializer<ManyPlaylistEntity> {
    static Result<ManyPlaylistEntity> from(const QJsonValue& value) {
        if (!value.isObject()) {
            return ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type"};
        }
        auto obj = value.toObject();
        if (!obj.contains("more") || !obj["more"].isBool()) {
            return ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type: more is not bool"};
        }
        if (!obj.contains("playlist") || !obj["playlist"].isArray()) {
            return ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type: playlist is not array"};
        }
        auto playlist = Deserializer<QList<PlaylistEntity>>::from(obj["playlist"]);
        if (playlist.isErr()) {
            return std::move(playlist).unwrapErr();
        }
        return ManyPlaylistEntity{
            obj["more"].toBool(),
            playlist.unwrap(),
        };
    }
};

template <>
struct Deserializer<AlbumInfoEntity> {
    static Result<AlbumInfoEntity> from(const QJsonValue& value) {
        // only a few fields are deserialized
        if (!value.isObject()) {
            return ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type"};
        }
        auto obj = value.toObject();
        if (!obj.contains("id") || !obj["id"].isDouble()) {
            return ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type: id is not number"};
        }
        QString name;
        if (!obj.contains("name") || obj["name"].isNull()) {
            name.clear();
        } else if (obj["name"].isString()) {
            name = obj["name"].toString();
        } else {
            return ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type: name is not string"};
        }
        QString picUrl;
        if (!obj.contains("picUrl") || obj["picUrl"].isNull()) {
            picUrl.clear();
        } else if (obj["picUrl"].isString()) {
            picUrl = obj["picUrl"].toString();
        } else {
            return ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type: picUrl is not string"};
        }
        return AlbumInfoEntity{
            static_cast<AlbumId>(obj["id"].toInteger()),
            std::move(name),
            std::move(picUrl),
        };
    }
};

template <>
struct Deserializer<ArtistInfoEntity> {
    static Result<ArtistInfoEntity> from(const QJsonValue& value) {
        // only a few fields are deserialized
        if (!value.isObject()) {
            return ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type"};
        }
        auto obj = value.toObject();
        if (!obj.contains("id") || !obj["id"].isDouble()) {
            return ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type: id is not number"};
        }
        QString name;
        if (!obj.contains("name") || obj["name"].isNull()) {
            name.clear();
        } else if (obj["name"].isString()) {
            name = obj["name"].toString();
        } else {
            return ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type: name is not string"};
        }
        return ArtistInfoEntity{
            static_cast<UserId>(obj["id"].toInteger()),
            std::move(name),
        };
    }
};

template <>
struct Deserializer<SongInfoEntity> {
    static Result<SongInfoEntity> from(const QJsonValue& value) {
        // only a few fields are deserialized
        if (!value.isObject()) {
            return ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type"};
        }
        auto obj = value.toObject();
        if (!obj.contains("id") || !obj["id"].isDouble()) {
            return ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type: id is not number"};
        }
        if (!obj.contains("name") || !obj["name"].isString()) {
            return ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type: name is not string"};
        }
        if (!obj.contains("alia") || !obj["alia"].isArray()) {
            return ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type: alia is not array"};
        }
        if (!obj.contains("ar") || !obj["ar"].isArray()) {
            return ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type: ar is not array"};
        }
        if (!obj.contains("al") || !obj["al"].isObject()) {
            return ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type: al is not object"};
        }
        auto aliases = Deserializer<QStringList>::from(obj["alia"]);
        if (aliases.isErr()) {
            return std::move(aliases).unwrapErr();
        }
        auto artists = Deserializer<QList<ArtistInfoEntity>>::from(obj["ar"]);
        if (artists.isErr()) {
            return std::move(artists).unwrapErr();
        }
        auto album = Deserializer<AlbumInfoEntity>::from(obj["al"]);
        if (album.isErr()) {
            return std::move(album).unwrapErr();
        }
        return SongInfoEntity{
            static_cast<SongId>(obj["id"].toInteger()),
            obj["name"].toString(),
            std::move(aliases).unwrap(),
            std::move(artists).unwrap(),
            std::move(album).unwrap(),
        };
    }
};

template <>
struct Deserializer<TrackIdEntity> {
    static Result<TrackIdEntity> from(const QJsonValue& value) {
        // only a few fields are deserialized
        if (!value.isObject()) {
            return ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type"};
        }
        auto obj = value.toObject();
        if (!obj.contains("id") || !obj["id"].isDouble()) {
            return ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type: id is not number"};
        }
        if (!obj.contains("at") || !obj["at"].isDouble()) {
            return ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type: at is not number"};
        }
        return TrackIdEntity{
            static_cast<SongId>(obj["id"].toInteger()),
            QDateTime::fromMSecsSinceEpoch(obj["at"].toInteger()),
        };
    }
};

template <>
struct Deserializer<PlaylistDetailEntity> {
    static Result<PlaylistDetailEntity> from(const QJsonValue& value) {
        // only a few fields are deserialized
        if (!value.isObject()) {
            return ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type"};
        }
        auto obj = value.toObject();

        if (!obj.contains("playlist") || !obj["playlist"].isObject()) {
            return ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type: playlist is not object"};
        }
        auto playlist = obj["playlist"].toObject();
        auto simple = Deserializer<PlaylistEntity>::from(playlist);
        if (simple.isErr()) {
            return std::move(simple).unwrapErr();
        }
        if (!playlist.contains("trackIds") || !playlist["trackIds"].isArray()) {
            return ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type: trackIds is not array"};
        }
        if (!playlist.contains("tracks") || !playlist["tracks"].isArray()) {
            return ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type: tracks is not array"};
        }
        auto trackIds = Deserializer<QList<TrackIdEntity>>::from(playlist["trackIds"]);
        if (trackIds.isErr()) {
            return std::move(trackIds).unwrapErr();
        }
        auto tracks = Deserializer<QList<SongInfoEntity>>::from(playlist["tracks"]);
        if (tracks.isErr()) {
            return std::move(tracks).unwrapErr();
        }
        return PlaylistDetailEntity{
            std::move(simple).unwrap(),
            std::move(trackIds).unwrap(),
            std::move(tracks).unwrap(),
        };
    }
};

template <>
struct Deserializer<ManySongInfoEntity> {
    static Result<ManySongInfoEntity> from(const QJsonValue& value) {
        if (!value.isObject()) {
            return ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type"};
        }
        auto obj = value.toObject();

        if (!obj.contains("songs") || !obj["songs"].isArray()) {
            return ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type: songs is not array"};
        }
        auto songs = Deserializer<QList<SongInfoEntity>>::from(obj["songs"]);
        if (songs.isErr()) {
            return std::move(songs).unwrapErr();
        }
        return ManySongInfoEntity{
            std::move(songs).unwrap(),
        };
    }
};

template <>
struct Deserializer<SongUrlInfoEntity> {
    static Result<SongUrlInfoEntity> from(const QJsonValue& value) {
        // only a few fields are deserialized
        if (!value.isObject()) {
            return ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type"};
        }
        auto obj = value.toObject();

        if (!obj.contains("id") || !obj["id"].isDouble()) {
            return ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type: id is not number"};
        }
        if (!obj.contains("url") || !obj["url"].isString()) {
            return ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type: url is not string"};
        }
        if (!obj.contains("br") || !obj["br"].isDouble()) {
            return ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type: br is not number"};
        }
        if (!obj.contains("size") || !obj["size"].isDouble()) {
            return ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type: size is not number"};
        }
        if (!obj.contains("md5") || !obj["md5"].isString()) {
            return ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type: md5 is not string"};
        }
        if (!obj.contains("type") || !obj["type"].isString()) {
            return ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type: type is not string"};
        }
        if (!obj.contains("level") || !obj["level"].isString()) {
            return ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type: level is not string"};
        }
        if (!obj.contains("payed") || !obj["payed"].isDouble()) {
            return ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type: payed is not number"};
        }
        if (!obj.contains("time") || !obj["time"].isDouble()) {
            return ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type: time is not number"};
        }
        return SongUrlInfoEntity{
            static_cast<SongId>(obj["id"].toInteger()),
            obj["url"].toString(),
            obj["br"].toInteger(),
            obj["size"].toInteger(),
            obj["md5"].toString(),
            obj["type"].toString(),
            obj["level"].toString(),
            obj["payed"].toInt() != 0,
            obj["time"].toInteger(),
        };
    }
};

template <>
struct Deserializer<ManySongUrlInfoEntity> {
    static Result<ManySongUrlInfoEntity> from(const QJsonValue& value) {
        if (!value.isObject()) {
            return ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type"};
        }
        auto obj = value.toObject();

        if (!obj.contains("data") || !obj["data"].isArray()) {
            return ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type: data is not array"};
        }
        auto data = Deserializer<QList<SongUrlInfoEntity>>::from(obj["data"]);
        if (data.isErr()) {
            return std::move(data).unwrapErr();
        }
        return ManySongUrlInfoEntity{
            std::move(data).unwrap(),
        };
    }
};

template <>
struct Deserializer<DailySongsEntity> {
    static Result<DailySongsEntity> from(const QJsonValue& value) {
        if (!value.isObject()) {
            return ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type"};
        }
        auto obj = value.toObject();

        if (!obj.contains("data") || !obj["data"].isObject()) {
            return ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type: data is not object"};
        }
        auto data = obj["data"].toObject();
        if (!data.contains("dailySongs") || !data["dailySongs"].isArray()) {
            return ErrorInfo{ErrorKind::JsonDeserializeError, "Invalid JSON type: data.dailySongs is not array"};
        }
        auto dailySongs = Deserializer<QList<SongInfoEntity>>::from(data["dailySongs"]);
        if (dailySongs.isErr()) {
            return std::move(dailySongs).unwrapErr();
        }
        return DailySongsEntity{
            std::move(dailySongs).unwrap(),
        };
    }
};

}; // namespace NeteaseCloudMusic