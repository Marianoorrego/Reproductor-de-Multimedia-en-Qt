#include "pantallacarga.h"
#include <QVBoxLayout>
#include <QApplication>
#include <QScreen>
#include <QAudioOutput>

PantallaCarga::PantallaCarga(QWidget *parent)
    : QMainWindow(parent),
    m_reproductor(new QMediaPlayer(this)),
    m_widgetVideo(new QVideoWidget(this)),
    m_temporizador(new QTimer(this)),
    m_reproductorAudio(new QMediaPlayer(this)),
    m_salidaAudio(new QAudioOutput(this))
{
    // Configurar ventana sin bordes
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    // Centrar la ventana
    QScreen *screen = QApplication::primaryScreen();
    QRect geometriaPantalla = screen->geometry();
    setGeometry(
        geometriaPantalla.width() / 2 - 400,
        geometriaPantalla.height() / 2 - 300,
        780,
        450
        );

    // Configurar layout
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(m_widgetVideo);

    QWidget *widgetCentral = new QWidget(this);
    widgetCentral->setLayout(layout);
    setCentralWidget(widgetCentral);

    // Configurar reproductor de video
    m_reproductor->setVideoOutput(m_widgetVideo);
    m_reproductor->setSource(QUrl("qrc:/imagenes/nexos.mp4"));

    // Configurar reproductor de audio
    m_reproductorAudio->setAudioOutput(m_salidaAudio);
    m_reproductorAudio->setSource(QUrl("qrc:/imagenes/nexossound.mp3"));

    // Conectar señales
    connect(m_reproductor, &QMediaPlayer::mediaStatusChanged,
            this, &PantallaCarga::onEstadoMediaCambiado);

    // Temporizador para forzar cierre si el video falla
    m_temporizador->setSingleShot(true);
    m_temporizador->setInterval(5000); // 5 segundos
    connect(m_temporizador, &QTimer::timeout, this, &PantallaCarga::forzarCierre);

    // Reproducir video y sonido
    m_reproductor->play();
    m_reproductorAudio->play();
    m_temporizador->start();
}

PantallaCarga::~PantallaCarga()
{
}

void PantallaCarga::onEstadoMediaCambiado(QMediaPlayer::MediaStatus status)
{
    if (status == QMediaPlayer::EndOfMedia) {
        // Detener audio cuando el video termina
        m_reproductorAudio->stop();
        emit cargaCompletada();
        close();
    }
}

void PantallaCarga::forzarCierre()
{
    if (m_reproductor->mediaStatus() != QMediaPlayer::EndOfMedia) {
        // Detener audio si se fuerza el cierre
        m_reproductorAudio->stop();
        emit cargaCompletada();
        close();
    }
}
