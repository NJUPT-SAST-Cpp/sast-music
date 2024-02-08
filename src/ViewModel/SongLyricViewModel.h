#ifndef SONGLYRICVIEWMODEL_H
#define SONGLYRICVIEWMODEL_H

#include "Model/SongLyric.h"
#include <QAbstractListModel>
#include <QtQml/QQmlEngine>
#include <Service/NeteaseCloudMusic/CloudMusicClient.h>
#include <Utility/NeteaseCloudMusic>

class SongLyricViewModel : public QAbstractListModel {
    Q_OBJECT
    QML_SINGLETON
    QML_NAMED_ELEMENT(SongLyricViewModel)

public:
    explicit SongLyricViewModel(QObject* parent = nullptr);
    static SongLyricViewModel* create(QQmlEngine*, QJSEngine*);

    enum Role {
        TimeStampRole = Qt::UserRole,
        ContentRole,
        TypeRole,
    };

    // Basic functionality:
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void loadSongLyric(SongId songId);

    //return QList with type info, used for each type individualy
    QList<SongLyric>&& lyricProcess(const QString& rawDataQstring, SongLyric::LyricType type);

    bool getHasLyric() const;
    void setHasLyric(bool newHasLyric);

signals:
    void loadSongLyricSuccess();
    void loadSongLyricFailed();

    void hasLyricChanged();

private:
    QList<SongLyric> model;
    bool hasLyric = false;
    bool hasTranslation = false;
    bool hasKaraoke = false;
    bool hasRomaji = false;
    Q_PROPERTY(bool hasLyric READ getHasLyric WRITE setHasLyric NOTIFY hasLyricChanged FINAL)
};

#endif // SONGLYRICVIEWMODEL_H
