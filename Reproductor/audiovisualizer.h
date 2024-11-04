#ifndef AUDIOVISUALIZER_H
#define AUDIOVISUALIZER_H

#include <QWidget>
#include <QAudioProbe>
#include <QAudioBuffer>
#include <QPainter>
#include <QTimer>

class AudioVisualizer : public QWidget
{
    Q_OBJECT
public:
    explicit AudioVisualizer(QWidget *parent = nullptr);
    void setMediaPlayer(QMediaPlayer *player);

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void processBuffer(const QAudioBuffer &buffer);
    void updateVisualizer();

private:
    QVector<float> m_levels;
    QAudioProbe *m_audioProbe;
    QTimer *m_updateTimer;
    const int BARS_COUNT = 16;
    float m_decay = 0.95f;
};

#endif // AUDIOVISUALIZER_H
