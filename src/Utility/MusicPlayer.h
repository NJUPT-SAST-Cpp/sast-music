#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <QMediaPlayer>
#include <Utility/NeteaseCloudMusic>

class MusicPlayer : public QMediaPlayer {
    Q_OBJECT
public:
    explicit MusicPlayer(QObject* parent = nullptr);

    void play(const QUrl& url);

private slots:
    void onAudioOutputDeviceChanged();
    void onVolumeChanged();

private:
    QAudioOutput* audioOutput;
};

#endif // MUSICPLAYER_H