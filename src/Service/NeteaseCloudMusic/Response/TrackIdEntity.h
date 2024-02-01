#pragma once
#include "BasicDef.h"
#include <QDateTime>
namespace NeteaseCloudMusic {
struct TrackIdEntity {
    SongId id;
    QDateTime at;
};
}; // namespace NeteaseCloudMusic