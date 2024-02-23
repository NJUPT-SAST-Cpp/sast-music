#include "SongLyricViewModel.h"
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

    // FIXME: Implement me!
    return QVariant();
}

QHash<int, QByteArray> SongLyricViewModel::roleNames() const {
    static QHash<int, QByteArray> roles;
    // FIXME: Implement me!
    return roles;
}

void SongLyricViewModel::loadSongLyric(SongId songId) {
    CloudMusicClient::getInstance()->getSongLyric(songId, [this](Result<SongLyricEntity> result) {
        if (result.isErr()) {
            qDebug() << "歌词加载失败";
            emit loadSongLyricFailed();
            return;
        }

        const auto& lyricEntity = result.unwrap();//前面有一层安全检测,到这里应该是安全的了,可以全部解掉

               // 然后接下来处理来自不同option的歌词
        QList<SongLyric> allLyrics;
        if (lyricEntity.trivial.has_value()) {
            const VersionedLyricEntity& trivialLyric = lyricEntity.trivial.value();
            allLyrics.append(SongLyricsp(trivialLyric.lyric)); // SongLyricsp接受QString
        }
        if (lyricEntity.translation.has_value()) {
            const VersionedLyricEntity& translationLyric = lyricEntity.translation.value();
            allLyrics.append(SongLyricsp(translationLyric.lyric));
        }
        if (lyricEntity.karaoke.has_value()) {
            const VersionedLyricEntity& karaokeLyric = lyricEntity.karaoke.value();
            allLyrics.append(SongLyricsp(karaokeLyric.lyric));
        }
        if (lyricEntity.Romaji.has_value()) {
            const VersionedLyricEntity& romajiLyric = lyricEntity.Romaji.value();
            allLyrics.append(SongLyricsp(romajiLyric.lyric));
        }


        if (lyricEntity.pureMusic) {

            qDebug() << "这是一首纯音乐.";
            emit loadSongLyricSuccess();
        } else {

            emit loadSongLyricSuccess();
        }
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
QList<SongLyric> SongLyricViewModel::SongLyricsp(const QString& stringLyric) {
    QList<SongLyric> splitLyric;
    QString currentLyric;
    QString timeStampText;

           // 字符串分割
    QStringList lines = stringLyric.split("\n");

    for (const QString& line : lines) {
        // 去除行首尾的空字符
        QString trimmedLine = line.trimmed();

               // 如果行是空的，则跳过
        if (trimmedLine.isEmpty()) {
            continue;
        }

               // 将行分割为时间戳和歌词文本
        int separatorIndex = trimmedLine.indexOf(']');
        if (separatorIndex == -1) {
            qDebug() << "Invalid lyric line:" << trimmedLine;
            continue;
        }

        timeStampText = trimmedLine.mid(1, separatorIndex - 1); // 提取时间戳，去掉开头的'['和结尾的']'
        currentLyric = trimmedLine.mid(separatorIndex + 1).trimmed(); // 提取歌词文本，并去掉前后的空白字符

               // 解析时间戳为毫秒
        qint64 minutes = timeStampText.left(2).toInt();
        qint64 seconds = timeStampText.mid(3, 2).toInt();
        qint64 milliseconds = timeStampText.mid(6, 3).toInt();
        qint64 timeStamp = minutes * 60 * 1000 + seconds * 1000 + milliseconds;

               // 创建SongLyric对象并添加到列表中
        SongLyric lyric;
        lyric.timeStamp = timeStamp;
        lyric.lyric = currentLyric;
        splitLyric.append(lyric);
    }

    return splitLyric;
}
