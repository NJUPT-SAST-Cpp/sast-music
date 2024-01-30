#pragma once
#include "BasicDef.h"
#include <QString>
namespace NeteaseCloudMusic {
struct SongUrlInfoEntity {
    SongId id;
    QString url;
    // json field: br
    int64_t bandRate;
    int64_t size;
    QString md5;
    QString type;
    QString level;
    bool payed;
    // json field: time, unit: ms
    int64_t duration;
};
}; // namespace NeteaseCloudMusic