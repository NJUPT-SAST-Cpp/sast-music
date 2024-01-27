#ifndef OUTPUTDEVICEVIEWMODEL_H
#define OUTPUTDEVICEVIEWMODEL_H

#include <QAbstractListModel>
#include <QAudioDevice>
#include <QMediaDevices>
#include <QtQml>

class OutputDeviceViewModel : public QAbstractListModel {
    Q_OBJECT
    QML_NAMED_ELEMENT(OutputDeviceViewModel)
    QML_SINGLETON
    Q_PROPERTY(qsizetype currentIndex READ getCurrentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged FINAL)

public:
    static OutputDeviceViewModel* getInstance();
    static OutputDeviceViewModel* create(QQmlEngine*, QJSEngine*);

    enum Role {
        DeviceName = Qt::UserRole,
    };

    // Basic functionality:
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE QString getDefaultDeviceName() const;

    Q_INVOKABLE void loadAudioOutputDevices();

    qsizetype getCurrentIndex() const;
    void setCurrentIndex(qsizetype newCurrentIndex);

signals:
    void currentIndexChanged();

private:
    explicit OutputDeviceViewModel(QObject* parent = nullptr);

    QList<QAudioDevice> devices = QMediaDevices::audioOutputs();

    inline static bool flag = false;

    qsizetype currentIndex;
};

#endif // OUTPUTDEVICEVIEWMODEL_H
