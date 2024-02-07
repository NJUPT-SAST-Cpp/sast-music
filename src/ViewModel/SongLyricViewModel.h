#ifndef SONGLYRICVIEWMODEL_H
#define SONGLYRICVIEWMODEL_H

#include "Model/SongLyric.h"
#include <QAbstractListModel>
#include <QtQml/QQmlEngine>
#include <Utility/NeteaseCloudMusic>

class SongLyricViewModel : public QAbstractListModel {
    Q_OBJECT
    QML_SINGLETON
    QML_NAMED_ELEMENT(SongLyricViewModel)

public:
    explicit SongLyricViewModel(QObject* parent = nullptr);
    static SongLyricViewModel* create(QQmlEngine*, QJSEngine*);

    enum Role {
        TimeStamp,
        Lyric,
        TrLyric,
    };

    // Basic functionality:
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void loadSongLyric(SongId songId);

    bool getHasLyric() const;
    void setHasLyric(bool newHasLyric);

    // task5 function
    QList<SongLyric> parseSongLyricEntity(const QString& rawSongLyricData);

signals:
    void loadSongLyricSuccess();
    void loadSongLyricFailed();

    void hasLyricChanged();

private:
    QList<SongLyric> model;
    bool hasLyric = false;
    Q_PROPERTY(bool hasLyric READ getHasLyric WRITE setHasLyric NOTIFY hasLyricChanged FINAL)
};

#endif // SONGLYRICVIEWMODEL_H
