#include "SongLyricViewModel.h"
#include "Utility/Tools.h"
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
    CloudMusicClient::getInstance()->getSongLyric(songId, [this](Result<SongLyricEntity> result) {
        if (result.isErr()) {
            emit loadSongLyricFailed(result.unwrapErr().message);
            return;
        }
        auto entity = result.unwrap();
        if (entity.pureMusic) {
            setHasLyric(false);
            return;
        }
        setHasLyric(true);
        Tools::parseLyric(entity.trivial->lyric);
        emit loadSongLyricSuccess();
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
