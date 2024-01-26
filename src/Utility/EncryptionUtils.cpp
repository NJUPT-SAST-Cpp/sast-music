#include "EncryptionUtils.h"
#include "Result.hpp"
#include <QByteArray>
#include <QString>
#include <mbedtls/aes.h>
#include <mbedtls/ctr_drbg.h>
#include <mbedtls/entropy.h>
#include <mbedtls/pk.h>
#include <mbedtls/platform.h>
#include <mbedtls/rsa.h>

Result<QByteArray> aesCbcEncrypt(const QByteArray& data, const QByteArray& key, const QByteArray& iv) {
    mbedtls_aes_context aes;
    mbedtls_aes_init(&aes);
    int ret;
    ret = mbedtls_aes_setkey_enc(&aes, reinterpret_cast<const uint8_t*>(key.data()), key.size() * 8);
    if (ret != 0) {
        mbedtls_aes_free(&aes);
        return ErrorInfo(ErrorKind::EncryptionError, QStringLiteral("mbedtls_aes_setkey_enc failed"));
    }
    uint8_t paddingLength = 16 - data.size() % 16;
    QByteArray dataPadded(data.size() + paddingLength, Qt::Uninitialized);
    std::copy(data.begin(), data.end(), dataPadded.begin());
    std::fill(dataPadded.end() - paddingLength, dataPadded.end(), paddingLength);
    QByteArray encryptedData(dataPadded.size(), Qt::Uninitialized);
    uint8_t curIv[16];
    std::copy(iv.begin(), iv.end(), curIv);
    ret = mbedtls_aes_crypt_cbc(&aes, MBEDTLS_AES_ENCRYPT, dataPadded.size(), curIv,
                                reinterpret_cast<const uint8_t*>(dataPadded.constData()),
                                reinterpret_cast<uint8_t*>(encryptedData.data()));
    if (ret != 0) {
        mbedtls_aes_free(&aes);
        return ErrorInfo(ErrorKind::EncryptionError, QStringLiteral("mbedtls_aes_crypt_cbc failed"));
    }
    mbedtls_aes_free(&aes);
    return encryptedData;
}

Result<QByteArray> encryptWithPublicKey(const QString& publicKeyPem, const QByteArray& data) {
    mbedtls_pk_context pk;
    mbedtls_rsa_context* rsa;
    static const char* pers = "rsa_encrypt";
    int ret;

    mbedtls_pk_init(&pk);

    auto publicKeyPemData = publicKeyPem.toUtf8();
    ret = mbedtls_pk_parse_public_key(&pk, reinterpret_cast<const uint8_t*>(publicKeyPemData.constData()),
                                      publicKeyPemData.size() + 1);
    if (ret != 0) {
        mbedtls_pk_free(&pk);
        return ErrorInfo(ErrorKind::EncryptionError, QStringLiteral("mbedtls_pk_parse_public_key failed"));
    }

    rsa = mbedtls_pk_rsa(pk);
    size_t output_len = mbedtls_pk_get_len(&pk);
    if (output_len < data.size()) {
        mbedtls_pk_free(&pk);
        return ErrorInfo(ErrorKind::EncryptionError, QStringLiteral("data is too long"));
    }
    QByteArray encryptedData(output_len, Qt::Uninitialized);
    std::copy(data.begin(), data.end(), encryptedData.end() - data.size());
    std::fill(encryptedData.begin(), encryptedData.end() - data.size(), 0);
    ret = mbedtls_rsa_public(rsa, reinterpret_cast<const uint8_t*>(encryptedData.constData()),
                             reinterpret_cast<uint8_t*>(encryptedData.data()));
    if (ret != 0) {
        mbedtls_pk_free(&pk);
        return ErrorInfo(ErrorKind::EncryptionError, QStringLiteral("mbedtls_rsa_pkcs1_encrypt failed"));
    }

    mbedtls_pk_free(&pk);
    return encryptedData;
}
