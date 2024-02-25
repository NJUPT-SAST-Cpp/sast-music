#include "SongLyricViewModel.h"
// #include "NextUpViewModel.h"
// #include "PlayingSongViewModel.h"
#include <Service/NeteaseCloudMusic/CloudMusicClient.h>
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
    auto element = model[index.row()];
    switch(role){
    case Role::Lyric:
        return element.lyric;
    case Role::TrLyric:
        return element.trLyric;
    case Role::TimeStamp:
        return element.timeStamp;
    }
      // FIXME: Implement me!
    return QVariant();
}

QHash<int, QByteArray> SongLyricViewModel::roleNames() const {
    static QHash<int, QByteArray> roles;
    // FIXME: Implement me!
    if(roles.isEmpty()){
        roles[Role::Lyric]="lyric";
        roles[Role::TrLyric]="trlyric";
        roles[Role::TimeStamp]="timestamp";
    }
    return roles;
}
QList<SongLyric> SongLyricViewModel::getlyric(QList<SongLyricEntity> _result){
    QList<SongLyric> lyricList;

           // 假设_result列表中只有一个元素包含所有歌词信息。
    auto songLyricEntity = _result.first();

    if(songLyricEntity.pureMusic) {
        SongLyric lyricItem;
        lyricItem.lyric = "这是一首纯音乐，请尽情享受。";
        lyricItem.timeStamp = 0;
        lyricList.append(lyricItem);
        return lyricList;
    }

    QString mainLyric = songLyricEntity.trivial.value().lyric; // 假设主要歌词存储在这里
    QString translatedLyric = songLyricEntity.translation.value().lyric; // 翻译歌词
    parseLyrics(mainLyric, lyricList); // 处理主要歌词
    appendTranslation(translatedLyric, lyricList); // 添加翻译到已解析的主要歌词中
    return lyricList;
}

void SongLyricViewModel::parseLyrics(const QString& lyrics, QList<SongLyric>& list) {
    int beginIndex = 0, endIndex = 0;

    while ((beginIndex = lyrics.indexOf('[', endIndex)) != -1) {
        endIndex = lyrics.indexOf(']', beginIndex);
        if (endIndex == -1) break;

        int timeStampEnd = lyrics.indexOf(']', beginIndex + 1);
        QString timeString = lyrics.mid(beginIndex + 1, timeStampEnd - beginIndex - 1);

        SongLyric lyricItem;
        lyricItem.timeStamp = getTimeStampFromString(timeString);
        lyricItem.lyric = lyrics.mid(endIndex + 1, lyrics.indexOf('\n', endIndex) - endIndex - 1).trimmed();

        if (!lyricItem.lyric.isEmpty()) {
            list.append(lyricItem);
        }

        endIndex = lyrics.indexOf('\n', endIndex) + 1;
    }
}
quint64 SongLyricViewModel::getTimeStampFromString(const QString& str) {
    // 使用QRegExp或者QString的split方法来分割分钟和秒
    QRegularExpression regex("(\\d+):(\\d+).(\\d+)"); // 正则表达式用于匹配 "分钟:秒.毫秒" 的格式
    if (regex.isValid()) {
        quint64 minutes = regex.captureCount(); // 第一个捕获组对应分钟
        quint64 seconds = regex.captureCount(); // 第二个捕获组对应秒
        quint64 miliseconds = regex.captureCount();//第三个捕获毫秒
        return 1000*(minutes * 60 + seconds)+miliseconds*10; // 将时间转换为毫秒
    }
    return 0; // 如果不符合预期格式，返回0
}

void SongLyricViewModel::appendTranslation(const QString& translation, QList<SongLyric>& list) {
    int beginIndex = 0, endIndex = 0;

    while ((beginIndex = translation.indexOf('[', endIndex)) != -1) {
        endIndex = translation.indexOf(']', beginIndex);
        if (endIndex == -1) break;

        int timeStampEnd = translation.indexOf(']', beginIndex + 1);
        QString timeString = translation.mid(beginIndex + 1, timeStampEnd - beginIndex - 1);
        int timeStamp = getTimeStampFromString(timeString);

        QString trLyric = translation.mid(endIndex + 1, translation.indexOf('\n', endIndex) - endIndex - 1).trimmed();

        for (auto& item : list) {
            if (item.timeStamp == timeStamp && !trLyric.isEmpty()) {
                item.trLyric = trLyric;
                break;
            }
        }

        endIndex = translation.indexOf('\n', endIndex) + 1;
    }
}
void SongLyricViewModel::loadSongLyric(SongId songId) {
    // FIXME: Implement me!
    CloudMusicClient::getInstance()->getPlaylistDetail(songId,[this](Result<PlaylistDetailEntity> result) {
        if (result.isErr()) {
            qDebug()<<result.unwrapErr().message;
            emit loadSongLyricFailed();
            return;
        }
        auto lyrics = result.unwrap();
        //here
        for (int i=0;i<114514;i++) {
            // 创建一个SongLyric对象，并将歌词数据填充进去
            SongLyric item;
            item.lyric = model[i].lyric;
            item.timeStamp = model[i].timeStamp;
            item.trLyric = model[i].trLyric;

                   // 将歌词项添加到model中
            model.emplace_back(item);
        }
        beginResetModel();
        model.clear();
        endResetModel();
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
