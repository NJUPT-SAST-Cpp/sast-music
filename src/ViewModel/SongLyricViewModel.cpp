#include "SongLyricViewModel.h"
#include <Service/NeteaseCloudMusic/CloudMusicClient.h>
#include <QDebug>

SongLyricViewModel::SongLyricViewModel(QObject* parent) : QAbstractListModel(parent) {}

SongLyricViewModel* SongLyricViewModel::create(QQmlEngine*, QJSEngine*) {
    return new SongLyricViewModel();
}

int SongLyricViewModel::rowCount(const QModelIndex& parent) const {
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    return model.count();
}

QVariant SongLyricViewModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid())
        return QVariant();

    // FIXME: Implement me!
    auto element = model[index.row()];
    switch (role) {
    case Role::TimeStamp:
        return (int)element.timeStamp;
    case Role::Lyric:
        return element.lyric;
    case Role::TrLyric:
        return element.trLyric;
    }
    return QVariant();
}

QHash<int, QByteArray> SongLyricViewModel::roleNames() const {
    static QHash<int, QByteArray> roles;
    // FIXME: Implement me!
    if (roles.isEmpty()) {
        roles[Role::TimeStamp] = "timeStamp";
        roles[Role::Lyric] = "lyric";
        roles[Role::TrLyric] = "trLyric";
    }
    return roles;
}

QList<SongLyric> SongLyricViewModel::ProcessLyric(QString lyrics)
{
    QList<SongLyric> processedlyric;
    QStringList lyriclines = lyrics.split("\n");
    SongLyric songlyric;
    for(QString&lyricline:lyriclines){
        QRegularExpression regularExpression("\\[(\\d+)?:(\\d+)?\\.(\\d+)?\\](.*)?");
        int index = 0;
        QRegularExpressionMatch match;
        match = regularExpression.match(lyricline, index);
        if(match.hasMatch()) {
            quint64 totalTime;
            totalTime = match.captured(1).toInt() * 60000 + match.captured(2).toInt() * 1000+match.captured(3).toInt() * 10;
            QString currentlyric =QString::fromStdString(match.captured(4).toStdString());
            songlyric.timeStamp = totalTime;
            songlyric.lyric = currentlyric;
        }
        processedlyric.append(songlyric);
    }
    return processedlyric;
}

void SongLyricViewModel::loadSongLyric(SongId songId) {
    // FIXME: Implement me!
    CloudMusicClient::getInstance()->getSongLyric(songId, [this](Result<SongLyricEntity> result) {
        if (result.isErr()) {
            emit loadSongLyricFailed();
            return;
        }
        auto lyrics = result.unwrap();
        if(lyrics.pureMusic){
            emit loadSongLyricSuccess();
            return;
        }
        else{
            beginResetModel();
            model = ProcessLyric(lyrics.trivial->lyric);
            endResetModel();
            emit loadSongLyricSuccess();
        }
    });
}

bool SongLyricViewModel::getHasLyric() const {
    return hasLyric;
}

void SongLyricViewModel::setHasLyric(bool newHasLyric) {
    if (hasLyric == newHasLyric)
        return;
    hasLyric = newHasLyric;
    emit hasLyricChanged();
}
