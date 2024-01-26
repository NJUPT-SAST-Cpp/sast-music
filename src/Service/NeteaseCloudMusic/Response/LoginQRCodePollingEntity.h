#pragma once
namespace NeteaseCloudMusic {
enum class LoginQRCodePollingStatus {
    WaitingForScan,
    WaitingForConfirm,
    Success,
    Timeout,
    Unknown,
};
struct LoginQRCodePollingEntity {
    LoginQRCodePollingStatus status;
};
}