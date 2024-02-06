#pragma once
#include <QStringView>
namespace NeteaseCloudMusic {
namespace MusicLevel {
// 标准
constexpr QStringView Standard{u"standard"};
// 较高
constexpr QStringView Higher{u"higher"};
// 极高
constexpr QStringView ExHigh{u"exhigh"};
// 无损
constexpr QStringView Lossless{u"lossless"};
// 高解析无损
constexpr QStringView HiRes{u"hires"};
// 高清臻音
constexpr QStringView SpatialAudio{u"jyeffect"};
// 沉浸环绕声
constexpr QStringView SurroundAudio{u"c51"};
// 超清母带
constexpr QStringView Master{u"jymaster"};
} // namespace MusicLevel
}; // namespace NeteaseCloudMusic