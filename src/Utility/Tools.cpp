#include "Tools.h"
#include "Model/SongLyric.h"
#include <QRandomGenerator>
#include <qdatetime.h>
#include <qtypes.h>

QString Tools::milsec2Time(quint64 milsec) {
    int seconds = milsec / 1000;
    int minutes = seconds / 60;
    seconds %= 60;
    return QString("%1:%2").arg(minutes).arg(seconds, 2, 10, QChar('0'));
}

quint64 Tools::time2milsec(const QString& time) {
    return QTime::fromString(time, "[mm:ss.zz]").msec();
}

int Tools::randomInt(int low, int high) {
    QRandomGenerator randomGenerator;
    return randomGenerator.bounded(low, high);
}

QList<SongLyric> Tools::parseLyric(const QString& lyric) {
    QList<SongLyric> lyrics;
    QStringList lines = lyric.split('\n');
    for (const auto& line : lines) {
        if (line.isEmpty())
            continue;
        auto index = line.indexOf(']') + 1;
        auto time = time2milsec(line.first(index));
        auto lyric = line.mid(index);
        lyrics.emplace_back(SongLyric{time, lyric});
    }
    return lyrics;
}
