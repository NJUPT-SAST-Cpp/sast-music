#ifndef NEXTUPVIEWMODEL_H
#define NEXTUPVIEWMODEL_H

#include <QAbstractListModel>
#include <QtQml/QQmlEngine>
#include <Utility/NeteaseCloudMusic>

class NextUpViewModel : public QAbstractListModel {
    Q_OBJECT
    QML_NAMED_ELEMENT(NextUpViewModel)
    QML_SINGLETON

public:
    static NextUpViewModel* getInstance();
    static NextUpViewModel* create(QQmlEngine*, QJSEngine*);

    // Basic functionality:
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

private:
    explicit NextUpViewModel(QObject* parent = nullptr);
};

#endif // NEXTUPVIEWMODEL_H
