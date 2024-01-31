#include "LikedSongViewModel.h"
#include "SongViewModel.h"
#include <Service/NeteaseCloudMusic/CloudMusicClient.h>
#include <Utility/Tools.h>
using namespace NeteaseCloudMusic;

LikedSongViewModel::LikedSongViewModel(QObject* parent) : QAbstractListModel(parent) {}

LikedSongViewModel* LikedSongViewModel::create(QQmlEngine*, QJSEngine*) {
    return new LikedSongViewModel();
}

int LikedSongViewModel::rowCount(const QModelIndex& parent) const {
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    return std::min((qsizetype)12, model.count());
}

QVariant LikedSongViewModel::data(const QModelIndex& index, int role) const {
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
    case Role::Duration:
        return Tools::milsec2Time(element.duration);
    }
    return QVariant();
}

QHash<int, QByteArray> LikedSongViewModel::roleNames() const {
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

void LikedSongViewModel::loadLikedSongs(PlaylistId playListId) {
    CloudMusicClient::getInstance()->getPlaylistDetail(playListId, [this](Result<PlaylistDetailEntity> result) {
        if (result.isErr()) {
            emit loadSongsFailed(result.unwrapErr().message);
            return;
        }
        auto songs = result.unwrap().tracks;
        emit beginResetModel();
        model.clear();
        for (const auto& song : songs) {
            model.emplace_back(song);
        }
        emit endResetModel();
        setCount(songs.count());
        emit loadSongsSuccess();
    });
}

void LikedSongViewModel::loadAllLikedSongs() {
    SongViewModel::getInstance()->resetModel(model);
}

int LikedSongViewModel::getCount() const {
    return count;
}

void LikedSongViewModel::setCount(int newCount) {
    if (count == newCount)
        return;
    count = newCount;
    emit countChanged();
}
