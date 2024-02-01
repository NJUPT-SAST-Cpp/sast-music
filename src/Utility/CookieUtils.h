#pragma once
#include <QByteArray>
#include <QList>
#include <QNetworkCookie>
#include <QString>
QString getCookieOrDefault(const QList<QNetworkCookie>& cookies, const QString& name, const QString& defaultValue);
bool containsCookie(const QList<QNetworkCookie>& cookies, const QString& name);
