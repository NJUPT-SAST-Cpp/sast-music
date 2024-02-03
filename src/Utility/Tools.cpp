#include "Tools.h"
#include <QRandomGenerator>

QString Tools::milsec2Time(quint64 milsec) {
    int seconds = milsec / 1000;
    int minutes = seconds / 60;
    seconds %= 60;
    return QString("%1:%2").arg(minutes).arg(seconds, 2, 10, QChar('0'));
}

int Tools::randomInt(int low, int high) {
    QRandomGenerator randomGenerator;
    return randomGenerator.bounded(low, high);
}
