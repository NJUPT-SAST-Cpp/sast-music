#ifndef SONGLYRIC_H
#define SONGLYRIC_H

#include <Service/NeteaseCloudMusic/Response/SongLyricEntity.h>
#include <Utility/NeteaseCloudMusic>

struct SongLyric {
    quint64 timeStamp;
    QString lyric;
    QString trLyric;
};

#endif // SONGLYRIC_H