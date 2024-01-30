#pragma once
#include "AlbumInfoEntity.h"
#include "ArtistInfoEntity.h"
#include "BasicDef.h"
#include <QDateTime>
#include <QString>
namespace NeteaseCloudMusic {
struct SongInfoEntity {
    SongId id;
    QString name;
    QStringList aliases;
    QList<ArtistInfoEntity> artists;
    AlbumInfoEntity album;
};
}; // namespace NeteaseCloudMusic