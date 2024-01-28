#include "CookieUtils.h"
QString getCookieOrDefault(const QList<QNetworkCookie>& cookies, const QString& name,
                                  const QString& defaultValue) {
    auto cookie = std::find_if(cookies.begin(), cookies.end(),
                               [&name](const QNetworkCookie& cookie) { return cookie.name() == name; });
    if (cookie != cookies.end()) {
        return QString::fromUtf8(cookie->value());
    } else {
        return defaultValue;
    }
}

bool containsCookie(const QList<QNetworkCookie>& cookies, const QString& name) {
    return std::find_if(cookies.begin(), cookies.end(),
                        [&](const QNetworkCookie& cookie) { return cookie.name() == name; }) != cookies.end();
}
