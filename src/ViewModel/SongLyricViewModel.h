#ifndef SONGLYRICVIEWMODEL_H
#define SONGLYRICVIEWMODEL_H

#include "Model/SongLyric.h"
#include "Model/Song.h"
#include <QAbstractListModel>
#include <QtQml/QQmlEngine>
#include <Utility/NeteaseCloudMusic>
#include <Service/NeteaseCloudMusic/CloudMusicClient.h>

#include <QTimer>

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
    Q_INVOKABLE void loadSongLyric_outside(QVariant songId);

    Q_INVOKABLE int changeindex(int index);


    bool getHasLyric() const;
    void setHasLyric(bool newHasLyric);

    int getCurrentplayindex() const;
    void setCurrentplayindex(int newcurrentplayindex);

    void fromsongtoid(Song song);
    QList<SongLyric> getcorrectlyric(QList<SongLyricEntity> _result);
    void changecurrentindexauto();
    void changetimestate();
    void resettime();

signals:
    void loadSongLyricSuccess();
    void loadSongLyricFailed(QString message);

    void hasLyricChanged();

    void currentplayindexChanged();

private:
    QList<SongLyric> model;
    bool hasLyric = false;

    int currentplayindex = -1;
    QTimer time;
    int nowtime = 0;
    bool timesate = false;
    bool nowlryicok =false;

    int test = 0;

    Q_PROPERTY(bool hasLyric READ getHasLyric WRITE setHasLyric NOTIFY hasLyricChanged FINAL)
    Q_PROPERTY(int currentplayindex READ getCurrentplayindex WRITE setCurrentplayindex NOTIFY currentplayindexChanged FINAL)

};

#endif // SONGLYRICVIEWMODEL_H
