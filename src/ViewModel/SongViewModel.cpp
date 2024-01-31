#include "SongViewModel.h"
#include "Model/Song.h"
#include "Service/NeteaseCloudMusic/Response/SongInfoEntity.h"
#include <Service/NeteaseCloudMusic/CloudMusicClient.h>
#include <qdatetime.h>

using namespace NeteaseCloudMusic;

SongViewModel::SongViewModel(QObject* parent) : QAbstractListModel(parent) {}

int SongViewModel::rowCount(const QModelIndex& parent) const {
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    return model.count();
}

QVariant SongViewModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid())
        return QVariant();
    auto element = model[index.row()];
    switch (role) {
    case Role::SongId:
        return (int)element.id;
    case Role::Name:
        return element.name;
    case Role::Alias:
        return element.alias;
    case Role::Artists:
        return element.artists;
    case Role::Album:
        return element.album;
    case Role::ImgUrl:
        return element.imgUrl;
    case Role::Duration: {
        int seconds = element.duration / 1000;
        int minutes = seconds / 60;
        seconds %= 60;
        return QString("%1:%2").arg(minutes).arg(seconds, 2, 10, QChar('0'));
    }
    }
    return QVariant();
}

QHash<int, QByteArray> SongViewModel::roleNames() const {
    static QHash<int, QByteArray> roles;
    if (roles.isEmpty()) {
        roles[Role::Name] = "name";
        roles[Role::SongId] = "songId";
        roles[Role::Alias] = "alias";
        roles[Role::Artists] = "artists";
        roles[Role::Album] = "album";
        roles[Role::ImgUrl] = "imgUrl";
        roles[Role::Duration] = "duration";
    }
    return roles;
}

void SongViewModel::loadLikedSongs(PlaylistId playListId) {
    CloudMusicClient::getInstance()->getPlaylistDetail(playListId, [this](Result<PlaylistDetailEntity> result) {
        if (result.isErr()) {
            emit loadSongsFailed(result.unwrapErr().message);
            return;
        }
        auto songs = result.unwrap().tracks;
        for (const auto& song : songs) {
            likedSongModel.emplace_back(song);
        }
        emit beginResetModel();
        auto size = std::min(songs.count(), (qsizetype)12);
        model = likedSongModel.sliced(0, size);
        emit endResetModel();
        setCount(songs.count());
        emit loadSongsSuccess();
    });
}

void SongViewModel::loadAllLikedSongs() {
    emit beginResetModel();
    model = std::move(likedSongModel);
    emit endResetModel();
}

void SongViewModel::loadSongs(PlaylistId playListId) {
    CloudMusicClient::getInstance()->getPlaylistDetail(playListId, [this](Result<PlaylistDetailEntity> result) {
        if (result.isErr()) {
            emit loadSongsFailed(result.unwrapErr().message);
            return;
        }
        auto songs = result.unwrap().tracks;
        emit beginResetModel();
        for (const auto& song : songs) {
            model.emplace_back(song);
        }
        emit endResetModel();
        setCount(songs.count());
        emit loadSongsSuccess();
    });
}

int SongViewModel::getCount() const {
    return count;
}

void SongViewModel::setCount(int newCount) {
    if (count == newCount)
        return;
    count = newCount;
    emit countChanged();
}
