#include "PlayListViewModel.h"
#include <Service/NeteaseCloudMusic/CloudMusicClient.h>
using namespace NeteaseCloudMusic;

PlayListViewModel::PlayListViewModel(QObject* parent) : QAbstractListModel(parent) {}

PlayListViewModel* PlayListViewModel::create(QQmlEngine*, QJSEngine*) {
    return new PlayListViewModel();
}

void PlayListViewModel::loadPlayList(UserId userId) {
    CloudMusicClient::getInstance()->getUserPlaylist(userId, 30, 0, false, [this](Result<ManyPlaylistEntity> result) {
        if (result.isErr()) {
            emit loadPlayListFailed(result.unwrapErr().message);
            return;
        }
        auto playLists = result.unwrap().playlist;
        if (playLists.isEmpty()) {
            emit loadPlayListSuccess();
            return;
        }
        auto likedPlayList = playLists.front();
        setLikedPlayListId(likedPlayList.id);
        playLists.pop_front();
        emit beginResetModel();
        model.clear();
        for (const auto& playList : playLists) {
            model.emplace_back(playList);
        }
        emit endResetModel();
        emit loadPlayListSuccess();
    });
}

PlaylistId PlayListViewModel::getLikedPlayListId() const {
    return likedPlayListId;
}

void PlayListViewModel::setLikedPlayListId(PlaylistId newLikedPlayListId) {
    if (likedPlayListId == newLikedPlayListId)
        return;
    likedPlayListId = newLikedPlayListId;
    emit likedPlayListIdChanged();
}

int PlayListViewModel::rowCount(const QModelIndex& parent) const {
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    return model.count();
}

QVariant PlayListViewModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid())
        return QVariant();
    auto element = model[index.row()];
    switch (role) {
    case Role::PlayListId:
        return (quint64)element.id;
    case Role::Name:
        return element.name;
    case Role::CreatorName:
        return element.creatorName;
    case Role::CoverImgUrl:
        return element.coverImgUrl;
    case Role::Description:
        return element.description;
    case Role::UpdateTime:
        return element.updateTime;
    }
    return QVariant();
}

QHash<int, QByteArray> PlayListViewModel::roleNames() const {
    static QHash<int, QByteArray> roles;
    if (roles.isEmpty()) {
        roles[Role::Name] = "name";
        roles[Role::PlayListId] = "playListId";
        roles[Role::CoverImgUrl] = "coverImgUrl";
        roles[Role::CreatorName] = "creatorName";
        roles[Role::UpdateTime] = "updateTime";
        roles[Role::Description] = "description";
    }
    return roles;
}
