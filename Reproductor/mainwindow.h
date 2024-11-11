#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>      // Ventana principal de la aplicación
#include <QMediaPlayer>     // Reproducción de audio y video
#include <QAudioOutput>     // Salida de audio
#include <QVideoWidget>     // Visualización de video
#include <QFileDialog>      // Diálogos de selección de archivos
#include <QMessageBox>      // Cuadros de mensaje
#include <QTime>            // Manejo de tiempo
#include <QDockWidget>      // Paneles acoplables
#include <QListWidget>      // Listas de elementos
#include <QMap>             // Mapeo de archivos a rutas
#include <QDirIterator>
#include <QDir>
#include <QShortcut>
#include <QLabel>
#include <QPropertyAnimation>
#include <QTimer>
#include <QSettings>
#include <QFileInfo>
#include <QCloseEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; } // Espacio de nombres para la interfaz de usuario
QT_END_NAMESPACE

// Clase MainWindow que representa la ventana principal de la aplicación
class MainWindow : public QMainWindow
{
    Q_OBJECT // Macro que permite la utilización de señales y slots

public:
    explicit MainWindow(QWidget *parent = nullptr); // Constructor
    ~MainWindow();                                  // Destructor

protected:
    void closeEvent(QCloseEvent *event) override;   // Evento al cerrar la ventana

private slots:
    // Slots de interfaz de usuario para botones y sliders
    void on_actionOpen_triggered();                       // Abrir archivo
    void on_horizontalSlider_Duration_valueChanged(int value); // Cambiar duración
    void on_pushButton_Play_Pause_clicked();              // Reproducir/pausar
    void on_pushButton_Stop_clicked();                    // Parar
    void on_pushButton_Volume_clicked();                  // Silencio
    void on_horizontalSlider_Volume_valueChanged(int value); // Cambiar volumen
    void on_pushButton_Seek_Backward_clicked();           // Retroceder
    void on_pushButton_Seek_Forward_clicked();            // Adelantar
    void on_pushButton_Next_clicked();                    // Siguiente
    void on_pushButton_Previous_clicked();                // Anterior
    void onFileSelected(QListWidgetItem *item);           // Selección de archivo
    void onMediaError(QMediaPlayer::Error error);         // Error en el medio
    void onMediaStatusChanged(QMediaPlayer::MediaStatus status); // Cambio de estado

    // Slots para manejar actualizaciones de reproducción
    void durationChanged(qint64 duration);                // Cambio de duración
    void positionChanged(qint64 duration);                // Cambio de posición
    void increaseVolume();                                // Aumentar volumen
    void decreaseVolume();                                // Disminuir volumen

    // Slots personalizados para funcionalidades adicionales
    void addFile();                                       // Agregar archivo
    void playFile(const QString& filePath);               // Reproducir archivo
    void updateMarqueeText();                             // Actualizar texto flotante

private:
    Ui::MainWindow *ui;                                   // Interfaz de usuario
    QMediaPlayer *Player;                                 // Reproductor principal
    QMediaPlayer *BackgroundPlayer;                       // Reproductor de fondo
    QVideoWidget *Video;                                  // Widget de video principal
    QVideoWidget *BackgroundVideo;                        // Widget de video de fondo
    QAudioOutput *audioOutput;                            // Salida de audio

    // Variables de estado
    bool IS_Pause = true;                                 // Estado de pausa
    bool IS_Muted = false;                                // Estado de silencio
    qint64 mDuration;                                     // Duración del medio

    // Elementos de interfaz adicionales
    QDockWidget *dock;                                    // Panel acoplable de archivos
    QListWidget *fileList;                                // Lista de archivos
    QLabel *m_floatingLabel;                              // Etiqueta flotante
    QPropertyAnimation *m_labelAnimation;                 // Animación de la etiqueta

    // Reproducción y gestión de lista de archivos
    QStringList playlist;                                 // Lista de reproducción
    QMap<QString, QString> fileMap;                       // Mapa de archivos con rutas
    int currentNumber = 1;                                // Número actual
    int currentIndex = -1;                                // Índice de reproducción actual

    // Elementos de texto flotante
    QTimer *m_marqueeTimer;                               // Temporizador de desplazamiento
    QString m_currentFileName;                            // Nombre del archivo actual
    int m_scrollPosition;                                 // Posición de desplazamiento

    // Métodos auxiliares
    void initializeVideoWidget();                         // Inicializar widget de video
    void initializePlaylistIndex();                       // Inicializar índice de lista
    void updateDuration(qint64 Duration);                 // Actualizar duración
    void showCover(const QString &fileName);              // Mostrar portada del archivo
    void createFloatingLabel();                           // Crear etiqueta flotante
    void updateFloatingLabel(const QString& fileName);    // Actualizar etiqueta flotante
    void savePlaylist();                                  // Guardar lista de reproducción
    void loadSavedPlaylist();                             // Cargar lista guardada
    void addFileToPlaylist(const QString& filePath);      // Agregar archivo a lista
    QString findBackgroundVideo();                        // Encontrar video de fondo
    void clearPlaylist();                                 // Limpiar lista de reproducción
};

#endif // MAINWINDOW_H
