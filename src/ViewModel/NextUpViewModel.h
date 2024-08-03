#ifndef NEXTUPVIEWMODEL_H
#define NEXTUPVIEWMODEL_H

#include "Model/Song.h"
#include <QAbstractListModel>
#include <QtQml/QQmlEngine>
#include <Utility/NeteaseCloudMusic>

class NextUpViewModel : public QAbstractListModel {
    Q_OBJECT
    QML_NAMED_ELEMENT(NextUpViewModel)
    QML_SINGLETON

public:
    static NextUpViewModel* getInstance();
    static NextUpViewModel* create(QQmlEngine*, QJSEngine*);

    enum PlayMode {
        PlayOnce,
        ListRepeat,
        RepeatOne,
        Shuffle,
    };
    Q_ENUM(PlayMode)

    enum Role {
        SongId = Qt::UserRole,
        Name,
        Alias,
        Artists,
        Album,
        ImgUrl,
        Duration,
    };

    // Basic functionality:
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    QHash<int, QByteArray> roleNames() const override;

    void resetModel(const QList<Song>& newModel);
    void homingModel();
    void appendModel(const Song& song);
    void removeModel(int index);
    void removeModel(const Song& song);

    Song getPlayingSong();

    void loadSongsUrl(const QList<Song>& songs);

    QUrl getSongUrl(NeteaseCloudMusic::SongId songId);

    Q_INVOKABLE void playSongByIndex(qsizetype index);
    Song getNextSong();
    Song getPreviousSong();

    PlayMode getPlayMode() const;
    void setPlayMode(PlayMode newPlayMode);

signals:
    void loadSongsUrlSuccess();
    void loadSongsUrlFailed(QString message);

    void playModeChanged();
    void playingSongChanged(Song song);

private:
    explicit NextUpViewModel(QObject* parent = nullptr);

    QList<Song> model; // model for "Play Queue"
    Song playingSong;
    QHash<NeteaseCloudMusic::SongId, QUrl> songUrls;
    PlayMode playMode = PlayMode::ListRepeat;
    Q_PROPERTY(PlayMode playMode READ getPlayMode WRITE setPlayMode NOTIFY playModeChanged FINAL)
};

#endif // NEXTUPVIEWMODEL_H
