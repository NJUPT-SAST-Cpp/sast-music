#ifndef SONGLYRICVIEWMODEL_H
#define SONGLYRICVIEWMODEL_H

#include "Model/SongLyric.h"
#include <QAbstractListModel>
#include <QtQml/QQmlEngine>
#include <Service/NeteaseCloudMusic/CloudMusicClient.h>
#include <Utility/NeteaseCloudMusic>
#include <qtmetamacros.h>
#include <qtypes.h>

class SongLyricViewModel : public QAbstractListModel {
    Q_OBJECT
    QML_SINGLETON
    QML_NAMED_ELEMENT(SongLyricViewModel)

public:
    static SongLyricViewModel* create(QQmlEngine*, QJSEngine*);
    static SongLyricViewModel* getInstance();

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
    qint64 getCurrentLyricIndex() const;
    void setHasLyric(bool newHasLyric);
    void setCurrentLyricIndex(qint64 newValue);

signals:
    void loadSongLyricSuccess();
    void loadSongLyricFailed();

    void hasLyricChanged();
    void currentLyricIndexChanged();

public slots: 
    void onTimestampChanged();

private:
    explicit SongLyricViewModel(QObject* parent = nullptr);
    
    QList<SongLyric> model;
    qint64 currentLyricIndex=0;
    bool hasLyric = false;
    bool hasTranslation = false; // TODO
    bool hasKaraoke = false;
    bool hasRomaji = false;
    Q_PROPERTY(bool hasLyric READ getHasLyric WRITE setHasLyric NOTIFY hasLyricChanged FINAL)
    Q_PROPERTY(qint64 currentLyricIndex READ getCurrentLyricIndex WRITE setCurrentLyricIndex NOTIFY currentLyricIndexChanged FINAL)
    void switchLyricIndex();
};

#endif // SONGLYRICVIEWMODEL_H
