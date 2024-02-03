#pragma once
#include <QString>
namespace NeteaseCloudMusic {
struct VersionedLyricEntity {
    int version = -1;
    // 除了歌词本身外，可能还有一些以 '{' 开头 '}' 结尾的 JSON，用以存储某些元数据
    QString lyric;
};
} // namespace NeteaseCloudMusic
