#pragma once
#include <QObject>
#include <QTimer>
#include <QtQml/QQmlEngine>
#include <Service/NeteaseCloudMusic/Response/LoginQRCodePollingEntity.h>

class LoginViewModel : public QObject {
    Q_OBJECT
    QML_NAMED_ELEMENT(LoginViewModel)
    QML_SINGLETON
private:
    QString m_qrCodeData;
    QString m_key;
    QTimer m_timer;
    NeteaseCloudMusic::LoginQRCodePollingStatus m_status = NeteaseCloudMusic::LoginQRCodePollingStatus::WaitingForScan;

private slots:
    void loginQRCodePolling();

public:
    explicit LoginViewModel(QObject* parent = nullptr);
    Q_INVOKABLE void newLoginQRCode();
    Q_INVOKABLE void logout();
    Q_PROPERTY(QString qrCodeData READ getQRCodeData NOTIFY onQRCodeDataChanged FINAL)
    Q_PROPERTY(NeteaseCloudMusic::LoginQRCodePollingStatus status READ getStatus NOTIFY onStatusChanged FINAL)
    QString getQRCodeData() const;
    NeteaseCloudMusic::LoginQRCodePollingStatus getStatus() const;

signals:
    void loginQRCodeNewFailed(QString message);
    void loginQRCodePollingFailed(QString message);
    void onQRCodeDataChanged();
    void onStatusChanged();
    void logoutSuccess();
    void logoutFailed(QString message);
};
