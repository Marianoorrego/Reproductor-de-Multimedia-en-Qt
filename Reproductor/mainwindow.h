#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>      // Clase base para la ventana principal de la aplicación
#include <QMediaPlayer>     // Clase para la reproducción de audio y video
#include <QAudioOutput>     // Clase para la salida de audio
#include <QVideoWidget>     // Clase para la visualización de video
#include <QFileDialog>      // Clase para mostrar diálogos de selección de archivos
#include <QMessageBox>      // Clase para mostrar cuadros de mensaje
#include <QTime>            // Clase para manejar tiempo
#include <QDockWidget>      // Clase para crear paneles acoplables
#include <QListWidget>      // Clase para mostrar listas de elementos
#include <QMap>             // Clase para mapear archivos a rutas
#include <QDirIterator>
#include <QDir>
#include <QShortcut>



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; } // Declaración del espacio de nombres para la interfaz de usuario
QT_END_NAMESPACE

// Clase MainWindow que representa la ventana principal de la aplicación
class MainWindow : public QMainWindow
{
    Q_OBJECT // Macro que permite la utilización de señales y slots

public:
    // Constructor de la clase, inicializa la ventana principal
    MainWindow(QWidget *parent = nullptr);

    // Destructor de la clase
    ~MainWindow();

private slots:
    // Slot para manejar la acción de abrir un archivo
    void on_actionOpen_triggered();

    // Slot para manejar cambios en el slider de duración
    void on_horizontalSlider_Duration_valueChanged(int value);

    // Slot para manejar el botón de reproducción/pausa
    void on_pushButton_Play_Pause_clicked();

    // Slot para manejar el botón de parada
    void on_pushButton_Stop_clicked();

    // Slot para manejar el botón de volumen
    void on_pushButton_Volume_clicked();

    // Slot para manejar cambios en el slider de volumen
    void on_horizontalSlider_Volume_valueChanged(int value);

    // Slot para manejar el botón de retroceso
    void on_pushButton_Seek_Backward_clicked();

    // Slot para manejar el botón de avance
    void on_pushButton_Seek_Forward_clicked();

    // Slot para manejar la selección de un archivo de la lista
    void onFileSelected(QListWidgetItem *item);

    // Slot para manejar cambios en la duración del medio
    void durationChanged(qint64 duration);

    // Slot para manejar cambios en la posición de reproducción
    void positionChanged(qint64 duration);

    // Agregar un archivo a la lista de reproducción
    void addFile();

    void onMediaError(QMediaPlayer::Error error);
    void onMediaStatusChanged(QMediaPlayer::MediaStatus status);
    void increaseVolume();
    void decreaseVolume();
void initializeVideoWidget();

    void moveItemUp();
    void moveItemDown();
    void on_pushButton_Next_clicked();   // Slot para el botón siguiente
    void on_pushButton_Previous_clicked(); // Slot para el botón anterior
     void playFile(const QString& filePath);
private:
    // Puntero a la interfaz de usuario
    Ui::MainWindow *ui;
 QString findBackgroundVideo();
    // Puntero al reproductor de medios
    QMediaPlayer *Player;

    // Puntero al reproductor de fondo para videos
    QMediaPlayer *BackgroundPlayer;

    // Puntero al widget de video
    QVideoWidget *Video;

    // Puntero al widget de video de fondo
      QVideoWidget *BackgroundVideo;

    // Puntero a la salida de audio
    QAudioOutput *audioOutput;

    // Variables de estado para la pausa y el silencio
    bool IS_Pause = true;
    bool IS_Muted = false;

    // Duración total del archivo multimedia
    qint64 mDuration;

    // Actualiza la duración mostrada en la interfaz
    void updateDuration(qint64 Duration);

    // Muestra la portada del archivo seleccionado
    void showCover(const QString &fileName);

    // Puntero a un panel acoplable para la lista de archivos
    QDockWidget *dock;

    // Puntero a la lista de archivos
    QListWidget *fileList;

    // Mapa de archivos con sus rutas
    QMap<QString, QString> fileMap;

    int currentIndex = -1; // Índice del archivo actual
    QStringList playlist;   // Lista de archivos para la reproducción
void initializePlaylistIndex();

};


#endif // MAINWINDOW_H
