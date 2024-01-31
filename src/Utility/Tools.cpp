#include "Tools.h"

QString Tools::milsec2Time(quint64 milisec) {
    int seconds = milisec / 1000;
    int minutes = seconds / 60;
    seconds %= 60;
    return QString("%1:%2").arg(minutes).arg(seconds, 2, 10, QChar('0'));
}
