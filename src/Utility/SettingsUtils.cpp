#include "SettingsUtils.h"

SettingsUtils* SettingsUtils::getInstance() {
    static SettingsUtils instance;
    return &instance;
}

SettingsUtils::SettingsUtils() {}

void SettingsUtils::setValue(QString key, QVariant value) {
    settings.setValue(key, value);
}

QVariant SettingsUtils::value(QString key, QVariant defaultValue) {
    return settings.value(key, defaultValue);
}

void SettingsUtils::remove(QString key) {
    settings.remove(key);
}

void SettingsUtils::clear() {
    settings.clear();
}

void SettingsUtils::sync() {
    settings.sync();
}
