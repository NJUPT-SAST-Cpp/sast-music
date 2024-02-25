#include "NextUpViewModel.h"
#include "Service/NeteaseCloudMusic/CloudMusicClient.h"
#include "Service/NeteaseCloudMusic/Response/BasicDef.h"
#include "Utility/SettingsUtils.h"
#include <Service/NeteaseCloudMusic/MusicLevel.h>
#include <Utility/NeteaseCloudMusic>
#include <Utility/Tools.h>
#include <ctime>

NextUpViewModel::NextUpViewModel(QObject* parent) : QAbstractListModel(parent) {
    e1.seed(time(0));
    auto songId = SettingsUtils::getInstance()->value("SongId").toULongLong();
    auto songUrl = SettingsUtils::getInstance()->value("SongUrl").toString();
    songUrls[songId] = QUrl(songUrl);
    playingSong.id = songId;
}

NextUpViewModel* NextUpViewModel::getInstance() {
    static NextUpViewModel instance;
    return &instance;
}

NextUpViewModel* NextUpViewModel::create(QQmlEngine*, QJSEngine*) {
    auto instance = getInstance();
    QJSEngine::setObjectOwnership(instance, QQmlEngine::CppOwnership);
    return instance;
}

int NextUpViewModel::rowCount(const QModelIndex& parent) const {
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    return model.count();
}

QVariant NextUpViewModel::data(const QModelIndex& index, int role) const {
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

QHash<int, QByteArray> NextUpViewModel::roleNames() const {
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

void NextUpViewModel::resetModel(const QList<Song>& newModel) {
    if (newModel.count() == 0) {
        qDebug()<<"reset failed!";
        return;
    }
    beginResetModel();
    model = newModel.sliced(1, newModel.count() - 1);
    endResetModel();
    playingSong = newModel[0];
    emit playingSongChanged(playingSong);
    loadSongsUrl(newModel);
}

void NextUpViewModel::homingModel() {
    if (playingSong.id == 0)
        return;
    beginInsertRows(QModelIndex(), model.count(), model.count());
    model.append(playingSong);
    endInsertRows();
}

void NextUpViewModel::appendModel(const Song& song) {
    auto index = model.indexOf(song);
    if (index >= 0) {
        removeModel(index);
    } else {
        homingModel();
        playingSong = song;
        loadSongsUrl({song});
        emit playingSongChanged(playingSong);
    }
}

void NextUpViewModel::removeModel(int index) {
    homingModel();
    if (index < 0 || index >= model.count())
        return;
    playingSong = model[index];
    emit playingSongChanged(playingSong);
    beginRemoveRows(QModelIndex(), index, index);
    model.removeAt(index);
    endRemoveRows();
    emit loadSongsUrlSuccess();
}

void NextUpViewModel::removeModel(const Song& song) {
    auto index = model.indexOf(song);
    if (index < 0)
        return;
    playingSong = model[index];
    emit playingSongChanged(playingSong);
    beginRemoveRows(QModelIndex(), index, index);
    model.removeAt(index);
    endRemoveRows();
    emit loadSongsUrlSuccess();
}

void NextUpViewModel::removeModelforall(const Song& song) {
    auto index = model.indexOf(song);
    if (index < 0)
        return;
    playingSong = model[index];
    beginRemoveRows(QModelIndex(), index, index);
    model.removeAt(index);
    endRemoveRows();
    emit loadSongsUrlSuccess();
}

Song NextUpViewModel::getPlayingSong() {
    return playingSong;
}

void NextUpViewModel::loadSongsUrl(const QList<Song>& songs) {
    QList<NeteaseCloudMusic::SongId> songIds;
    for (const auto& song : songs) {
        songIds.push_back(song.id);
    }
    QStringView level;
    switch (SettingsUtils::getInstance()->value("MusicQualityIndex").toInt()) {
    case 0:
        level = MusicLevel::Standard;
        break;
    case 1:
        level = MusicLevel::Higher;
        break;
    case 2:
        level = MusicLevel::ExHigh;
        break;
    case 3:
        level = MusicLevel::Lossless;
        break;
    case 4:
        level = MusicLevel::HiRes;
        break;
    }
    CloudMusicClient::getInstance()->getSongsUrl(songIds, level, [this](Result<ManySongUrlInfoEntity> result) {
        if (result.isErr()) {
            emit loadSongsUrlFailed(result.unwrapErr().message);
            return;
        }
        auto songUrls = result.unwrap().data;
        for (const auto& songUrl : songUrls) {
            this->songUrls[songUrl.id] = QUrl(songUrl.url);
        }
        emit loadSongsUrlSuccess();
    });
}

void NextUpViewModel::loadSongsUrlnotemit(const QList<Song>& songs) {
    QList<NeteaseCloudMusic::SongId> songIds;
    for (const auto& song : songs) {
        songIds.push_back(song.id);
    }
    QStringView level;
    switch (SettingsUtils::getInstance()->value("MusicQualityIndex").toInt()) {
    case 0:
        level = MusicLevel::Standard;
        break;
    case 1:
        level = MusicLevel::Higher;
        break;
    case 2:
        level = MusicLevel::ExHigh;
        break;
    case 3:
        level = MusicLevel::Lossless;
        break;
    case 4:
        level = MusicLevel::HiRes;
        break;
    }
    CloudMusicClient::getInstance()->getSongsUrl(songIds, level, [this](Result<ManySongUrlInfoEntity> result) {
        if (result.isErr()) {
            emit loadSongsUrlFailed(result.unwrapErr().message);
            return;
        }
        auto songUrls = result.unwrap().data;
        for (const auto& songUrl : songUrls) {
            this->songUrls[songUrl.id] = songUrl.url;
        }
        emit loadSongsUrlSuccess();
    });
}


QUrl NextUpViewModel::getSongUrl(NeteaseCloudMusic::SongId songId) {
    return songUrls[songId];
}

void NextUpViewModel::playSongByIndex(qsizetype index) {
    NextUpViewModel::getInstance()->removeModel(index);
}

Song NextUpViewModel::getNextSong() {
    if (model.isEmpty())
        return Song{};
    Song song;
    switch (playMode) {
    case PlayMode::PlayOnce: {
        song.id = 0;
        break;
    }
    case PlayMode::ListRepeat: {
        // TODO
        if(playingSong.id == 0){
            song = model[0];
            break;
        }
        auto num = model.indexOf(playingSong);
        auto size = model.size();
        if(num == size-1){
            song = model[0];
        }else{
            song = model[num+1];
        }
        break;
    }
    case PlayMode::RepeatOne: {
        // TODO
        if(playingSong.id == 0){
            return Song{};
        }
        song=playingSong;
        break;
    }
    case PlayMode::Shuffle: {
        // TODO
        srand(unsigned(time(NULL)));
        int num=rand()%model.size();
        song = model[num];
        break;
       }
    }
    emit playingSongChanged(song);
    return song;
}

Song NextUpViewModel::getPreviousSong() {
    if (model.isEmpty())
        return Song{};
    return model.last();
}

NextUpViewModel::PlayMode NextUpViewModel::getPlayMode() const {
    return playMode;
}

void NextUpViewModel::setPlayMode(PlayMode newPlayMode) {
    if (playMode == newPlayMode)
        return;
    playMode = newPlayMode;
    emit playModeChanged();
}
