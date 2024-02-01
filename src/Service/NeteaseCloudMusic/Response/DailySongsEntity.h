#pragma once
#include "SongInfoEntity.h"
#include <QList>
namespace NeteaseCloudMusic {
struct DailySongsEntity {
    QList<SongInfoEntity> dailySongs;
};
}; // namespace NeteaseCloudMusic