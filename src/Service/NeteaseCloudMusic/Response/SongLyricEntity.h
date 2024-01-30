#pragma once
#include "VersionedLyricEntity.h"
namespace NeteaseCloudMusic {
struct SongLyricEntity {
    // Trivial, json field: lrc
    VersionedLyricEntity trivial;
    // Translation, json field: tlyric
    VersionedLyricEntity translation;
    // for Karaoke, json field: klyric
    VersionedLyricEntity karaoke;
    // Romaji of Japanese, json field: romalrc
    VersionedLyricEntity Romaji;
};
} // namespace NeteaseCloudMusic