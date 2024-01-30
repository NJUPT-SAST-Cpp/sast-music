#pragma once
#include "BasicDef.h"
#include <QDateTime>
#include <QString>
namespace NeteaseCloudMusic {
struct ProfileInfoEntity {
    UserId userId = 0;
    QString nickname;
    QString avatarUrl;
    QString backgroundUrl;
    bool defaultAvatar;
    QDateTime birthday;
    QDateTime createTime;
};
}; // namespace NeteaseCloudMusic