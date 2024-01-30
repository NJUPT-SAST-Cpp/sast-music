#pragma once
#include "PlaylistEntity.h"
#include "SongInfoEntity.h"
#include "TrackIdEntity.h"
#include <QList>
namespace NeteaseCloudMusic {
struct PlaylistDetailEntity {
    PlaylistEntity simple;
    // 包含所有，但只有 id，如果需要其他信息，请通过 getSongDetail 获取
    QList<TrackIdEntity> trackIds;
    // 仅包含前 1000 条
    QList<SongInfoEntity> tracks;
};
}; // namespace NeteaseCloudMusic