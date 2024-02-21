#include "SongLyricViewModel.h"
#include "Model/SongLyric.h"
#include "PlayingSongViewModel.h"
#include <algorithm>
#include <climits>
#include <exception>
#include <iostream>
#include <qdebug.h>
#include <qsize.h>
#include <qtypes.h>
#include <regex>
#include <utility>

SongLyricViewModel::SongLyricViewModel(QObject* parent) : QAbstractListModel(parent) {
    connect(PlayingSongViewModel::getInstance(), &PlayingSongViewModel::timeStampChanged, this,
                     &SongLyricViewModel::onTimestampChanged);
    loadSongLyric(PlayingSongViewModel::getInstance()->getSongId());
}

SongLyricViewModel* SongLyricViewModel::create(QQmlEngine*, QJSEngine*) {
    return getInstance();
}

SongLyricViewModel* SongLyricViewModel::getInstance(){
    static auto instance = new SongLyricViewModel();
        return instance;
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
    auto& element = model[index.row()];
    switch (role) {
    case Role::TimeStampRole:
        return element.timeStamp;
    case Role::ContentRole:
        return element.lyric;
    case Role::TypeRole:
        return element.type;
    }
    return QVariant();
}

QHash<int, QByteArray> SongLyricViewModel::roleNames() const {
    static QHash<int, QByteArray> roles;
    if (roles.empty()) {
        roles[Role::TimeStampRole] = "timeStamp";
        roles[Role::ContentRole] = "content";
        roles[Role::TypeRole] = "type";
    }
    return roles;
}

void SongLyricViewModel::loadSongLyric(SongId songId) {
    CloudMusicClient::getInstance()->getSongLyric(songId, [this](Result<SongLyricEntity> result) {
        if (result.isErr()) {
            emit loadSongLyricFailed();
        }

        auto& [trivial, translation, karaoke, romaji, pureMusic] = result.unwrap();
        this->hasLyric = !pureMusic;

        QList<SongLyric> lyric; // include all lyric;
        for (auto& [content, type, hasType] : {std::tuple{trivial, SongLyric::LyricType::Common, &hasLyric},
                                               {translation, SongLyric::LyricType::Translation, &hasTranslation},
                                               {karaoke, SongLyric::LyricType::Karaoke, &hasKaraoke},
                                               {romaji, SongLyric::LyricType::Romaji, &hasRomaji}}) {
            if (content.has_value() && content->lyric.length() != 0) {
                *hasType = bool(content.value().version);
                try {
                    lyric += this->lyricProcess(content->lyric, type);
                } catch (std::exception e) {
                    qDebug() << e.what();
                }
            }
        }
        std::stable_sort(lyric.begin(), lyric.end(),
                         [](const SongLyric& l, const SongLyric& r) { return l.timeStamp < r.timeStamp; });
        // for (auto& i : lyric) {
        //     qDebug() << i.timeStamp << ":" << i.lyric;
        // }
        beginResetModel();
        this->model = std::move(lyric);
        endResetModel();
        emit hasLyricChanged();
        qDebug()<<"lyric updated: "<<hasLyric;
    });
}

QList<SongLyric>&& SongLyricViewModel::lyricProcess(const QString& rawDataQstring, SongLyric::LyricType type) {
    std::string rawData = std::move(rawDataQstring.toStdString());
    // qDebug() << rawData;

    // remove json
    std::regex jsonRegex(R"(\{.*\})", std::regex_constants::ECMAScript);
    while (std::regex_search(rawData, jsonRegex)) {
        rawData = std::move(std::regex_replace(rawData, jsonRegex, ""));
    }

    // remove extra metadata
    std::regex metaDataRegex(R"(\[by:.*?\])", std::regex_constants::ECMAScript);
    while (std::regex_search(rawData, metaDataRegex)) {
        rawData = std::move(std::regex_replace(rawData, metaDataRegex, ""));
    }

    // match 4 parts every lyric line: min, sec, milisec, content
    std::regex lyricSplit(R"([0-9]{2}(?=.*\])|(?=\]).*?(?=\n))", std::regex_constants::ECMAScript);
    std::sregex_iterator lyricBegin(rawData.begin(), rawData.end(), lyricSplit);
    std::sregex_iterator regexEnd;
    // std::cout << std::distance(lyricBegin, regexEnd) << std::endl;
    static QList<SongLyric> out;
    out.clear();
    for (int i = 0; lyricBegin != regexEnd; lyricBegin++, i++) {
        // std::cout<<i<<" "<<lyricBegin->str()<<std::endl;
        static size_t timestamp;
        std::string content;
        switch (i % 4) {
        case 0:
            timestamp = std::stoull(lyricBegin->str()) * 60 * 1000;
            break;
        case 1:
            timestamp += std::stoull(lyricBegin->str()) * 1000;
            break;
        case 2:
            timestamp += std::stoull(lyricBegin->str()) * 10;
            break;
        case 3:
            content = std::move(lyricBegin->str().substr(1));
            // std::cout << timestamp << ":" << content << std::endl;
            out += SongLyric{timestamp, std::move(QString::fromUtf8(content.data(), int(content.size()))), type};
            break;
        }
    }
    return std::move(out);
}

void SongLyricViewModel::onTimestampChanged() {
    this->switchLyricIndex();
}

void SongLyricViewModel::switchLyricIndex() {
    // find index
    const auto time = PlayingSongViewModel::getInstance()->getTimeStamp();
    qint64 index = 0;
    for (qsizetype i = 0; i < model.size(); i++) {
        if (model.at(i).timeStamp<time&&model.at(i).timeStamp!=model.at(index).timeStamp) {
            index = i;
        }
        if (model.at(i).timeStamp>time){
            break;
        }
    }
    currentLyricIndex = index;
    emit currentLyricIndexChanged();
}

bool SongLyricViewModel::getHasLyric() const {
    return hasLyric;
}

qint64 SongLyricViewModel::getCurrentLyricIndex() const {
    return currentLyricIndex;
}

void SongLyricViewModel::setHasLyric(bool newHasLyric) {
    if (hasLyric == newHasLyric)
        return;
    hasLyric = newHasLyric;
    emit hasLyricChanged();
}

void SongLyricViewModel::setCurrentLyricIndex(qint64 newValue){
    if (currentLyricIndex == newValue)
        return;
    currentLyricIndex = newValue;
    emit hasLyricChanged();
}
