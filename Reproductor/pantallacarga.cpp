#include "pantallacarga.h"       // Incluye el archivo de cabecera de PantallaCarga, donde se declara la clase PantallaCarga
#include <QVBoxLayout>           // Incluye la clase QVBoxLayout para gestionar el diseño de los widgets en la interfaz de manera vertical
#include <QApplication>          // Incluye la clase QApplication que permite crear y gestionar la aplicación
#include <QScreen>               // Incluye la clase QScreen para obtener información sobre las pantallas disponibles en el sistema
#include <QAudioOutput>          // Incluye la clase QAudioOutput para gestionar la salida de audio en la aplicación

// Constructor de la clase PantallaCarga
PantallaCarga::PantallaCarga(QWidget *parent)     
    : QMainWindow(parent),                        // Llama al constructor de la clase base QMainWindow y pasa el widget padre
    m_reproductor(new QMediaPlayer(this)),        // Crea un nuevo reproductor de medios para video y lo asocia al widget actual
    m_widgetVideo(new QVideoWidget(this)),        // Crea un nuevo widget de video donde se mostrará el video
    m_temporizador(new QTimer(this)),             // Crea un nuevo temporizador que se utilizará para el control de cierre forzado
    m_reproductorAudio(new QMediaPlayer(this)),   // Crea un nuevo reproductor de medios para audio
    m_salidaAudio(new QAudioOutput(this))         // Crea una nueva salida de audio para controlar el audio de la aplicación
{
    // Configurar ventana sin bordes  (solo la ventana, sin barra de título ni bordes)
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    // Centrar la ventana en la pantalla
    QScreen *screen = QApplication::primaryScreen();   // Obtiene la pantalla principal del sistema
    QRect geometriaPantalla = screen->geometry();      // Obtiene las dimensiones de la pantalla
    setGeometry(                                       // Establece las dimensiones y la posición de la ventana
        geometriaPantalla.width() / 2 - 400,           // Coloca la ventana centrada en la pantalla horizontalmente (con un desplazamiento)
        geometriaPantalla.height() / 2 - 300,          // Coloca la ventana centrada en la pantalla verticalmente (con un desplazamiento)
        780,              // Ancho de la ventana
        450               // Alto de la ventana
        );

   // Configurar el layout de la ventana (organizar los widgets)
    QVBoxLayout *layout = new QVBoxLayout();       // Crea un layout vertical
    layout->addWidget(m_widgetVideo);              // Añade el widget de video al layout

    QWidget *widgetCentral = new QWidget(this);    // Crea un widget central donde se colocarán los elementos de la interfaz
    widgetCentral->setLayout(layout);              // Asocia el layout al widget central
    setCentralWidget(widgetCentral);               // Establece el widget central de la ventana principal

    // Configurar reproductor de video
    m_reproductor->setVideoOutput(m_widgetVideo);                  // Establece el widget de video como salida para el reproductor de video
    m_reproductor->setSource(QUrl("qrc:/imagenes/nexos.mp4"));     // Establece la fuente del video (archivo dentro de los recursos de la aplicación)

    // Configurar reproductor de audio
    m_reproductorAudio->setAudioOutput(m_salidaAudio);                       // Establece la salida de audio para el reproductor de audio
    m_reproductorAudio->setSource(QUrl("qrc:/imagenes/nexossound.mp3"));     // Establece la fuente del audio (archivo dentro de los recursos de la aplicación)

    // Conectar señales y ranuras 
    connect(m_reproductor, &QMediaPlayer::mediaStatusChanged,      // Conecta la señal de cambio de estado del reproductor de video
            this, &PantallaCarga::onEstadoMediaCambiado);          // A la ranura onEstadoMediaCambiado de esta clase

    // Temporizador para forzar cierre si el video falla, no termina a tiempo
    m_temporizador->setSingleShot(true);                 // Hace que el temporizador solo se ejecute una vez
    m_temporizador->setInterval(5000); // 5 segundos     // Establece el intervalo del temporizador a 5000 ms (5 segundos)
    connect(m_temporizador, &QTimer::timeout, this, &PantallaCarga::forzarCierre);   // Conecta la señal de timeout del temporizador a la ranura forzarCierre

    // Reproducir video y sonido simultáneamente
    m_reproductor->play();          // Inicia la reproducción del video
    m_reproductorAudio->play();     // Inicia la reproducción del audio
    m_temporizador->start();        // Inicia el temporizador para controlar el cierre forzado si el video falla
}

// Destructor de la clase PantallaCarga
PantallaCarga::~PantallaCarga()
{
     // No es necesario liberar memoria explícitamente, ya que los objetos se destruyen automáticamente
}

// Ranura que se activa cuando el estado del reproductor de medios cambia
void PantallaCarga::onEstadoMediaCambiado(QMediaPlayer::MediaStatus status)
{
    if (status == QMediaPlayer::EndOfMedia) {    // Si el video ha terminado
        
        m_reproductorAudio->stop();              // Detener audio cuando el video termina   
        emit cargaCompletada();                  // Emite la señal cargaCompletada indicando que la carga ha terminado
        close();                                 // Cierra la ventana de carga
    }
}

// Ranura que se activa cuando el temporizador se agota
void PantallaCarga::forzarCierre()
{
    if (m_reproductor->mediaStatus() != QMediaPlayer::EndOfMedia) {    // Si el video no ha terminado
        m_reproductorAudio->stop();                  // Detener audio si se fuerza el cierre      
        emit cargaCompletada();                      // Emite la señal cargaCompletada indicando que la carga ha terminado
        close();                                     // Cierra la ventana de carga
    }
}
