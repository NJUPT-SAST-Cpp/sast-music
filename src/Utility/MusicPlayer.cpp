#include "MusicPlayer.h"
#include "ViewModel/OutputDeviceViewModel.h"
#include "ViewModel/VolumeViewModel.h"
#include <QAudioOutput>

MusicPlayer::MusicPlayer(QObject* parent) : QMediaPlayer(parent), audioOutput(new QAudioOutput) {
    // TODO: connect signals and slots
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
        oldUrl = url;
    };
    QMediaPlayer::play();
}

void MusicPlayer::onAudioOutputDeviceChanged() {
    audioOutput->setDevice(OutputDeviceViewModel::getInstance()->device());
}

void MusicPlayer::onVolumeChanged() {
    audioOutput->setVolume(VolumeViewModel::getInstance()->volume());
}