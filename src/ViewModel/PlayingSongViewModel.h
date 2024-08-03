#ifndef PLAYINGSONGVIEWMODEL_H
#define PLAYINGSONGVIEWMODEL_H

#include "Utility/MusicPlayer.h"
#include <QAbstractListModel>
#include <QtMultimedia>
#include <QtQml/QQmlEngine>
#include <Utility/NeteaseCloudMusic>

struct Song;
class MusicPlayer;
class PlayingSongViewModel : public QObject {
    Q_OBJECT
    QML_NAMED_ELEMENT(PlayingSongViewModel)
    QML_SINGLETON

public:
    explicit PlayingSongViewModel(QObject* parent = nullptr);
    static PlayingSongViewModel* create(QQmlEngine*, QJSEngine*);
    ~PlayingSongViewModel();

    Q_INVOKABLE void play();
    Q_INVOKABLE void pause();
    Q_INVOKABLE void next();
    Q_INVOKABLE void previous();

    SongId getSongId() const;
    void setSongId(SongId newSongId);

    QString getName() const;
    void setName(const QString& newName);

    QString getAlias() const;
    void setAlias(const QString& newAlias);

    QString getArtists() const;
    void setArtists(const QString& newArtists);

    QString getAlbum() const;
    void setAlbum(const QString& newAlbum);

    QString getImgUrl() const;
    void setImgUrl(const QString& newImgUrl);

    QString getDurationTime() const;

    quint64 getDuration() const;
    void setDuration(quint64 newDuration);

    quint64 getTimeStamp() const;
    void setTimeStamp(quint64 newTimeStamp);

    bool getPlaying() const;
    void setPlaying(bool newPlaying);

signals:
    void playSongFailed(QString message);

    void songIdChanged();
    void nameChanged();
    void aliasChanged();
    void artistsChanged();
    void albumChanged();
    void imgUrlChanged();
    void durationChanged();
    void timeStampChanged();
    void playingChanged();

private slots:
    void onMediaStatusChanged(QMediaPlayer::MediaStatus status);
    void onMusicPositionChanged(qint64 position);
    void onPlaybackStateChanged(QMediaPlayer::PlaybackState status);
    void playSong();
    void setPlayingSong(const Song& song);

private:
    MusicPlayer* player;
    QUrl songUrl;
    bool playing = false;

    SongId songId = 0;
    QString name;
    QString alias;
    QString artists;
    QString album;
    QString imgUrl;
    QString durationTime = "0:00";
    quint64 duration = 0;
    quint64 timeStamp;
    Q_PROPERTY(SongId songId READ getSongId WRITE setSongId NOTIFY songIdChanged FINAL)
    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged FINAL)
    Q_PROPERTY(QString alias READ getAlias WRITE setAlias NOTIFY aliasChanged FINAL)
    Q_PROPERTY(QString artists READ getArtists WRITE setArtists NOTIFY artistsChanged FINAL)
    Q_PROPERTY(QString album READ getAlbum WRITE setAlbum NOTIFY albumChanged FINAL)
    Q_PROPERTY(QString imgUrl READ getImgUrl WRITE setImgUrl NOTIFY imgUrlChanged FINAL)
    Q_PROPERTY(QString durationTime READ getDurationTime NOTIFY durationChanged FINAL)
    Q_PROPERTY(quint64 duration READ getDuration WRITE setDuration NOTIFY durationChanged FINAL)
    Q_PROPERTY(quint64 timeStamp READ getTimeStamp WRITE setTimeStamp NOTIFY timeStampChanged FINAL)
    Q_PROPERTY(bool playing READ getPlaying WRITE setPlaying NOTIFY playingChanged FINAL)

    void load();
    void save();
};

#endif // PLAYINGSONGVIEWMODEL_H
