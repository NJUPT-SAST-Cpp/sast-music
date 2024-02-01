#include "PlayingSongViewModel.h"
#include <Service/NeteaseCloudMusic/CloudMusicClient.h>

PlayingSongViewModel::PlayingSongViewModel(QObject* parent) : QObject{parent} {}

PlayingSongViewModel* PlayingSongViewModel::create(QQmlEngine*, QJSEngine*) {
    return new PlayingSongViewModel();
}

void PlayingSongViewModel::play() {}

void PlayingSongViewModel::pause() {}

void PlayingSongViewModel::loadSongUrl() {}
