#include "audiovisualizer.h"  // Incluye la cabecera de la clase AudioVisualizer
#include <QPainter>           // Permite dibujar gráficos en el widget
#include <QDebug>             // Permite imprimir información de depuración

AudioVisualizer::AudioVisualizer(QWidget *parent)   // Constructor de la clase AudioVisualizer
    : QWidget(parent)                 // Llama al constructor de QWidget con el widget padre
{
    m_audioProbe = new QAudioProbe(this);     // Inicializa m_audioProbe para capturar el audio
    m_updateTimer = new QTimer(this);         // Inicializa el temporizador para actualizar la visualización

    setMinimumSize(100, 50);         // Establece el tamaño mínimo del widget
    m_levels.fill(0, BARS_COUNT);    // Llena el vector m_levels con valores de 0 para cada barra

    connect(m_audioProbe, &QAudioProbe::audioBufferProbed,  
            this, &AudioVisualizer::processBuffer);        // Conecta la señal de audioBufferProbed a la función processBuffer

    m_updateTimer->setInterval(16);                       // Configura el temporizador para que se dispare cada 16 ms
    connect(m_updateTimer, &QTimer::timeout,              
            this, &AudioVisualizer::updateVisualizer);    // Conecta la señal de tiempo de m_updateTimer a la función updateVisualizer
    m_updateTimer->start();                               // Inicia el temporizador

    setAutoFillBackground(true);                 // Permite que el widget llene el fondo
    QPalette pal = palette();                    // Obtiene la paleta de colores del widget
    pal.setColor(QPalette::Window, Qt::black);   // Establece el fondo en color negro
    setPalette(pal);                             // Aplica la paleta de colores al widget
}

void AudioVisualizer::setMediaPlayer(QMediaPlayer *player)   // Método para conectar el visualizador con un reproductor multimedia
{
    m_audioProbe->setSource(player);     // Conecta el QAudioProbe al reproductor
}

void AudioVisualizer::processBuffer(const QAudioBuffer &buffer)  // Procesa los datos de audio y actualiza los niveles para la visualización
{
    if (buffer.format().channelCount() != 2)    // Si el buffer no tiene 2 canales, sale de la función
        return;

    const qint16 *data = buffer.constData<qint16>();   // Obtiene los datos de audio en formato de 16 bits
    int samples = buffer.frameCount();                 // Número de muestras en el buffer

    QVector<float> newLevels(BARS_COUNT, 0.0f);   // Inicializa un vector temporal para almacenar los niveles de las barras

    for (int i = 0; i < samples; ++i) {                     // Itera a través de cada muestra de audio
        float left = qAbs(data[i * 2] / 32768.0f);          // Calcula el valor absoluto de cada canal (izquierdo y derecho)
        float right = qAbs(data[i * 2 + 1] / 32768.0f);
        float value = (left + right) / 2.0f;                // Promedio entre los dos canales

        int index = (i * BARS_COUNT) / samples;    // Calcula el índice de la barra correspondiente
        if (value > newLevels[index])             
            newLevels[index] = value;              // Actualiza el valor máximo para la barra
    }

    // Actualiza m_levels con los nuevos niveles, manteniendo el valor máximo actual
    for (int i = 0; i < BARS_COUNT; ++i) {
        if (newLevels[i] > m_levels[i])
            m_levels[i] = newLevels[i];
    }
}

void AudioVisualizer::updateVisualizer()         // Reductor de los niveles de audio para crear un efecto de decaimiento
{
    for (int i = 0; i < BARS_COUNT; ++i) {
        m_levels[i] *= m_decay;                  // Aplica el factor de decaimiento a cada barra
    }
    update();          // Llama a update() para redibujar el widget
}

void AudioVisualizer::paintEvent(QPaintEvent *)   // Método que se llama automáticamente cuando el widget necesita ser redibujado
{
    QPainter painter(this);                   // Crea un objeto QPainter para dibujar en el widget
    painter.fillRect(rect(), Qt::black);      // Llena el fondo del widget con color negro


    int barWidth = width() / BARS_COUNT;      // Calcula el ancho de cada barra
    int maxHeight = height();                 // Altura máxima de las barras (altura del widget)

    painter.setPen(Qt::NoPen);                // Desactiva el contorno de las barras
    painter.setBrush(Qt::green);              // Establece el color de las barras en verde

    // Dibuja cada barra de la visualización
    for (int i = 0; i < BARS_COUNT; ++i) {
        int barHeight = maxHeight * m_levels[i];         // Calcula la altura de la barra según el nivel de audio
        QRect bar(i * barWidth, maxHeight - barHeight,   // Define la geometría de la barra
                  barWidth - 1, barHeight);
        painter.drawRect(bar);                           // Dibuja la barra
    }
}
