#include "SongViewModel.h"
#include <Service/NeteaseCloudMusic/CloudMusicClient.h>
#include <Utility/Tools.h>
using namespace NeteaseCloudMusic;

SongViewModel::SongViewModel(QObject* parent) : QAbstractListModel(parent) {}

SongViewModel* SongViewModel::getInstance() {
    static SongViewModel instance;
    return &instance;
}

SongViewModel* SongViewModel::create(QQmlEngine*, QJSEngine*) {
    auto instance = getInstance();
    QJSEngine::setObjectOwnership(instance, QQmlEngine::CppOwnership);
    return instance;
}

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
    case Role::Duration:
        return Tools::milsec2Time(element.duration);
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

void SongViewModel::loadSongs(PlaylistId playListId) {
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

void SongViewModel::resetModel(const QList<Song>& model) {
    emit beginResetModel();
    this->model = model;
    emit endResetModel();
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

QString SongViewModel::getName() const {
    return name;
}

void SongViewModel::setName(const QString& newName) {
    if (name == newName)
        return;
    name = newName;
    emit nameChanged();
}

QString SongViewModel::getCoverImgUrl() const {
    return coverImgUrl;
}

void SongViewModel::setCoverImgUrl(const QString& newCoverImgUrl) {
    if (coverImgUrl == newCoverImgUrl)
        return;
    coverImgUrl = newCoverImgUrl;
    emit coverImgUrlChanged();
}

QString SongViewModel::getCreatorName() const {
    return creatorName;
}

void SongViewModel::setCreatorName(const QString& newCreatorName) {
    if (creatorName == newCreatorName)
        return;
    creatorName = newCreatorName;
    emit creatorNameChanged();
}

QString SongViewModel::getDescription() const {
    return description;
}

void SongViewModel::setDescription(const QString& newDescription) {
    if (description == newDescription)
        return;
    description = newDescription;
    emit descriptionChanged();
}

QString SongViewModel::getUpdateTime() const {
    return updateTime;
}

void SongViewModel::setUpdateTime(const QString& newUpdateTime) {
    if (updateTime == newUpdateTime)
        return;
    updateTime = newUpdateTime;
    emit updateTimeChanged();
}

PlaylistId SongViewModel::getPlaylistId() const {
    return playlistId;
}

void SongViewModel::setPlaylistId(PlaylistId newPlaylistId) {
    if (playlistId == newPlaylistId)
        return;
    playlistId = newPlaylistId;
    emit playlistIdChanged();
}
