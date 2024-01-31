#include "OutputDeviceViewModel.h"

OutputDeviceViewModel::OutputDeviceViewModel(QObject* parent) : QAbstractListModel(parent) {}

qsizetype OutputDeviceViewModel::getCurrentIndex() const {
    return currentIndex;
}

void OutputDeviceViewModel::setCurrentIndex(qsizetype newCurrentIndex) {
    if (currentIndex == newCurrentIndex)
        return;
    flag = true;
    currentIndex = newCurrentIndex;
    emit currentIndexChanged();
}

QString OutputDeviceViewModel::getDefaultDeviceName() const {
    if (!flag)
        return QMediaDevices::defaultAudioOutput().description();
    return devices[currentIndex].description();
}

void OutputDeviceViewModel::loadAudioOutputDevices() {
    devices = QMediaDevices::audioOutputs();
    emit dataChanged(index(0), index(devices.count()));
}

QAudioDevice OutputDeviceViewModel::device() {
    auto device = devices[currentIndex];
    return device;
}

OutputDeviceViewModel* OutputDeviceViewModel::getInstance() {
    static OutputDeviceViewModel instance;
    return &instance;
}

OutputDeviceViewModel* OutputDeviceViewModel::create(QQmlEngine*, QJSEngine*) {
    auto instance = getInstance();
    QJSEngine::setObjectOwnership(instance, QQmlEngine::CppOwnership);
    return instance;
}

int OutputDeviceViewModel::rowCount(const QModelIndex& parent) const {
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    return devices.count();
}

QVariant OutputDeviceViewModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid())
        return QVariant();
    auto element = devices[index.row()];
    switch (role) {
    case DeviceName:
        return element.description();
    }
    return QVariant();
}

QHash<int, QByteArray> OutputDeviceViewModel::roleNames() const {
    static QHash<int, QByteArray> roles;
    if (roles.empty()) {
        roles[DeviceName] = "name";
    }
    return roles;
}
