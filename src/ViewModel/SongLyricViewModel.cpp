#include "SongLyricViewModel.h"
#include <Service/NeteaseCloudMusic/CloudMusicClient.h>

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
    return QVariant();
}

QHash<int, QByteArray> SongLyricViewModel::roleNames() const {
    static QHash<int, QByteArray> roles;
    // FIXME: Implement me!
    return roles;
}

void SongLyricViewModel::loadSongLyric(SongId songId) {
    // FIXME: Implement me!
    CloudMusicClient::getInstance()->getSongLyric(songId, [this](Result<SongLyricEntity> result) {
        if (result.isErr()) {
            qDebug() << "err";
            emit loadSongLyricFailed();
            return;
        }
        QString songlyric = result.unwrap().trivial->lyric;
        QList<SongLyric> splitlyric = splitSongLyric(songlyric);

        emit loadSongLyricSuccess();
    });
}

QList<SongLyric> SongLyricViewModel::splitSongLyric(const QString StringLyric)
{
    QList<QString> splitStringLyric = StringLyric.split("\n");
    QList<SongLyric> splitLyric;
    QString StringLyricpiece, TimeStamptext;
    qint64 timeStamp_ms;
    SongLyric songLyric;
    for (int i=0; i<splitStringLyric.count(); i++)
    {
        if (StringLyricpiece.compare("") == 0) continue;
        StringLyricpiece = splitStringLyric[i];
        TimeStamptext = StringLyricpiece.left(10);
        songLyric.timeStamp = TimeStamptext.mid(1, 2).toInt()*60*1000 + TimeStamptext.mid(4, 5).toDouble()*1000;
        songLyric.lyric = StringLyricpiece.right(StringLyricpiece.length()-10);
        splitLyric.append(songLyric);
    }
    return splitLyric;
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
