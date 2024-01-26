#include "RandomUtils.h"
#include <QString>
#include <random>
static std::random_device randomEngine;
QString generateRandomString(qsizetype length, QString characters) {
    std::uniform_int_distribution<int> distribution(0, characters.size() - 1);
    QString data;
    data.reserve(length);
    for (qsizetype i = 0; i < length; ++i)
        data.append(characters[distribution(randomEngine)]);
    return data;
}
QString generateRandomHexString(qsizetype length) {
    return generateRandomString(length, QStringLiteral("0123456789abcdef"));
}
QString generateRandomBase62String(qsizetype length) {
    return generateRandomString(length,
                                QStringLiteral("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"));
}