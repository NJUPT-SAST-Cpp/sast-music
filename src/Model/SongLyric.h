#ifndef SONGLYRIC_H
#define SONGLYRIC_H

#include <Service/NeteaseCloudMusic/Response/SongLyricEntity.h>
#include <Utility/NeteaseCloudMusic>

struct SongLyric {
    quint64 timeStamp;
    QString lyric;
    //QString trLyric; //abandon
    enum LyricType{
        Common,
        Translation,
        Karaoke,
        Romaji
    }type;
};

#endif // SONGLYRIC_H