#include "SongLyricViewModel.h"
#include "NextUpViewModel.h"
#include "PlayingSongViewModel.h"

SongLyricViewModel::SongLyricViewModel(QObject* parent) : QAbstractListModel(parent) {
    connect(NextUpViewModel::getInstance(),&NextUpViewModel::playingSongChanged,this,&SongLyricViewModel::fromsongtoid);
    connect(NextUpViewModel::getInstance(),&NextUpViewModel::playingSongChanged,this,&SongLyricViewModel::resettime);
    connect(PlayingSongViewModel::getInstance(),&PlayingSongViewModel::playingChanged,
             this,&SongLyricViewModel::changetimestate);
    connect(&time,&QTimer::timeout,this,&SongLyricViewModel::changecurrentindexauto);

    time.setInterval(500);
}

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
    auto element = model[index.row()];
    switch (role) {
    case Role::TimeStamp:
        return (quint64)element.timeStamp;
    case Role::Lyric:
        return element.lyric;
    case Role::TrLyric:
        return element.trLyric;
    }
    return QVariant();
}

QHash<int, QByteArray> SongLyricViewModel::roleNames() const {
    static QHash<int, QByteArray> roles;
    // FIXME: Implement me!
    if (roles.isEmpty()) {
        roles[Role::TimeStamp] = "TimeStamp";
        roles[Role::Lyric] = "Lyric";
        roles[Role::TrLyric] = "TrLyric";
    }
    return roles;
}

//先返回中文，然后是其他语言
QList<SongLyric> SongLyricViewModel::getcorrectlyric(QList<SongLyricEntity> _result){
    auto totallyric = _result.first();
    auto origin = totallyric.trivial.lyric;
    QString translation = totallyric.translation.lyric;
    QList<SongLyric> _return;
    int nowindexend,nowindexbegin,nowlyricbegin;
    nowindexend = 0;
    nowindexbegin = 0;
    nowlyricbegin = 0;
    if(origin[0]=='{'){//如果发现有json格式的内容，直接抛弃
        while(origin.indexOf('}',nowindexbegin+1) != -1){
            //qDebug()<<nowindexbegin;
            nowindexbegin = origin.indexOf('}',nowindexbegin+1);
        }
        nowindexbegin+=2;
    }
    //qDebug()<<nowindexbegin;
    //qDebug()<<origin.mid(nowindexbegin);
    //qDebug()<<translation;
    nowindexend = nowindexbegin;
    for(int i=0;i<=origin.size()-1;){
        nowindexbegin = origin.indexOf('[',nowindexend);
        if(nowindexbegin == -1){
            break;
        }
        nowindexend = origin.indexOf('[',nowindexbegin+1);
        nowlyricbegin = origin.indexOf(']',nowindexbegin);
        if(nowindexbegin >= 1730 ){
            int a=1;
        }
        SongLyric thislyric;
        thislyric.lyric = origin.mid(nowlyricbegin+1,nowindexend-nowlyricbegin-1);
        thislyric.timeStamp =origin.mid(nowindexbegin+1,2).toInt()*60+origin.mid(nowindexbegin+4,2).toInt();
        if(thislyric.lyric.isEmpty()) continue;
        //qDebug()<<thislyric.timeStamp<<" :"<<nowindexbegin<<" "<<(thislyric.lyric[0] == '\n');
        if(thislyric.lyric[0] == '\n' ) continue;
        _return.append(thislyric);
    }
    // if(!totallyric.Romaji.lyric.isEmpty()){
    //     otherlanguage = totallyric.Romaji.lyric;
    // }else{
    //     if(totallyric.karaoke.lyric.isEmpty()){
    //         otherlanguage = totallyric.karaoke.lyric;
    //     }else{
    //         if(totallyric.translation.lyric.isEmpty()){
    //             otherlanguage = totallyric.translation.lyric;
    //         }
    //     }
    // }
    if(translation.size()<10){//有可能返回一个"/n",所以说如果小于9个字符，肯定是没有翻译（一个时间戳有10字符长度)
        return _return;
    }
    nowindexend = 0;
    nowindexbegin = 0;
    nowlyricbegin = 0;
    while(nowindexbegin >= 0 && nowindexbegin <= translation.size()-1){//跳过开头没有意义的东西
        nowindexbegin = translation.indexOf('[',nowindexend);
        nowindexend = translation.indexOf('[',nowindexbegin+1);
        //qDebug()<<"translation: "<<nowindexbegin;
        if(translation[nowindexbegin+1] >= '0'&& translation[nowindexbegin+1] <= '9'){
            break;
        }
        if(nowindexbegin == -1){
            return _return;
        }
    }

    for(int i=0;i<=_return.size()-1;){//因为已经做到了，所以说这里要先处理再往后迭代
        nowlyricbegin = translation.indexOf(']',nowindexbegin);
        SongLyric thislyric;
        thislyric.trLyric = translation.mid(nowlyricbegin+1,nowindexend-nowlyricbegin-1);
        if(thislyric.trLyric.isEmpty() || thislyric.trLyric[0] == '\n'){
            nowindexbegin = translation.indexOf('[',nowindexend);//应该更新以后进行下一步，吐了，查了1小时
            if(nowindexbegin == -1){
                break;
            }
            nowindexend = translation.indexOf('[',nowindexbegin+1);
            continue;
        }

        thislyric.timeStamp =translation.mid(nowindexbegin+1,2).toInt()*60+translation.mid(nowindexbegin+4,2).toInt();
        for(int j=i;j<=_return.size()-1;j++){//把翻译和原歌词匹配。如果对不上，大概率是原歌词有不需要翻译的，直接跳过（我十分的相信网易云返回的格式）
            if(_return[j].timeStamp == thislyric.timeStamp){
                _return[j].trLyric = thislyric.trLyric;
                // if(j!=_return.size()-1 && (_return[j].timeStamp == _return[j+1].timeStamp) ){
                //     break;
                // }
                if(j!=_return.size()-1) i=j+1;
                else i=j;
                break;
            }
        }
        //_return.append(thislyric);

        nowindexbegin = translation.indexOf('[',nowindexend);
        if(nowindexbegin == -1){
            break;
        }
        nowindexend = translation.indexOf('[',nowindexbegin+1);
        //qDebug()<<"111111"<<nowindexbegin<<nowindexend;
    }
    return _return;
}

