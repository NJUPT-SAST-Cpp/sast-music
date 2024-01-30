#include "UserProfileViewModel.h"
#include "Model/UserProfile.h"
#include "Service/NeteaseCloudMusic/Response/LoginStatusEntity.h"
#include <Service/NeteaseCloudMusic/CloudMusicClient.h>
#include <Utility/SettingsUtils.h>
#include <qtypes.h>

using namespace NeteaseCloudMusic;

UserProfileViewModel::UserProfileViewModel(QObject* parent) : QObject(parent) {
    isLogin = !SettingsUtils::getInstance()->value("Cookies").isNull();
}

UserProfileViewModel* UserProfileViewModel::create(QQmlEngine*, QJSEngine*) {
    return new UserProfileViewModel();
}

void UserProfileViewModel::loadUserProfile() {
    auto settingsUtils = SettingsUtils::getInstance();
    if (isLogin) {
        auto userId = settingsUtils->value("userId").toUInt();
        auto nickName = settingsUtils->value("nickName").toString();
        auto avatarUrl = settingsUtils->value("avatarUrl").toString();
        auto defaultAvatar = settingsUtils->value("defaultAvatar").toBool();
        setIsLogin(true);
        setUserProfileModel(UserProfile{userId, nickName, avatarUrl, defaultAvatar});
        emit loadUserProfileSuccess();
        return;
    }
    CloudMusicClient::getInstance()->getLoginStatus([=](Result<LoginStatusEntity> result) {
        if (result.isErr()) {
            emit loadUserProfileFailed(result.unwrapErr().message);
            return;
        }
        auto entity = result.unwrap();
        if (!entity.account.has_value() || entity.account.value().anonimousUser) {
            setIsLogin(false);
        } else {
            setIsLogin(true);
        }
        setUserProfileModel(UserProfile(entity));
        emit loadUserProfileSuccess();
        settingsUtils->setValue("userId", (quint16)userProfileModel.userId);
        settingsUtils->setValue("nickName", userProfileModel.nickname);
        settingsUtils->setValue("avatarUrl", userProfileModel.avatarUrl);
        settingsUtils->setValue("defaultAvatar", userProfileModel.defaultAvatar);
    });
}

bool UserProfileViewModel::getIsLogin() const {
    return isLogin;
}

void UserProfileViewModel::setIsLogin(bool newIsLogin) {
    if (isLogin == newIsLogin)
        return;
    isLogin = newIsLogin;
    emit isLoginChanged();
}

UserId UserProfileViewModel::getUserId() const {
    return userProfileModel.userId;
}

QString UserProfileViewModel::getNickname() const {
    return userProfileModel.nickname;
}

QString UserProfileViewModel::getAvatarUrl() const {
    return userProfileModel.avatarUrl;
}

bool UserProfileViewModel::getDefaultAvatar() const {
    return userProfileModel.defaultAvatar;
}

void UserProfileViewModel::setUserProfileModel(const UserProfile& model) {
    userProfileModel = std::move(model);
    emit userIdChanged();
    emit nicknameChanged();
    emit avatarUrlChanged();
    emit defaultAvatarChanged();
}
