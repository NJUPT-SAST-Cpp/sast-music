#pragma once
namespace NeteaseCloudMusic {
struct SubscriptionCountEntity {
    int programCount;
    // 电台
    int djRadioCount;
    int mvCount;
    int artistCount;
    int newProgramCount;
    // 创建的电台
    int createDjRadioCount;
    // 创建的歌单
    int createdPlaylistCount;
    // 收藏的歌单
    int subPlaylistCount;
};
}; // namespace NeteaseCloudMusic