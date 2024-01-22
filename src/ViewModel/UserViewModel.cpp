#include "UserViewModel.h"

UserViewModel::UserViewModel(QObject* parent) : QObject{parent} {}

bool UserViewModel::getIsLogin() const {
    return isLogin;
}

void UserViewModel::setIsLogin(bool newIsLogin) {
    if (isLogin == newIsLogin)
        return;
    isLogin = newIsLogin;
    emit isLoginChanged();
}
