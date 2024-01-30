#pragma once
#include "PlaylistEntity.h"
#include <QList>
namespace NeteaseCloudMusic {
struct ManyPlaylistEntity {
    bool more;
    QList<PlaylistEntity> playlist;
};
}; // namespace NeteaseCloudMusic