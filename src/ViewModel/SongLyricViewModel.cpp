#include "SongLyricViewModel.h"
#include <Service/NeteaseCloudMusic/CloudMusicClient.h>
#include <Service/NeteaseCloudMusic/Response/SongLyricEntity.h>
#include <Utility/Result.hpp>

SongLyricViewModel::SongLyricViewModel(QObject* parent) : QAbstractListModel(parent) {}

SongLyricViewModel* SongLyricViewModel::create(QQmlEngine*, QJSEngine*) {
    return new SongLyricViewModel();
}

int SongLyricViewModel::rowCount(const QModelIndex& parent) const {
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    return model.count();
}

QVariant SongLyricViewModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid())
        return QVariant();

    // FIXME: Implement me! (initially completed)
    auto element = model[index.row()];
    switch (role) {
    case Role::Lyric:
        return element.lyric;
    case Role::TrLyric:
        return element.trLyric;
    case Role::TimeStamp:
        return element.timeStamp;
    }

    return QVariant();
}

QHash<int, QByteArray> SongLyricViewModel::roleNames() const {
    static QHash<int, QByteArray> roles;

    // FIXME: Implement me! (initially completed)
    if (roles.isEmpty()) {
        roles[Role::Lyric] = "lyric";
        roles[Role::TrLyric] = "trLyric";
        roles[Role::TimeStamp] = "timeStamp";
    }

    return roles;
}

void SongLyricViewModel::loadSongLyric(SongId songId) {
    // FIXME: Implement me! (initially completed)
    CloudMusicClient::getInstance()->getSongLyric(songId, [this](Result<SongLyricEntity> result) {
        if (result.isErr()) {
            emit loadSongLyricFailed();
            return;
        }
        auto entity = result.unwrap();
        this->hasLyric = !entity.pureMusic;
        if (this->hasLyric) {
            if (entity.trivial.has_value()) {
                this->model = std::move(parseSongLyricEntity(entity.trivial->lyric));
                if (model.isEmpty())
                    this->hasLyric = false;
            } else {
                this->hasLyric = false;
            }
        }
        emit loadSongLyricSuccess();
    });
}

bool SongLyricViewModel::getHasLyric() const {
    return hasLyric;
}

void SongLyricViewModel::setHasLyric(bool newHasLyric) {
    if (hasLyric == newHasLyric)
        return;
    hasLyric = newHasLyric;
    emit hasLyricChanged();
}

// task5 function definition
QList<SongLyric> SongLyricViewModel::parseSongLyricEntity(const QString& rawSongLyricData) {
    // FIXME: Implement me! (partially completed)

    QList<SongLyric> SongLyricList;
    qint64 first_index = 0;

    // get rid of jsons of the header
    auto strSize = rawSongLyricData.size();
    for (; first_index < strSize; first_index++) {
        // check [00:00.00] format
        if (rawSongLyricData[first_index] == '[' && strSize - first_index >= 10) {
            auto i = first_index;
            if (rawSongLyricData[i + 1].isDigit() && rawSongLyricData[i + 2].isDigit()
                && rawSongLyricData[i + 3] == ':' && rawSongLyricData[i + 6] == '.') {
                break;
            }
        }
    }

    auto getLyricTimeStamp = [](const QString& timeStampStr) -> quint64{
        return 0;
    };

    qDebug() << "Begin Sliced!" << "first_index = " << first_index;
    auto&& SongLyricData = rawSongLyricData.mid(first_index, rawSongLyricData.size() - first_index);
    qDebug() << "End Sliced!";

    if (!SongLyricData.isEmpty()) {
        qDebug() << "This is indeed not a pure music!";
    }

    qDebug() << "[rawSongLyricData Begin]";
    qDebug() << SongLyricData;
    qDebug() << "[rawSongLyricData End]";

    return SongLyricList;
}
