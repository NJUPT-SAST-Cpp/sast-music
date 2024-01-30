#ifndef USERPROFILE_H
#define USERPROFILE_H

#include <Service/NeteaseCloudMusic/Response/LoginStatusEntity.h>
#include <Utility/NeteaseCloudMusic>
#include <initializer_list>

struct UserProfile {
    UserProfile() = default;
    UserProfile(UserId userId, QString nickname, QString avatarUrl, bool defaultAvatar)
        : userId(userId), nickname(std::move(nickname)), avatarUrl(std::move(avatarUrl)), defaultAvatar(defaultAvatar) {
    }
    UserProfile(const LoginStatusEntity& entity) {
        auto account = entity.account.value_or(AccountInfoEntity{true, 0});
        auto profile = entity.profile.value_or(ProfileInfoEntity{0, "guest", "", "", true});
        userId = profile.userId;
        nickname = profile.nickname;
        if (!profile.defaultAvatar && !account.anonimousUser)
            avatarUrl = profile.avatarUrl;
    }

    UserId userId;
    QString nickname = "guest";
    QString avatarUrl = "qrc:///res/img/avatar.svg";
    bool defaultAvatar;
};

#endif // USERPROFILE_H
