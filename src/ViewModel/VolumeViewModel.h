#ifndef VOLUMEVIEWMODEL_H
#define VOLUMEVIEWMODEL_H

#include <QObject>
#include <QtQml/QQmlEngine>

class VolumeViewModel : public QObject {
    Q_OBJECT
    QML_SINGLETON
    QML_NAMED_ELEMENT(VolumeViewModel)
    Q_PROPERTY(int volume READ volume WRITE setVolume NOTIFY volumeChanged FINAL)

public:
    static VolumeViewModel* getInstance();
    static VolumeViewModel* create(QQmlEngine*, QJSEngine*);

    int volume() const;
    void setVolume(int newVolume);

signals:
    void volumeChanged();

private:
    explicit VolumeViewModel(QObject* parent = nullptr);

    int m_volume = 100;
};

#endif // VOLUMEVIEWMODEL_H
