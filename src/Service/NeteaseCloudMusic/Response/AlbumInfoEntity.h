#pragma once
#include "BasicDef.h"
#include <QDateTime>
#include <QString>
namespace NeteaseCloudMusic {
struct AlbumInfoEntity {
    AlbumId id;
    QString name;
    // 可选
    QString picUrl;
};
}; // namespace NeteaseCloudMusic