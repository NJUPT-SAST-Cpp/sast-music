#include <Service/NeteaseCloudMusic/Response/LoginStatusEntity.h>
#include <Utility/Def.h>

struct UserProfile {
    UserProfile() = default;
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