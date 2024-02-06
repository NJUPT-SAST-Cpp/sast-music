#include "PlayingSongViewModel.h"
#include "NextUpViewModel.h"
#include "Utility/MusicPlayer.h"
#include <Service/NeteaseCloudMusic/CloudMusicClient.h>
#include <Utility/MusicPlayer.h>
#include <Utility/SettingsUtils.h>
#include <Utility/Tools.h>

PlayingSongViewModel::PlayingSongViewModel(QObject* parent) : QObject{parent}, player(new MusicPlayer(this)) {
    load();
    player->setPosition(timeStamp);
    QObject::connect(NextUpViewModel::getInstance(), &NextUpViewModel::loadSongsUrlSuccess, this,
                     &PlayingSongViewModel::playSong);
    QObject::connect(NextUpViewModel::getInstance(), &NextUpViewModel::playingSongChanged, this,
                     &PlayingSongViewModel::setPlayingSong);
    // TODO: connect signals and slots
    QObject::connect(player,&QMediaPlayer::mediaStatusChanged,this,&PlayingSongViewModel::onMediaStatusChanged);
    QObject::connect(player,&QMediaPlayer::positionChanged,this,&PlayingSongViewModel::onMusicPositionChanged);
    QObject::connect(player,&QMediaPlayer::playbackStateChanged,this,&PlayingSongViewModel::onPlayStateChanged);
}

PlayingSongViewModel::~PlayingSongViewModel() {
    save();
}

PlayingSongViewModel* PlayingSongViewModel::create(QQmlEngine*, QJSEngine*) {
    return new PlayingSongViewModel();
}

void PlayingSongViewModel::playSong() {
    songId = NextUpViewModel::getInstance()->getPlayingSong().id;
    if (songId == 0)
        return;
    songUrl = NextUpViewModel::getInstance()->getSongUrl(songId);
    if (songUrl.toString().isEmpty()) {
        emit playSongFailed("no copyright in song");
        next();
        return;
    }
    player->play(songUrl);
}

void PlayingSongViewModel::play() {
    playSong();
}

void PlayingSongViewModel::pause() {
    player->pause();
}

void PlayingSongViewModel::next() {
    auto song = NextUpViewModel::getInstance()->getNextSong();
    if (song.id == 0)
        return;
    player->play(NextUpViewModel::getInstance()->getSongUrl(song.id));
    NextUpViewModel::getInstance()->removeModel(song);
}

void PlayingSongViewModel::previous() {
    auto song = NextUpViewModel::getInstance()->getPreviousSong();
    if (song.id == 0)
        return;
    player->play(NextUpViewModel::getInstance()->getSongUrl(song.id));
    NextUpViewModel::getInstance()->removeModel(song);
}

void PlayingSongViewModel::onMediaStatusChanged(QMediaPlayer::MediaStatus status) {
    if (status == QMediaPlayer::EndOfMedia) {
        next();
    } else if (status == QMediaPlayer::NoMedia) {
        emit playSongFailed("No media");
    }
}

void PlayingSongViewModel::onMusicPositionChanged(qint64 position) {
    timeStamp = position;
    emit timeStampChanged();
}

void PlayingSongViewModel::onPlayStateChanged(QMediaPlayer::PlaybackState status) {
    if (status == QMediaPlayer::PlayingState) {
        setPlaying(true);
    } else {
        setPlaying(false);
    }
}

void PlayingSongViewModel::setPlayingSong(const Song& song) {
    setSongId(song.id);
    setName(song.name);
    setAlias(song.alias);
    setArtists(song.artists);
    setAlbum(song.album);
    setImgUrl(song.imgUrl);
    setDuration(song.duration);
}

bool PlayingSongViewModel::getPlaying() const {
    return playing;
}

void PlayingSongViewModel::setPlaying(bool newPlaying) {
    if (playing == newPlaying)
        return;
    playing = newPlaying;
    emit playingChanged();
}
SongId PlayingSongViewModel::getSongId() const {
    return songId;
}

void PlayingSongViewModel::setSongId(SongId newSongId) {
    if (songId == newSongId)
        return;
    songId = newSongId;
    emit songIdChanged();
}

QString PlayingSongViewModel::getName() const {
    return name;
}

void PlayingSongViewModel::setName(const QString& newName) {
    if (name == newName)
        return;
    name = newName;
    emit nameChanged();
}

QString PlayingSongViewModel::getAlias() const {
    return alias;
}

void PlayingSongViewModel::setAlias(const QString& newAlias) {
    if (alias == newAlias)
        return;
    alias = newAlias;
    emit aliasChanged();
}

QString PlayingSongViewModel::getArtists() const {
    return artists;
}

void PlayingSongViewModel::setArtists(const QString& newArtists) {
    if (artists == newArtists)
        return;
    artists = newArtists;
    emit artistsChanged();
}

QString PlayingSongViewModel::getAlbum() const {
    return album;
}

void PlayingSongViewModel::setAlbum(const QString& newAlbum) {
    if (album == newAlbum)
        return;
    album = newAlbum;
    emit albumChanged();
}

QString PlayingSongViewModel::getImgUrl() const {
    return imgUrl;
}

void PlayingSongViewModel::setImgUrl(const QString& newImgUrl) {
    if (imgUrl == newImgUrl)
        return;
    imgUrl = newImgUrl;
    emit imgUrlChanged();
}

QString PlayingSongViewModel::getDurationTime() const {
    return Tools::milsec2Time(duration);
}

quint64 PlayingSongViewModel::getDuration() const {
    return duration;
}

void PlayingSongViewModel::setDuration(quint64 newDuration) {
    if (duration == newDuration)
        return;
    duration = newDuration;
    emit durationChanged();
}

quint64 PlayingSongViewModel::getTimeStamp() const {
    return timeStamp;
}

void PlayingSongViewModel::setTimeStamp(quint64 newTimeStamp) {
    if (timeStamp == newTimeStamp)
        return;
    timeStamp = newTimeStamp;
    player->setPosition(timeStamp);
    emit timeStampChanged();
}

void PlayingSongViewModel::load() {
    auto settings = SettingsUtils::getInstance();
    songId = settings->value("SongId").toULongLong();
    name = settings->value("Name").toString();
    alias = settings->value("Alias").toString();
    artists = settings->value("Artists").toString();
    album = settings->value("Album").toString();
    imgUrl = settings->value("ImgUrl").toString();
    duration = settings->value("Duration").toULongLong();
    timeStamp = settings->value("TimeStamp").toULongLong();
    songUrl = settings->value("SongUrl").toString();
}

void PlayingSongViewModel::save() {
    auto settings = SettingsUtils::getInstance();
    settings->setValue("SongId", (quint64)songId);
    settings->setValue("Name", name);
    settings->setValue("Alias", alias);
    settings->setValue("Artists", artists);
    settings->setValue("Album", album);
    settings->setValue("ImgUrl", imgUrl);
    settings->setValue("Duration", duration);
    settings->setValue("TimeStamp", timeStamp);
    settings->setValue("SongUrl", songUrl);
}
