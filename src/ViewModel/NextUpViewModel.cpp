#include "NextUpViewModel.h"

NextUpViewModel::NextUpViewModel(QObject* parent) : QAbstractListModel(parent) {}

NextUpViewModel* NextUpViewModel::getInstance() {
    static NextUpViewModel instance;
    return &instance;
}

NextUpViewModel* NextUpViewModel::create(QQmlEngine*, QJSEngine*) {
    auto instance = getInstance();
    QJSEngine::setObjectOwnership(instance, QQmlEngine::CppOwnership);
    return instance;
}

int NextUpViewModel::rowCount(const QModelIndex& parent) const {
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    // FIXME: Implement me!
    return 0;
}

QVariant NextUpViewModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid())
        return QVariant();

    // FIXME: Implement me!
    return QVariant();
}
