#ifndef SONGVIEWMODEL_H
#define SONGVIEWMODEL_H

#include "Model/Song.h"
#include "Service/NeteaseCloudMusic/Response/BasicDef.h"
#include <QAbstractListModel>
#include <QtQml/QQmlEngine>
#include <Utility/NeteaseCloudMusic>
#include <qtmetamacros.h>

class SongViewModel : public QAbstractListModel {
    Q_OBJECT
    QML_NAMED_ELEMENT(SongViewModel)
    QML_SINGLETON

public:
    static SongViewModel* getInstance();
    static SongViewModel* create(QQmlEngine*, QJSEngine*);

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

    Q_INVOKABLE void loadSongs(PlaylistId playListId);
    Q_INVOKABLE void playSongByIndex(int index);
    Q_INVOKABLE void playAllSongs();
    Q_INVOKABLE void loadAndPlayAllSongs(PlaylistId playListId);

    Q_INVOKABLE void loadSongsoutside(PlaylistId playListId);

    void resetModel(const QList<Song>& model);

    int getCount() const;
    void setCount(int newCount);

    QString getName() const;
    void setName(const QString& newName);

    QString getCoverImgUrl() const;
    void setCoverImgUrl(const QString& newCoverImgUrl);

    QString getCreatorName() const;
    void setCreatorName(const QString& newCreatorName);

    QString getDescription() const;
    void setDescription(const QString& newDescription);

    QString getUpdateTime() const;
    void setUpdateTime(const QString& newUpdateTime);

    PlaylistId getPlaylistId() const;
    void setPlaylistId(PlaylistId newPlaylistId);

signals:
    void loadSongsSuccess();
    void loadSongsFailed(QString message);

    void prepareForPlaying();

    void countChanged();
    void nameChanged();
    void coverImgUrlChanged();
    void creatorNameChanged();
    void descriptionChanged();
    void updateTimeChanged();

    void playlistIdChanged();

private:
    explicit SongViewModel(QObject* parent = nullptr);

    QList<Song> model;

    int count = 0;
    PlaylistId playlistId;
    QString name;
    QString coverImgUrl;
    QString creatorName;
    QString updateTime;
    QString description;
    Q_PROPERTY(int count READ getCount WRITE setCount NOTIFY countChanged FINAL)
    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged FINAL)
    Q_PROPERTY(QString coverImgUrl READ getCoverImgUrl WRITE setCoverImgUrl NOTIFY coverImgUrlChanged FINAL)
    Q_PROPERTY(QString creatorName READ getCreatorName WRITE setCreatorName NOTIFY creatorNameChanged FINAL)
    Q_PROPERTY(QString description READ getDescription WRITE setDescription NOTIFY descriptionChanged FINAL)
    Q_PROPERTY(QString updateTime READ getUpdateTime WRITE setUpdateTime NOTIFY updateTimeChanged FINAL)
    Q_PROPERTY(PlaylistId playlistId READ getPlaylistId WRITE setPlaylistId NOTIFY playlistIdChanged FINAL)
};

#endif // SONGVIEWMODEL_H
