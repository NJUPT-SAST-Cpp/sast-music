#pragma once
#include "SongInfoEntity.h"
#include <QList>
namespace NeteaseCloudMusic {
struct ManySongInfoEntity {
    QList<SongInfoEntity> songs;
};
}; // namespace NeteaseCloudMusic