#ifndef AUDIOVISUALIZER_H
#define AUDIOVISUALIZER_H

// Incluir dependencias necesarias para la visualización de audio y la integración en el widget
#include <QWidget>          // Clase base para crear widgets
#include <QAudioProbe>      // Clase para monitorear el audio que se está reproduciendo
#include <QAudioBuffer>     // Clase que representa datos de audio en un buffer
#include <QPainter>         // Clase para realizar operaciones de dibujo en el widget
#include <QTimer>           // Clase para configurar un temporizador, útil para actualizar la visualización en intervalos

class AudioVisualizer : public QWidget     // Definición de la clase AudioVisualizer que hereda de QWidget, lo cual permite mostrar la visualización en un widget gráfico
{
    Q_OBJECT             // Macro de Qt necesaria para definir señales y slots en la clase
public:
    explicit AudioVisualizer(QWidget *parent = nullptr);   // Constructor explícito de la clase, con el parámetro 'parent'
    void setMediaPlayer(QMediaPlayer *player);         // Método para asociar un reproductor multimedia (QMediaPlayer) a la visualización

protected:
    void paintEvent(QPaintEvent *event) override;      // Método protegido que se llama automáticamente para pintar el widget cada vez que es necesario redibujarlo

private slots:
    void processBuffer(const QAudioBuffer &buffer);     // Slot para procesar el buffer de audio y extraer niveles de volumen
    void updateVisualizer();       // Slot para actualizar la visualización en pantalla de manera periódica

private:
    QVector<float> m_levels;     // Vector para almacenar los niveles de audio de cada barra de la visualización
    QAudioProbe *m_audioProbe;   // Puntero a QAudioProbe para capturar datos de audio en tiempo real
    QTimer *m_updateTimer;       // Puntero a QTimer para actualizar la visualización en intervalos
    const int BARS_COUNT = 16;   // Constante para el número de barras en la visualización
    float m_decay = 0.95f;       // Factor de decaimiento para suavizar la visualización (evita picos bruscos)
};

#endif // AUDIOVISUALIZER_H  Fin de la protección de inclusión
