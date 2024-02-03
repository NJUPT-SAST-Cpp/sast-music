#ifndef TOOLS_H
#define TOOLS_H

#include <QString>

struct Tools {
    static QString milsec2Time(quint64 milsec);
    static int randomInt(int low, int high);
};

#endif // TOOLS_H