void SongLyricViewModel::loadSongLyric(SongId songId) {
    // FIXME: Implement me!
    hasLyric=false;
    emit hasLyricChanged();
    qDebug()<<"start loadSongLyric!";
    CloudMusicClient::getInstance()->getSongLyric(songId,  [this](Result<SongLyricEntity> result) {
        if (result.isErr()) {
            qDebug()<<result.unwrapErr().message;
            emit loadSongLyricFailed(result.unwrapErr().message);
            return;
        }
        auto totallyric = result.unwrap();
        // qDebug()<<totallyric.translation.lyric;
        // qDebug()<<totallyric.karaoke.lyric;
        // qDebug()<<totallyric.Romaji.lyric;
        //qDebug()<<totallyric.trivial.lyric;
        QList<SongLyricEntity> qlisttotallyric;
        qlisttotallyric.append(totallyric);
        model.clear();
        model = getcorrectlyric(qlisttotallyric);
        // for(int i=0;i<=model.size()-1;i++){
        //     qDebug()<<i<<":";
        //     qDebug()<<model[i].timeStamp;
        //     qDebug()<<model[i].lyric<<"  "<<model[i].trLyric;
        //     qDebug()<<"\n";
        // }
        hasLyric = true;
        emit hasLyricChanged();
        emit loadSongLyricSuccess();
        qDebug()<<"loadSongLyricSuccess!";
    });
}

void SongLyricViewModel::loadSongLyric_outside(QVariant songId) {
    loadSongLyric(songId.toUInt());
}


int SongLyricViewModel::changeindex(int index){
    if(currentplayindex == index){
        return model[currentplayindex].timeStamp;
    }
    setCurrentplayindex(index);
    return model[currentplayindex].timeStamp;
}

void SongLyricViewModel::changecurrentindexauto(){
    //quint64 nowtime = timeStamp/1000;
    nowtime = PlayingSongViewModel::getInstance()->getposition()/1000;
    qDebug()<<"nowtime:"<<nowtime;
    if(hasLyric && PlayingSongViewModel::getInstance()->getPlaying() && nowtime >= model[currentplayindex+1].timeStamp){
        currentplayindex++;
        emit currentplayindexChanged();
    }
}

void SongLyricViewModel::changetimestate(){
    if(timesate){
        //qDebug()<<" "<<test<<":"<<"change timestate to stop!";
        time.stop();
        test++;
    }
    else{
        time.start();
        //qDebug()<<" "<<test<<":"<<"change timestate to open!";
        test++;
    }
    timesate = !timesate;
}

void SongLyricViewModel::resettime(){
    // if(timesate) {
    //     time.stop();
    //     qDebug()<<" "<<test<<":"<<"reset timestate to stop!";
    //     test++;
    // }
    // timesate = false;
    nowtime = 0;
    currentplayindex = -1;
    emit currentplayindexChanged();
}



void SongLyricViewModel::fromsongtoid(Song song){
    loadSongLyric(song.id);
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

int SongLyricViewModel::getCurrentplayindex() const {
    return currentplayindex;
}

void SongLyricViewModel::setCurrentplayindex(int newcurrentplayindex) {
    if (currentplayindex == newcurrentplayindex)
        return;
    currentplayindex = newcurrentplayindex;
    emit currentplayindexChanged();
}



