#pragma once
#include <QByteArray>
#include <QString>
#include "Result.hpp"
// PKCS#7 padding
Result<QByteArray> aesCbcEncrypt(const QByteArray& data, const QByteArray& key, const QByteArray& iv);
// PKCS#7 padding
Result<QByteArray> aesEcbEncrypt(const QByteArray& data, const QByteArray& key);
// PKCS#7 padding
Result<QByteArray> aesEcbDecrypt(const QByteArray& data, const QByteArray& key);
// Raw RSA encryption
Result<QByteArray> encryptWithPublicKey(const QString& publicKeyPem, const QByteArray& data);