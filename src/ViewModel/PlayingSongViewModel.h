#ifndef PLAYINGSONGVIEWMODEL_H
#define PLAYINGSONGVIEWMODEL_H

#include <QAbstractListModel>
#include <QtMultimedia>
#include <QtQml/QQmlEngine>
#include <Utility/NeteaseCloudMusic>

class PlayingSongViewModel : public QObject {
    Q_OBJECT
    QML_NAMED_ELEMENT(PlayingSongViewModel)
    QML_SINGLETON

public:
    explicit PlayingSongViewModel(QObject* parent = nullptr);
    static PlayingSongViewModel* create(QQmlEngine*, QJSEngine*);

    Q_INVOKABLE void play();
    Q_INVOKABLE void pause();

    Q_INVOKABLE void loadSongUrl();

signals:
private:
};

#endif // PLAYINGSONGVIEWMODEL_H
