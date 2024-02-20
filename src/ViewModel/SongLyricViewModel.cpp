#include "SongLyricViewModel.h"
#include <Service/NeteaseCloudMusic/CloudMusicClient.h>
#include <Service/NeteaseCloudMusic/MusicLevel.h>
#include <Utility/NeteaseCloudMusic>

SongLyricViewModel::SongLyricViewModel(QObject* parent) : QAbstractListModel(parent) {}

SongLyricViewModel* SongLyricViewModel::create(QQmlEngine*, QJSEngine*) {
    return new SongLyricViewModel();
}

int SongLyricViewModel::rowCount(const QModelIndex& parent) const {
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    return model.count();
}

QVariant SongLyricViewModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid())
        return QVariant();
    // FIXME: Implement me!
    auto& element = model[index.row()];
    switch (role) {
    case Role::Lyric:
        return element.lyric;
    case Role::TrLyric:
        return element.trLyric;
    case Role::TimeStamp:
        return (quint64)element.timeStamp;
    }
    return QVariant();
}

QHash<int, QByteArray> SongLyricViewModel::roleNames() const {
    static QHash<int, QByteArray> roles;
    // FIXME: Implement me!
    if (roles.isEmpty()){
        roles[Role::Lyric] = "lyric";
        roles[Role::TrLyric] = "trLyric";
        roles[Role::TimeStamp] = "timeStamp";
    }
    return roles;
}
quint64 SongLyricViewModel::convertoTimestamp(const QString timeString){
    //00:00.00
    int mm=timeString.left(2).toInt();
    int ss=timeString.mid(3,2).toInt();
    int ms=timeString.right(2).toInt();
    quint64 totalmsStamp=mm*60*1000+ss*1000+ms;
    return totalmsStamp;
}


QList<SongLyric> SongLyricViewModel::getLyricList(QString strJson){

    QList<SongLyric> tmp;
    QStringList parts=strJson.split('\n');
    for(const QString&part:parts){
        QString left=part.left(10);//[00:00.00 ]
        QString right=part.right(part.length()-10);
        QString timeStr=left.mid(1,8);
        QString lyric=right;
        quint64 timeStamp=convertoTimestamp(timeStr);
        tmp.push_back({timeStamp,lyric,""});
    }
    return tmp;
}
void SongLyricViewModel::loadSongLyric(SongId songId) {

    // FIXME: Implement me!
    CloudMusicClient::getInstance()->getSongLyric(songId, [this](Result<SongLyricEntity> result){
    if (result.isErr()) {
        emit loadSongLyricFailed();
        return;
    }
    auto entire=result.unwrap();
    if(entire.pureMusic)
    {
        setHasLyric(!entire.pureMusic);
    }
    else{
        QString jsontrl=entire.trivial->lyric;
        beginResetModel();
        model=getLyricList(jsontrl);
        endResetModel();
        setHasLyric(!entire.pureMusic);
    }
    emit loadSongLyricSuccess();
    });
}

bool SongLyricViewModel::getHasLyric() const {
    return hasLyric;
}

void SongLyricViewModel::setHasLyric(bool newHasLyric) {
    if (hasLyric == newHasLyric)
        return;
    hasLyric = newHasLyric;
    emit hasLyricChanged();
}
