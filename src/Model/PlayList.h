#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <Utility/NeteaseCloudMusic>
#include <Service/NeteaseCloudMusic/Response/PlaylistEntity.h>

struct PlayList {
    PlayList() = default;
    PlayList(const PlaylistEntity& entity) : id(entity.id), name(entity.name),
        coverImgUrl(entity.coverImgUrl), creatorName(entity.creator.nickname),
        updateTime(entity.updateTime.toString("MMMM d, yyyy")), description(entity.description){}

    PlaylistId id;
    QString name;
    QString coverImgUrl;
    QString creatorName;
    QString updateTime;
    QString description;
};

#endif // PLAYLIST_H
