#ifndef TOOLS_H
#define TOOLS_H

#include <QString>

struct Tools
{
    static QString milsec2Time(const quint64& milisec);
};

#endif // TOOLS_H
