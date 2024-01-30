#pragma once
#include "PlaylistEntity.h"
#include "ProfileInfoEntity.h"
#include "SongInfoEntity.h"
#include <QList>
namespace NeteaseCloudMusic {
struct SearchResultEntity {
    int64_t songCount;
    int64_t playlistCount;
    int64_t profileCount;
    QList<SongInfoEntity> songs;
    QList<PlaylistEntity> playlists;
    QList<ProfileInfoEntity> profiles;
};
}; // namespace NeteaseCloudMusic