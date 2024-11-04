#include "audiovisualizer.h"
#include <QPainter>
#include <QDebug>

AudioVisualizer::AudioVisualizer(QWidget *parent)
    : QWidget(parent)
{
    m_audioProbe = new QAudioProbe(this);
    m_updateTimer = new QTimer(this);

    setMinimumSize(100, 50);
    m_levels.fill(0, BARS_COUNT);

    connect(m_audioProbe, &QAudioProbe::audioBufferProbed,
            this, &AudioVisualizer::processBuffer);

    m_updateTimer->setInterval(16);
    connect(m_updateTimer, &QTimer::timeout,
            this, &AudioVisualizer::updateVisualizer);
    m_updateTimer->start();

    setAutoFillBackground(true);
    QPalette pal = palette();
    pal.setColor(QPalette::Window, Qt::black);
    setPalette(pal);
}

void AudioVisualizer::setMediaPlayer(QMediaPlayer *player)
{
    m_audioProbe->setSource(player);
}

void AudioVisualizer::processBuffer(const QAudioBuffer &buffer)
{
    if (buffer.format().channelCount() != 2)
        return;

    const qint16 *data = buffer.constData<qint16>();
    int samples = buffer.frameCount();

    QVector<float> newLevels(BARS_COUNT, 0.0f);

    for (int i = 0; i < samples; ++i) {
        float left = qAbs(data[i * 2] / 32768.0f);
        float right = qAbs(data[i * 2 + 1] / 32768.0f);
        float value = (left + right) / 2.0f;

        int index = (i * BARS_COUNT) / samples;
        if (value > newLevels[index])
            newLevels[index] = value;
    }

    for (int i = 0; i < BARS_COUNT; ++i) {
        if (newLevels[i] > m_levels[i])
            m_levels[i] = newLevels[i];
    }
}

void AudioVisualizer::updateVisualizer()
{
    for (int i = 0; i < BARS_COUNT; ++i) {
        m_levels[i] *= m_decay;
    }
    update();
}

void AudioVisualizer::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(rect(), Qt::black);

    int barWidth = width() / BARS_COUNT;
    int maxHeight = height();

    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::green);

    for (int i = 0; i < BARS_COUNT; ++i) {
        int barHeight = maxHeight * m_levels[i];
        QRect bar(i * barWidth, maxHeight - barHeight,
                  barWidth - 1, barHeight);
        painter.drawRect(bar);
    }
}
