#include "NextUpViewModel.h"
#include "Service/NeteaseCloudMusic/CloudMusicClient.h"
#include "Service/NeteaseCloudMusic/Response/BasicDef.h"
#include "Utility/SettingsUtils.h"
#include <Service/NeteaseCloudMusic/MusicLevel.h>
#include <Utility/NeteaseCloudMusic>
#include <Utility/Tools.h>
#include <Utility/RandomUtils.h>
// #include "SongLyricViewModel.h"

NextUpViewModel::NextUpViewModel(QObject* parent) : QAbstractListModel(parent) {
    auto settings = SettingsUtils::getInstance();
    playingSong.id = settings->value("SongId").toULongLong();
    playingSong.name = settings->value("Name").toString();
    playingSong.alias = settings->value("Alias").toString();
    playingSong.artists = settings->value("Artists").toString();
    playingSong.album = settings->value("Album").toString();
    playingSong.imgUrl = settings->value("ImgUrl").toString();
    playingSong.duration = settings->value("Duration").toULongLong();
    songUrls[playingSong.id] = QUrl(settings->value("SongUrl").toString());
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
    homingModel();
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
        // TODO (initially completed)

        // // for testing task5 (In SongLyricViewModel.cpp)
        // std::thread t([this]() {
        //     auto lyricVM_ptr = new SongLyricViewModel(this);
        //     qDebug() << "Created!";
        //     lyricVM_ptr->loadSongLyric(playingSong.id);
        //     qDebug() << "Loaded!";
        // });
        // t.detach();

        if (playingSong.id == model.last().id) {
            song = model.first();
        } else {
            int cntIndex = model.indexOf(playingSong);

            if (cntIndex == -1)
                song = model.first();
            else
                song = model[cntIndex + 1];
        }
        break;
    }
    case PlayMode::RepeatOne: {
        // TODO (initially completed)
        song = playingSong;
        break;
    }
    case PlayMode::Shuffle: {
        // TODO (initially completed)
        int random_index = randomInt(0, model.count());
        song = model[random_index];
        break;
    }
    }
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
