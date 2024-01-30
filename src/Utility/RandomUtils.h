#pragma once
#include <QString>
QString generateRandomString(qsizetype length, QString characters);
QString generateRandomHexString(qsizetype length);
QString generateRandomBase62String(qsizetype length);
// Generate a random integer in [min, max]
int randomInt(int min, int max);
