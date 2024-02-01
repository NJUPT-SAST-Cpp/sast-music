#pragma once
#include "SongUrlInfoEntity.h"
#include <QList>
namespace NeteaseCloudMusic {
struct ManySongUrlInfoEntity {
    QList<SongUrlInfoEntity> data;
};
}; // namespace NeteaseCloudMusic