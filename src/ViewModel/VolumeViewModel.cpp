#include "VolumeViewModel.h"

VolumeViewModel::VolumeViewModel(QObject *parent) : QObject{parent} {}

VolumeViewModel *VolumeViewModel::getInstance()
{
    static VolumeViewModel instance;
    return &instance;
}

VolumeViewModel *VolumeViewModel::create(QQmlEngine *, QJSEngine *)
{
    auto instance = getInstance();
    QJSEngine::setObjectOwnership(instance, QQmlEngine::CppOwnership);
    return instance;
}

int VolumeViewModel::volume() const
{
    return m_volume;
}

void VolumeViewModel::setVolume(int newVolume)
{
    if (m_volume == newVolume)
        return;
    m_volume = newVolume;
    emit volumeChanged();
}
