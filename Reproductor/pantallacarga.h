#ifndef PANTALLACARGA_H    // Verifica si PANTALLACARGA_H no ha sido definido previamente para evitar la inclusión repetida del archivo de cabecera
#define PANTALLACARGA_H    // Define PANTALLACARGA_H para indicar que este archivo ya ha sido incluido

#include <QMainWindow>    // Incluye la clase QMainWindow de Qt, que permite crear una ventana principal para la aplicación
#include <QMediaPlayer>   // Incluye la clase QMediaPlayer para controlar la reproducción de medios (video/audio)
#include <QVideoWidget>   // Incluye la clase QVideoWidget para mostrar contenido de video en la interfaz gráfica
#include <QTimer>         // Incluye la clase QTimer para crear temporizadores (usado, por ejemplo, para actualizar la interfaz o realizar acciones después de un tiempo)
#include <QAudioOutput>   // Incluye la clase QAudioOutput para la salida de audio, permitiendo controlar el audio en la aplicación


class PantallaCarga : public QMainWindow        // Declara una clase llamada PantallaCarga que hereda de QMainWindow (ventana principal)

{
    Q_OBJECT           // Macro que permite a la clase tener características relacionadas con las señales y ranuras (usadas para la comunicación en Qt)

public:
    explicit PantallaCarga(QWidget *parent = nullptr);     // Constructor de la clase PantallaCarga, con un parámetro opcional que define el widget padre (si no se especifica, es null)
    ~PantallaCarga();                                      // Destructor de la clase PantallaCarga, usado para liberar recursos cuando la instancia es destruida

signals:
    void cargaCompletada();                    // Define una señal llamada cargaCompletada que será emitida cuando se complete un proceso (probablemente relacionado con la carga del video o audio)

private slots:
    void onEstadoMediaCambiado(QMediaPlayer::MediaStatus status);    // Define una ranura llamada onEstadoMediaCambiado que será llamada cuando el estado del reproductor de medios cambie
    void forzarCierre();         // Define una ranura llamada forzarCierre que forzará el cierre de la ventana o el proceso de carga

private:
    QMediaPlayer *m_reproductor;          // Puntero a un objeto QMediaPlayer, utilizado para la reproducción de medios (video/audio)
    QVideoWidget *m_widgetVideo;          // Puntero a un objeto QVideoWidget, utilizado para mostrar video en la interfaz
    QTimer *m_temporizador;               // Puntero a un objeto QTimer, utilizado para manejar temporizadores en la aplicación (como un temporizador de carga)
    QMediaPlayer *m_reproductorAudio;     // Puntero a otro objeto QMediaPlayer, pero específicamente para la reproducción de audio
    QAudioOutput *m_salidaAudio;          // Puntero a un objeto QAudioOutput, utilizado para manejar la salida de audio (control de volumen, etc.)

};

#endif // PANTALLACARGA_H Fin de la inclusión condicional de este archivo de cabecera
