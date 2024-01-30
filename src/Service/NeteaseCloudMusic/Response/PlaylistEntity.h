#pragma once
#include "ProfileInfoEntity.h"
namespace NeteaseCloudMusic {
struct PlaylistEntity {
    bool subscribed;
    ProfileInfoEntity creator;
    int64_t subscribedCount;
    int64_t cloudTrackCount;
    QDateTime trackUpdateTime;
    int64_t trackCount;
    QDateTime updateTime;
    QString coverImgUrl;
    QDateTime createTime;
    QDateTime trackNumberUpdateTime;
    int64_t playCount;
    QString description;
    UserId userId;
    QString name;
    PlaylistId id;
    QStringList tags;
};
}; // namespace NeteaseCloudMusic