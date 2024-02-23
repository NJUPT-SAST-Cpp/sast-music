#include "MusicPlayer.h"
#include "ViewModel/OutputDeviceViewModel.h"
#include "ViewModel/VolumeViewModel.h"
#include <QAudioOutput>

MusicPlayer::MusicPlayer(QObject* parent) : QMediaPlayer(parent), audioOutput(new QAudioOutput(this)) {
    // TODO: connect signals and slots
    connect(OutputDeviceViewModel::getInstance(), &OutputDeviceViewModel::currentIndexChanged, this, &MusicPlayer::onAudioOutputDeviceChanged);
    connect(VolumeViewModel::getInstance(), &VolumeViewModel::volumeChanged, this, &MusicPlayer::onVolumeChanged);
 //分别连接信号槽,一方面当载入的设备发生变化的时候重新载入音频驱动和设备,第二方面当音量发生变化的时候就更新当前音频输出的音量大小
    audioOutput->setDevice(QMediaDevices::defaultAudioOutput());
    audioOutput->setVolume(VolumeViewModel::getInstance()->volume());
    setAudioOutput(audioOutput);
}

void MusicPlayer::play(const QUrl& url) {
    static QUrl oldUrl;
    if (oldUrl != url) {
        setSource(url);
        oldUrl = url;
        QMediaPlayer::stop();
    }
    QMediaPlayer::play();
}

void MusicPlayer::onAudioOutputDeviceChanged() {
    audioOutput->setDevice(OutputDeviceViewModel::getInstance()->device());
}

void MusicPlayer::onVolumeChanged() {
    audioOutput->setVolume(VolumeViewModel::getInstance()->volume() / 100.0);
}

