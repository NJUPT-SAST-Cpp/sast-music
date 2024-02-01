#pragma once
#include <QSettings>

class SettingsUtils {
public:
    static SettingsUtils* getInstance();

    void setValue(QString key, QVariant value);
    QVariant value(QString key, QVariant defaultValue = QVariant());
    void remove(QString key);
    void clear();
    void sync();

private:
    SettingsUtils();

    QSettings settings;
};