#ifndef USERVIEWMODEL_H
#define USERVIEWMODEL_H

#include <Model/UserProfile.h>
#include <QObject>
#include <QtQml/QQmlEngine>

class UserProfileViewModel : public QObject {
    Q_OBJECT
    QML_NAMED_ELEMENT(UserProfileViewModel)
    QML_SINGLETON

public:
    explicit UserProfileViewModel(QObject* parent = nullptr);
    static UserProfileViewModel* create(QQmlEngine*, QJSEngine*);

    Q_INVOKABLE void loadUserProfile();

    bool getIsLogin() const;
    void setIsLogin(bool newIsLogin);

    UserId getUserId() const;

    QString getNickname() const;

    QString getAvatarUrl() const;

    bool getDefaultAvatar() const;

    void setUserProfileModel(UserProfile model);

signals:
    void isLoginChanged();

    void userIdChanged();

    void nicknameChanged();

    void avatarUrlChanged();

    void defaultAvatarChanged();

    void loadUserProfileFailed(QString message);
    void loadUserProfileSuccess();

private:
    bool isLogin = false;
    UserProfile userProfileModel;

    Q_PROPERTY(bool isLogin READ getIsLogin WRITE setIsLogin NOTIFY isLoginChanged FINAL)
    Q_PROPERTY(UserId userId READ getUserId NOTIFY userIdChanged FINAL)
    Q_PROPERTY(QString nickname READ getNickname NOTIFY nicknameChanged FINAL)
    Q_PROPERTY(QString avatarUrl READ getAvatarUrl NOTIFY avatarUrlChanged FINAL)
    Q_PROPERTY(bool defaultAvatar READ getDefaultAvatar NOTIFY defaultAvatarChanged FINAL)
};

#endif // USERVIEWMODEL_H
