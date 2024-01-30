#ifndef PLAYLISTVIEWMODEL_H
#define PLAYLISTVIEWMODEL_H

#include "Model/PlayList.h"
#include <QAbstractListModel>
#include <QtQml/QQmlEngine>
#include <Utility/NeteaseCloudMusic>

class PlayListViewModel : public QAbstractListModel {
    Q_OBJECT
    QML_NAMED_ELEMENT(PlayListViewModel)
    QML_SINGLETON

public:
    explicit PlayListViewModel(QObject* parent = nullptr);
    static PlayListViewModel* create(QQmlEngine*, QJSEngine*);

    enum Role {
        PlayListId = Qt::UserRole,
        Name,
        CoverImgUrl,
        CreatorName,
        UpdateTime,
        Description,
    };

    // Basic functionality:
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void loadPlayList(UserId userId);

    PlaylistId getLikedPlayListId() const;
    void setLikedPlayListId(PlaylistId newLikedPlayListId);

signals:
    void loadPlayListSuccess();
    void loadPlayListFailed(QString message);

    void likedPlayListIdChanged();

private:
    QList<PlayList> model;

    PlaylistId likedPlayListId;
    Q_PROPERTY(
        PlaylistId likedPlayListId READ getLikedPlayListId WRITE setLikedPlayListId NOTIFY likedPlayListIdChanged FINAL)
};

#endif // PLAYLISTVIEWMODEL_H
