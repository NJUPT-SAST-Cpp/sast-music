#ifndef TOOLS_H
#define TOOLS_H

#include "Model/SongLyric.h"
#include <QList>
#include <QString>

struct Tools {
    static QString milsec2Time(quint64 milsec);
    static quint64 time2milsec(const QString& time);
    static int randomInt(int low, int high);
    static QList<SongLyric> parseLyric(const QString& lyric);
};

#endif // TOOLS_H
