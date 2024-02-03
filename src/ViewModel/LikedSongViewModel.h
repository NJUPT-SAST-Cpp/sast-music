#ifndef LIKEDSONGVIEWMODEL_H
#define LIKEDSONGVIEWMODEL_H

#include "Model/Song.h"
#include <QAbstractListModel>
#include <QtQml/QQmlEngine>
#include <Utility/NeteaseCloudMusic>
#include <qtmetamacros.h>

#include <SongLyricViewModel.h>


class LikedSongViewModel : public QAbstractListModel {
    Q_OBJECT
    QML_NAMED_ELEMENT(LikedSongViewModel)
    QML_SINGLETON

public:
    explicit LikedSongViewModel(QObject* parent = nullptr);
    static LikedSongViewModel* create(QQmlEngine*, QJSEngine*);

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

    Q_INVOKABLE void loadLikedSongs(PlaylistId playlistId);
    Q_INVOKABLE void loadAllLikedSongs();
    Q_INVOKABLE void playSongByIndex(int index);
    Q_INVOKABLE void playAllSongs();

    int getCount() const;
    void setCount(int newCount);

signals:
    void loadSongsSuccess();
    void loadSongsFailed(QString message);

    void countChanged();

private:
    QList<Song> model;
    QList<Song> allLikedSongs;

    //SongLyricViewModel test;

    int count = 0;
    Q_PROPERTY(int count READ getCount WRITE setCount NOTIFY countChanged FINAL)
};

#endif // LIKEDSONGVIEWMODEL_H
