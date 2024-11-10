#ifndef PANTALLACARGA_H
#define PANTALLACARGA_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QTimer>
#include <QAudioOutput>

class PantallaCarga : public QMainWindow
{
    Q_OBJECT

public:
    explicit PantallaCarga(QWidget *parent = nullptr);
    ~PantallaCarga();

signals:
    void cargaCompletada();

private slots:
    void onEstadoMediaCambiado(QMediaPlayer::MediaStatus status);
    void forzarCierre();

private:
    QMediaPlayer *m_reproductor;
    QVideoWidget *m_widgetVideo;
    QTimer *m_temporizador;
    QMediaPlayer *m_reproductorAudio;
    QAudioOutput *m_salidaAudio;

};

#endif // PANTALLACARGA_H
