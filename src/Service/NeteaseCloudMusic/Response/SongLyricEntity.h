#pragma once
#include "VersionedLyricEntity.h"
namespace NeteaseCloudMusic {
struct SongLyricEntity {
    // Trivial, json field: lrc
    std::optional<VersionedLyricEntity> trivial;
    // Translation, json field: tlyric
    std::optional<VersionedLyricEntity> translation;
    // for Karaoke, json field: klyric
    std::optional<VersionedLyricEntity> karaoke;
    // Romaji of Japanese, json field: romalrc
    std::optional<VersionedLyricEntity> Romaji;
    bool pureMusic;
};
} // namespace NeteaseCloudMusic