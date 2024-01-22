#ifndef USERVIEWMODEL_H
#define USERVIEWMODEL_H

#include <QObject>
#include <QtQml/QQmlEngine>

class UserViewModel : public QObject {
    Q_OBJECT
    QML_NAMED_ELEMENT(UserViewModel)
    QML_SINGLETON
    Q_PROPERTY(bool isLogin READ getIsLogin WRITE setIsLogin NOTIFY isLoginChanged FINAL)
public:
    explicit UserViewModel(QObject* parent = nullptr);

    bool getIsLogin() const;
    void setIsLogin(bool newIsLogin);

signals:

    void isLoginChanged();

private:
    bool isLogin = false;
};

#endif // USERVIEWMODEL_H
