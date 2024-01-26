#pragma once
#include <QByteArray>
#include <QString>
#include "Result.hpp"
Result<QByteArray> aesCbcEncrypt(const QByteArray& data, const QByteArray& key, const QByteArray& iv);
Result<QByteArray> encryptWithPublicKey(const QString& publicKeyPem, const QByteArray& data);