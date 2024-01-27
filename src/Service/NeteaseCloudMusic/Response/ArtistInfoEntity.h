#pragma once
#include "BasicDef.h"
#include <QDateTime>
#include <QString>
namespace NeteaseCloudMusic {
struct ArtistInfoEntity {
    // 未关联歌手为 0
    UserId id;
    QString name;
};
}; // namespace NeteaseCloudMusic