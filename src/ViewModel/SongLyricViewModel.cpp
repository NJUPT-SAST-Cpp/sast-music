#include "SongLyricViewModel.h"
#include <Service/NeteaseCloudMusic/CloudMusicClient.h>
#include <Service/NeteaseCloudMusic/MusicLevel.h>
#include <Utility/NeteaseCloudMusic>

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
    auto& element = model[index.row()];
    switch (role) {
    case Role::Lyric:
        return element.lyric;
    case Role::TrLyric:
        return element.trLyric;
    case Role::TimeStamp:
        return (quint64)element.timeStamp;
    }
    return QVariant();
}

QHash<int, QByteArray> SongLyricViewModel::roleNames() const {
    static QHash<int, QByteArray> roles;
    // FIXME: Implement me!
    if (roles.isEmpty()){
        roles[Role::Lyric] = "lyric";
        roles[Role::TrLyric] = "trLyric";
        roles[Role::TimeStamp] = "timeStamp";
    }
    return roles;
}

void SongLyricViewModel::loadSongLyric(SongId songId) {
    // FIXME: Implement me!
    CloudMusicClient::getInstance()->getSongLyric(songId, [this](Result<SongLyricEntity> result){
    if (result.isErr()) {
        emit loadSongLyricFailed();
        return;
    }
    
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
