#ifndef SONGVIEWMODEL_H
#define SONGVIEWMODEL_H

#include "Model/Song.h"
#include <QAbstractListModel>
#include <QtQml/QQmlEngine>
#include <Utility/NeteaseCloudMusic>

class SongViewModel : public QAbstractListModel {
    Q_OBJECT
    QML_NAMED_ELEMENT(SongViewModel)

public:
    explicit SongViewModel(QObject* parent = nullptr);

    enum Role {
        SongId = Qt::UserRole,
        Name,
        Alias,
        Artists,
        Album,
        ImgUrl,
    };

    // Basic functionality:
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void loadSongs(PlaylistId playListId);
    Q_INVOKABLE void loadAllLikedSongs();
    Q_INVOKABLE void loadLikedSongs(PlaylistId playlistId);

    int getCount() const;
    void setCount(int newCount);

signals:
    void loadSongsSuccess();
    void loadSongsFailed(QString message);

    void countChanged();

private:
    QList<Song> model;
    QList<Song> likedSongModel;
    int count = 0;
    Q_PROPERTY(int count READ getCount WRITE setCount NOTIFY countChanged FINAL)
};

#endif // SONGVIEWMODEL_H
