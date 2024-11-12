#include "mainwindow.h" // Incluye el archivo de encabezado que define la clase MainWindow
#include "ui_mainwindow.h" // Incluye la interfaz de usuario generada por Qt Designer para la ventana principal


MainWindow::MainWindow(QWidget *parent) // Constructor de la clase MainWindow, recibe un parámetro 'parent' que es el widget padre
    : QMainWindow(parent)               // Llama al constructor de la clase base QMainWindow, pasando el parámetro 'parent'
    , ui(new Ui::MainWindow)           // Crea una nueva instancia de la interfaz de usuario para la ventana principal
    , Player(nullptr)                  // Inicializa el puntero 'Player' a nullptr, este será el reproductor de medios
    , BackgroundPlayer(nullptr)       // Inicializa el puntero 'BackgroundPlayer' a nullptr, relacionado con el video en segundo plano
    , Video(nullptr)                  // Inicializa el puntero 'Video' a nullptr, este será el video principal
    , BackgroundVideo(nullptr)       // Inicializa el puntero 'BackgroundVideo' a nullptr, este será el video en segundo plano
    , audioOutput(nullptr)           // Inicializa el puntero 'audioOutput' a nullptr, este manejará la salida de audio
    , IS_Pause(false)                // Inicializa el estado de pausa como falso
    , IS_Muted(false)                // Inicializa el estado de mute como falso
    , mDuration(0)                   // Inicializa la duración del video como 0
    , fileList(nullptr)              // Inicializa el puntero 'fileList' a nullptr, que será la lista de archivos multimedia
    , currentNumber(0)               // Inicializa el número actual de archivo como 0
    , currentIndex(-1)               // Inicializa el índice actual como -1, lo que indica que no se ha seleccionado ningún archivo
    , m_floatingLabel(nullptr)       // Inicializa el puntero 'm_floatingLabel' a nullptr, que se puede usar para mostrar etiquetas flotantes
    , m_marqueeTimer(nullptr)        // Inicializa el puntero 'm_marqueeTimer' a nullptr, que se puede usar para temporizadores
    , m_scrollPosition(0)            // Inicializa la posición de desplazamiento a 0
{
    qApp->setStyle("Fusion");  // Mantener Fusion, Establece el estilo de la aplicación como 'Fusion', que es un estilo de interfaz predefinido en Qt

    QPalette darkPalette; // Crea un objeto QPalette para configurar los colores de la interfaz
    darkPalette.setColor(QPalette::Window, QColor(20, 20, 20));           // Establece el color de fondo de la ventana como un gris oscuro
    darkPalette.setColor(QPalette::WindowText, QColor(220, 220, 220));    // Establece el color del texto de la ventana como gris claro
    darkPalette.setColor(QPalette::Base, QColor(15, 15, 15));             // Establece el color de fondo base como un gris más oscuro
    darkPalette.setColor(QPalette::AlternateBase, QColor(25, 25, 25));    // Establece el color de fondo alternativo
    darkPalette.setColor(QPalette::ToolTipBase, QColor(30, 30, 30));      // Establece el color de fondo de las tooltips como un gris oscuro
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);               // Establece el color del texto de las tooltips como blanco
    darkPalette.setColor(QPalette::Text, QColor(220, 220, 220));          // Establece el color del texto en general como gris claro
    darkPalette.setColor(QPalette::Button, QColor(25, 25, 25));           // Establece el color de fondo de los botones como gris muy oscuro
    darkPalette.setColor(QPalette::ButtonText, Qt::white);                // Establece el color del texto de los botones como blanco
    darkPalette.setColor(QPalette::BrightText, Qt::red);                  // Establece el color del texto brillante como rojo
    darkPalette.setColor(QPalette::Link, QColor(50, 100, 200));           // Establece el color de los enlaces como un azul suave
    darkPalette.setColor(QPalette::Highlight, QColor(50, 100, 200));      // Establece el color de selección como azul suave
    darkPalette.setColor(QPalette::HighlightedText, Qt::white);           // Establece el color del texto destacado como blanco

    qApp->setPalette(darkPalette);     /// Aplica la paleta de colores a la aplicación

    qApp->setStyleSheet(R"(
    * {
        background-color: #141414;  // Aún más oscuro 
        color: #DDDDDD;             // Texto gris claro 
    }
    QListWidget {
        background-color: #0F0F0F;  // Negro profundo 
        border: 1px solid #333333;  //Borde gris oscuro 
    }
    QLineEdit, QTextEdit {
        background-color: #0A0A0A;  //  Negro más profundo 
        color: #DDDDDD;             //Texto gris claro 
        border: 1px solid #333333;  //Borde gris oscuro
    }
    QPushButton {
        background-color: #1E1E1E; //  Gris muy oscuro 
        color: #DDDDDD;            // Texto gris claro
        border: 1px solid #333333; // Borde gris oscuro
        padding: 5px;              // Espaciado alrededor del texto 
        border-radius: 3px;        // Bordes redondeados 
    }
    QPushButton:hover {
        background-color: #262626;  // Color más claro cuando el mouse pasa sobre el botón 
    }
    QPushButton:pressed {
        background-color: #111111;  //  Color más oscuro cuando el botón es presionado
    }
    QSlider::handle:horizontal {
        background: #3A6EA5;       //  Azul más oscuro para el control deslizante
        width: 18px;
        margin: -5px 0;
        border-radius: 9px;       //  Bordes redondeados para el control deslizante
    }
    QSlider::groove:horizontal {
        background: #2A2A2A;     // Fondo gris oscuro para la pista del control deslizante
        height: 8px;             // Altura del control deslizante 
    }
    QMenuBar, QMenu {
        background-color: #141414; // Fondo oscuro para la barra de menús
        color: #DDDDDD;            // Texto gris claro en los menús
    }
    QMenu::item:selected {
        background-color: #2A2A2A;  // Color de fondo para los ítems seleccionados en el menú
    }
)");                              // Estilos personalizados aplicados
    ui->setupUi(this);            // Configura la interfaz de usuario para la ventana principal
    resize(1100, 630);

    // Inicialización del reproductor de medios y configuración de salida de audio
    Player = new QMediaPlayer(this);           // Crea un nuevo reproductor de medios
    audioOutput = new QAudioOutput(this);      // Crea un nuevo objeto de salida de audio
    Player->setAudioOutput(audioOutput);       // Asocia la salida de audio al reproductor

    // Inicializar el QVideoWidget una vez
    BackgroundVideo = new QVideoWidget(this);  // Crea un nuevo widget de video para el fondo
    initializeVideoWidget();                   // Llama a una función para inicializar el widget de video

    // Creación del panel lateral (Dock) para la lista de archivos multimedia
    QDockWidget *dock = new QDockWidget(tr("Lista de archivos"), this);        // Crea un widget de panel lateral para la lista de archivos
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);   // Permite que el panel se coloque a la izquierda o a la derecha

    // Widget contenedor para la lista y el botón
    QWidget *dockContents = new QWidget();                       // Crea un nuevo widget para el contenido del panel lateral
    QVBoxLayout *dockLayout = new QVBoxLayout(dockContents);     // Crea un diseño vertical para el contenido del panel

    // Lista de archivos
    fileList = new QListWidget();           // Crea un widget de lista para mostrar los archivos multimedia
    fileList->setDragEnabled(true);        // Habilita el arrastre de archivos en la lista
    fileList->setDropIndicatorShown(true); // Muestra un indicador de caída cuando se arrastran archivos
    fileList->setAcceptDrops(true);        // Permite que se acepten archivos arrastrados a la lista
    
    // Botón para añadir archivos
    QPushButton *addFileButton = new QPushButton("Añadir archivo");              // Se crea un botón con el texto "Añadir archivo"
    connect(addFileButton, &QPushButton::clicked, this, &MainWindow::addFile);   // Se conecta el evento de hacer clic en el botón a la función "addFile" para manejar la acción


    // Añadir widgets al layout
    dockLayout->addWidget(fileList);        // Se agrega el widget de la lista de archivos (fileList) al layout del dock
    dockLayout->addWidget(addFileButton);   // Se agrega el botón de añadir archivo (addFileButton) al layout del dock


    // Conectar selección de archivo a la reproducción correspondiente
    connect(fileList, &QListWidget::itemClicked, this, &MainWindow::onFileSelected); // Se conecta el evento de clic en un archivo de la lista a la función "onFileSelected" que maneja la selección y reproducción

    // Configurar el widget del dock
    dock->setWidget(dockContents);                 // Se establece el contenido del dock (dockContents) como el widget principal que se mostrará
    addDockWidget(Qt::LeftDockWidgetArea, dock);   // Se agrega el dock a la ventana en el área izquierda

    // Inicialización del reproductor y widget de video de fondo
    BackgroundPlayer = new QMediaPlayer(this);             // Se crea un nuevo reproductor de medios para el video de fondo
    BackgroundVideo = new QVideoWidget(ui->groupBox_Video); // Se crea un widget de video para mostrar el fondo en el área de "groupBox_Video"
    BackgroundVideo->setGeometry(5, 5, ui->groupBox_Video->width() - 10, ui->groupBox_Video->height() - 10);  // Se establece el tamaño y posición del widget dentro del grupo
    BackgroundPlayer->setVideoOutput(BackgroundVideo);   // Se asigna el widget de video como la salida del reproductor de medios de fondo
    BackgroundVideo->setVisible(false);                  // Se oculta el widget de video de fondo inicialmente

    // Configuración para reproducción en bucle del video de fondo
    connect(BackgroundPlayer, &QMediaPlayer::mediaStatusChanged, this, [=](QMediaPlayer::MediaStatus status) {     // Se conecta el cambio de estado del reproductor de medios a una función lambda
        if (status == QMediaPlayer::EndOfMedia) {     // Si el video llega al final
            BackgroundPlayer->play();                 // Se inicia nuevamente la reproducción del video para que se repita en bucle
        }
    });

    // Asignación de iconos a los botones del reproductor
    ui->pushButton_Play_Pause->setIcon(QIcon(":/imagenes/play.png"));            // Se asigna el icono de "play" al botón de reproducción/pausa
    ui->pushButton_Stop->setIcon(QIcon(":/imagenes/stop.png"));                  // Se asigna el icono de "stop" al botón de detener
    ui->pushButton_Seek_Backward->setIcon(QIcon(":/imagenes/retroceder.png"));   // Se asigna el icono de "retroceder" al botón de retroceso
    ui->pushButton_Seek_Forward->setIcon(QIcon(":/imagenes/avanzar.png"));       // Se asigna el icono de "avanzar" al botón de avance
    ui->pushButton_Volume->setIcon(QIcon(":/imagenes/sound.png"));               // Se asigna el icono de "sound" al botón de volumen
    ui->pushButton_Next->setIcon(QIcon(":/imagenes/next.png"));                  // Se asigna el icono de "next" al botón de siguiente
    ui->pushButton_Previous->setIcon(QIcon(":/imagenes/prev.png"));             // Se asigna el icono de "prev" al botón de anterior

    // Configuración inicial de volumen
    ui->horizontalSlider_Volume->setMinimum(0);                             // Se establece el valor mínimo del control deslizante de volumen en 0
    ui->horizontalSlider_Volume->setMaximum(100);                           // Se establece el valor máximo del control deslizante de volumen en 100
    ui->horizontalSlider_Volume->setValue(30);                              // Se establece el valor inicial del control deslizante de volumen en 30
    audioOutput->setVolume(ui->horizontalSlider_Volume->value() / 100.0);   // Se ajusta el volumen del reproductor de medios en función del valor del control deslizante

    // Conexión de las señales del reproductor
    connect(Player, &QMediaPlayer::durationChanged, this, &MainWindow::durationChanged);         // Conecta el cambio de duración del archivo al método "durationChanged"
    connect(Player, &QMediaPlayer::positionChanged, this, &MainWindow::positionChanged);         // Conecta el cambio de posición del archivo al método "positionChanged"
    connect(Player, &QMediaPlayer::errorOccurred, this, &MainWindow::onMediaError);              // Conecta los errores del reproductor al método "onMediaError"
    connect(Player, &QMediaPlayer::mediaStatusChanged, this, &MainWindow::onMediaStatusChanged); // Conecta el cambio de estado del reproductor al método "onMediaStatusChanged"

    // Atajos de teclado
    QShortcut *playPauseShortcut = new QShortcut(QKeySequence("Space"), this);                                 // Se crea un atajo de teclado para reproducir/pausar con la barra espaciadora
    connect(playPauseShortcut, &QShortcut::activated, this, &MainWindow::on_pushButton_Play_Pause_clicked);    // Se conecta la activación del atajo al método de clic del botón de reproducción/pausa

    QShortcut *stopShortcut = new QShortcut(QKeySequence("Ctrl+S"), this);                              // Se crea un atajo de teclado para detener con Ctrl+S
    connect(stopShortcut, &QShortcut::activated, this, &MainWindow::on_pushButton_Stop_clicked);       // Se conecta la activación del atajo al método de clic del botón de detener

    QShortcut *muteShortcut = new QShortcut(QKeySequence("M"), this);                                 // Se crea un atajo de teclado para silenciar con la tecla "M"
    connect(muteShortcut, &QShortcut::activated, this, &MainWindow::on_pushButton_Volume_clicked);    // Se conecta la activación del atajo al método de clic del botón de volumen

    QShortcut *seekForwardShortcut = new QShortcut(QKeySequence("Right"), this);                                 // Se crea un atajo de teclado para avanzar con la tecla de flecha derecha
    connect(seekForwardShortcut, &QShortcut::activated, this, &MainWindow::on_pushButton_Seek_Forward_clicked);  // Se conecta la activación del atajo al método de clic del botón de avance

    QShortcut *seekBackwardShortcut = new QShortcut(QKeySequence("Left"), this);                                    // Se crea un atajo de teclado para retroceder con la tecla de flecha izquierda
    connect(seekBackwardShortcut, &QShortcut::activated, this, &MainWindow::on_pushButton_Seek_Backward_clicked);   // Se conecta la activación del atajo al método de clic del botón de retroceso

    QShortcut *increaseVolumeShortcut = new QShortcut(QKeySequence("Up"), this);                 // Se crea un atajo de teclado para aumentar el volumen con la tecla de flecha hacia arriba
    connect(increaseVolumeShortcut, &QShortcut::activated, this, &MainWindow::increaseVolume);   // Se conecta la activación del atajo al método de aumento de volumen

    QShortcut *decreaseVolumeShortcut = new QShortcut(QKeySequence("Down"), this);               // Se crea un atajo de teclado para disminuir el volumen con la tecla de flecha hacia abajo
    connect(decreaseVolumeShortcut, &QShortcut::activated, this, &MainWindow::decreaseVolume);    // Se conecta la activación del atajo al método de disminución de volumen

    // Desactivar características de desacople
    dock->setFeatures(QDockWidget::NoDockWidgetFeatures);    // Se desactivan las características del dock para que no sea acoplable o despegable

    connect(ui->pushButton_Next, &QPushButton::clicked, this, &MainWindow::on_pushButton_Next_clicked);           // Se conecta el clic en el botón de siguiente al método "on_pushButton_Next_clicked"
    connect(ui->pushButton_Previous, &QPushButton::clicked, this, &MainWindow::on_pushButton_Previous_clicked);   // Se conecta el clic en el botón de anterior al método "on_pushButton_Previous_clicked"

    // Establecer el video de fondo directamente
    BackgroundPlayer->setSource(QUrl("qrc:/imagenes/nexosbackgroud.mp4"));    // Se establece la fuente del video de fondo (en este caso, un archivo MP4 en los recursos)

    // Botón para limpiar la lista de archivos
    QPushButton *clearPlaylistButton = new QPushButton("Limpiar Lista", dockContents);      // Se crea un botón para limpiar la lista de archivos
    dockLayout->addWidget(clearPlaylistButton);                                             // Se agrega el botón de limpiar lista al layout del dock
    connect(clearPlaylistButton, &QPushButton::clicked, this, &MainWindow::clearPlaylist);  // Se conecta el clic en el botón a la función "clearPlaylist" para limpiar la lista

    // Atajo para limpiar la lista
    QShortcut *clearPlaylistShortcut = new QShortcut(QKeySequence("Ctrl+L"), this);          // Se crea un atajo para limpiar la lista con Ctrl+L
    connect(clearPlaylistShortcut, &QShortcut::activated, this, &MainWindow::clearPlaylist);  // Se conecta el atajo al método de limpiar lista
    createFloatingLabel();   // Llama a la función createFloatingLabel() para crear una etiqueta flotante en la interfaz gráfica
    setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);  /*Modifica las banderas de la ventana para deshabilitar el botón de maximizar de la ventana, usando "windowFlags()"
    para obtener las banderas actuales y eliminando el "Qt::WindowMaximizeButtonHint" */
    loadSavedPlaylist();   // Llama a la función loadSavedPlaylist() para cargar una lista de reproducción guardada previamente.


    setWindowTitle("N E X O S");                  // Establece el título de la ventana de la aplicación como "N E X O S", que es el nombre del proyecto o la aplicación
    QSettings settings("Nexos Media", "NEXOS");   // Crea un objeto QSettings para manejar la configuración de la aplicación. "Nexos Media" es el nombre de la organización y "NEXOS" es el nombre del programa, lo que permitirá guardar o cargar configuraciones específicas de la aplicación.

}

MainWindow::~MainWindow()        // Destructor de la clase MainWindow. Se ejecuta cuando se destruye el objeto de la ventana principal.

{
    // Guardar la lista antes de cerrar
    savePlaylist();      // Llama al método savePlaylist() para guardar la lista de reproducción actual antes de cerrar la ventana.

    // Resto del código de limpieza existente
    if (m_marqueeTimer) {        // Si el temporizador de desplazamiento (marquee) existe...
        m_marqueeTimer->stop();  // Detiene el temporizador.
        delete m_marqueeTimer;   // Elimina el objeto temporizador de memoria.
    }

    if (m_floatingLabel) {        // Si el objeto de etiqueta flotante existe...
        delete m_floatingLabel;   // Elimina el objeto de etiqueta flotante de memoria.
    }
    if (m_labelAnimation) {       // Si el objeto de animación de la etiqueta existe...
        delete m_labelAnimation;  // Elimina el objeto de animación de memoria.
    }
    delete ui;           / Elimina el objeto de interfaz de usuario generado por Qt Designer.
}

void MainWindow::onMediaError(QMediaPlayer::Error error)         // Método que maneja los errores de reproducción de medios.
{
    QString errorMessage;     // Cadena para almacenar el mensaje de error.

    switch (error) {             // Compara el tipo de error recibido.
    case QMediaPlayer::NoError:  // Si no hay error...
        return; // No hay error  // No hacer nada, ya que no hay error.
    case QMediaPlayer::ResourceError:    // Si el error es de recurso...
        errorMessage = tr("Error de recurso: No se puede acceder al archivo.");  // Mensaje de error específico para error de recurso.
        break;
    case QMediaPlayer::FormatError:    // Si el error es de formato...
        errorMessage = tr("Error de formato: El archivo no es compatible.");   // Mensaje de error específico para error de formato.
        break;
    default:                                       // Si el error no es uno de los anteriores...
        errorMessage = tr("Error desconocido.");   // Mensaje de error general para cualquier otro tipo de error.
        break;
    }

    QMessageBox::warning(this, tr("Error de Reproducción"), errorMessage);   // Muestra una ventana de advertencia con el mensaje de error.
}


void MainWindow::on_pushButton_Volume_clicked()                           // Método que se llama cuando se hace clic en el botón de volumen.
{
    IS_Muted = !IS_Muted; // Cambia el estado de silencio                 // Cambia el estado de silencio (si está silenciado, lo activa, y viceversa).
    audioOutput->setVolume(IS_Muted ? 0 : ui->horizontalSlider_Volume->value() / 100.0);    // Si está silenciado, pone el volumen a 0, de lo contrario, ajusta el volumen según el valor del deslizador.
    ui->pushButton_Volume->setIcon(IS_Muted ? QIcon(":/imagenes/mute.png") : QIcon(":/imagenes/sound.png"));   // Cambia el icono del botón de volumen entre "mute" y "sound" dependiendo del estado de silencio.
}

// Método que actualiza la duración máxima del video en el deslizador de progreso
void MainWindow::durationChanged(qint64 duration)          // Método que se llama cuando cambia la duración del video.
{
    mDuration = duration / 1000;                           // Convierte la duración a segundos (divide entre 1000).
    ui->horizontalSlider_Duration->setMaximum(mDuration);  // Establece la duración máxima del deslizador de progreso al valor calculado.

}

// Método que actualiza la posición del video en el deslizador de progreso
void MainWindow::positionChanged(qint64 duration)             // Método que se llama cuando cambia la posición actual del video.
{
    // Solo actualizar el slider si no está siendo arrastrado
    if (!ui->horizontalSlider_Duration->isSliderDown())     // Si el deslizador no está siendo arrastrado...
    
    {
        ui->horizontalSlider_Duration->blockSignals(true);         // Bloquea temporalmente las señales del deslizador para evitar cambios no deseados.
        ui->horizontalSlider_Duration->setValue(duration / 1000);  // Establece el valor del deslizador con la posición actual del video en segundos.
        ui->horizontalSlider_Duration->blockSignals(false);        // Desbloquea las señales del deslizador.
    }

    // Actualizar las etiquetas de tiempo solo cada segundo completo
    static qint64 lastSecond = -1;              // Variable estática para almacenar el segundo anterior (inicialmente -1).
    qint64 currentSecond = duration / 1000;     // Convierte la duración actual a segundos.
    if (currentSecond != lastSecond)            // Si el segundo actual es diferente al último segundo registrado...
    {
        updateDuration(currentSecond);  // Actualiza las etiquetas de tiempo con el nuevo segundo.
        lastSecond = currentSecond;     // Actualiza el último segundo registrado con el segundo actual.
    }
}

// Actualización del tiempo actual y total del video
void MainWindow::updateDuration(qint64 Duration)    // Método que actualiza las etiquetas de tiempo (actual y total) del video.
{
    if (Duration || mDuration)      // Si la duración actual o la duración total son válidas...
    {
        QTime CurrentTime((Duration / 3600) % 60, (Duration / 60) % 60, Duration % 60);    // Convierte la duración actual a formato de tiempo (hh:mm:ss o mm:ss).
        QTime TotalTime((mDuration / 3600) % 60, (mDuration / 60) % 60, mDuration % 60);   // Convierte la duración total a formato de tiempo (hh:mm:ss o mm:ss).
        QString Format = mDuration > 3600 ? "hh:mm:ss" : "mm:ss";                          // Establece el formato de hora según si la duración total es mayor a una hora.

        ui->label_current_Time->setText(CurrentTime.toString(Format));   // Actualiza la etiqueta de tiempo actual con el formato adecuado.
        ui->label_Total_Time->setText(TotalTime.toString(Format));       // Actualiza la etiqueta de tiempo total con el formato adecuado.
    }
}

// Método para abrir un archivo de video o audio desde el explorador
void MainWindow::on_actionOpen_triggered()
{
    QString FileName = QFileDialog::getOpenFileName(this, tr("Seleccione Un video"), "", tr("MP4 Files (*.mp4), MP3 Files (*.mp3)"));

    if (FileName.isEmpty()) {
        return;
    }

    QFileInfo fileInfo(FileName);
    QString displayName = fileInfo.fileName();

    if (FileName.endsWith(".mp3")) {

        BackgroundPlayer->setSource(QUrl("qrc:/imagenes/nexosbackgroud.mp4"));

        BackgroundVideo->setVisible(true);
        BackgroundPlayer->play();

        Player->setSource(QUrl::fromLocalFile(FileName));
        Player->play();
    } else {
        // Reproducción de un archivo de video
        BackgroundVideo->setVisible(false);
        Video = new QVideoWidget();
        Video->setGeometry(5, 5, ui->groupBox_Video->width() - 10, ui->groupBox_Video->height() - 10);
        Video->setParent(ui->groupBox_Video);
        Player->setVideoOutput(Video);
        Player->setSource(QUrl::fromLocalFile(FileName));
        Video->setVisible(true);
        Video->show();
    }

    // Mostrar marquee para el archivo abierto directamente
    updateFloatingLabel(displayName);
}
// Control del botón de Play/Pause
void MainWindow::on_pushButton_Play_Pause_clicked()
{
    if (IS_Pause) {
        IS_Pause = false;
        Player->play();
        ui->pushButton_Play_Pause->setIcon(QIcon(":/imagenes/pausa.png"));
    } else {
        IS_Pause = true;
        Player->pause();
        ui->pushButton_Play_Pause->setIcon(QIcon(":/imagenes/play.png"));
    }

    // Asegúrate de que el video de fondo se muestre correctamente
    if (Player->mediaStatus() == QMediaPlayer::PlayingState) {
        QString currentFile = playlist[currentIndex]; // Asegúrate de que currentIndex esté actualizado
        if (currentFile.endsWith(".mp3")) {
            BackgroundPlayer->setSource(QUrl("qrc:/imagenes/nexosbackgroud.mp4"));

            BackgroundVideo->setVisible(true);
            BackgroundPlayer->play();
        } else {
            BackgroundPlayer->stop();
            BackgroundVideo->setVisible(false);
        }
    }
}

// Control del botón de Stop
void MainWindow::on_pushButton_Stop_clicked()
{
    // Detener la reproducción
    Player->stop();

    // Detener el marquee
    if (m_marqueeTimer) {
        m_marqueeTimer->stop();
    }

    // Ocultar el label flotante
    if (m_floatingLabel) {
        m_floatingLabel->hide();
    }

    // Resetear el estado de reproducción
    IS_Pause = true;
    ui->pushButton_Play_Pause->setIcon(QIcon(":/imagenes/play.png"));

    // Detener el video de fondo
    BackgroundPlayer->stop();
    BackgroundVideo->setVisible(false);
}
// Control del volumen
void MainWindow::on_horizontalSlider_Volume_valueChanged(int value)
{
    audioOutput->setVolume(value / 100.0);
    if (value == 0) {
        IS_Muted = true;
        ui->pushButton_Volume->setIcon(QIcon(":/imagenes/mute.png")); // Cambia a mute
    } else {
        IS_Muted = false;
        ui->pushButton_Volume->setIcon(QIcon(":/imagenes/sound.png")); // Cambia a sonido
    }
}
void MainWindow::on_horizontalSlider_Duration_valueChanged(int value)
{
    Player->setPosition(value * 1000);
}
// Avance en el video (seek forward)
void MainWindow::on_pushButton_Seek_Backward_clicked()
{
    ui->horizontalSlider_Duration->setValue(ui->horizontalSlider_Duration->value() - 20);
    Player->setPosition(ui->horizontalSlider_Duration->value() * 1000);
}

// Retroceso en el video (seek backward)
void MainWindow::on_pushButton_Seek_Forward_clicked()
{
    ui->horizontalSlider_Duration->setValue(ui->horizontalSlider_Duration->value() + 20);
    Player->setPosition(ui->horizontalSlider_Duration->value() * 1000);
}

void MainWindow::initializeVideoWidget() {
    BackgroundVideo->setGeometry(5, 5, ui->groupBox_Video->width() - 10, ui->groupBox_Video->height() - 10);
    BackgroundVideo->setVisible(false); // Inicialmente no visible
    Player->setVideoOutput(BackgroundVideo); // Asignar el video widget al reproductor
}


void MainWindow::onMediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    if (status == QMediaPlayer::EndOfMedia) {
        // Mostrar de nuevo el archivo actual en la lista
        for (int i = 0; i < fileList->count(); ++i) {
            QListWidgetItem* item = fileList->item(i);
            if (!item->isHidden()) {
                item->setHidden(false);
                break;
            }
        }

        // Ir al siguiente archivo
        on_pushButton_Next_clicked();
    }

    // Actualizar el botón de Play/Pause según el estado del reproductor
    if (Player->mediaStatus() == QMediaPlayer::PlayingState) {
        ui->pushButton_Play_Pause->setIcon(QIcon(":/imagenes/pausa.png"));
        IS_Pause = false; // Asegúrate de que el estado de pausa sea correcto
    } else if (Player->mediaStatus() == QMediaPlayer::PausedState) {
        ui->pushButton_Play_Pause->setIcon(QIcon(":/imagenes/play.png"));
        IS_Pause = true; // Asegúrate de que el estado de pausa sea correcto
    }
}
void MainWindow::increaseVolume() {
    int currentVolume = ui->horizontalSlider_Volume->value();
    if (currentVolume < 100) {
        currentVolume += 5; // Aumenta el volumen en 5 unidades
        ui-> horizontalSlider_Volume->setValue(currentVolume);
        audioOutput->setVolume(currentVolume / 100.0);
    }
}

void MainWindow::decreaseVolume() {
    int currentVolume = ui->horizontalSlider_Volume->value();
    if (currentVolume > 0) {
        currentVolume -= 5; // Disminuye el volumen en 5 unidades
        ui->horizontalSlider_Volume->setValue(currentVolume);
        audioOutput->setVolume(currentVolume / 100.0);
    }
}

void MainWindow::initializePlaylistIndex()
{
    if (playlist.isEmpty()) {
        currentIndex = -1;
        return;
    }

    // Buscar el primer archivo válido
    for (int i = 0; i < playlist.size(); ++i) {
        QFileInfo fileInfo(playlist[i]);
        if (fileInfo.exists()) {
            currentIndex = i;
            return;
        }
    }

    // Si no se encontró ningún archivo válido
    currentIndex = -1;
}



void MainWindow::playFile(const QString& filePath)
{
    if (filePath.isEmpty()) return;

    // Limpiar configuraciones anteriores
    if (Video) {
        delete Video;
        Video = nullptr;
    }

    // Ocultar todos los archivos visibles antes de mostrar el nuevo
    for (int i = 0; i < fileList->count(); ++i) {
        fileList->item(i)->setHidden(false);
    }

    // Encontrar el item correspondiente al archivo
    QString displayName;
    for (auto it = fileMap.begin(); it != fileMap.end(); ++it) {
        if (it.value() == filePath) {
            displayName = it.key(); // Usar la clave completa que incluye el número
            break;
        }
    }

    // Ocultar el archivo actual de la lista
    QList<QListWidgetItem*> items = fileList->findItems(displayName.split(". ").last(), Qt::MatchExactly);
    if (!items.isEmpty()) {
        QListWidgetItem* currentItem = items.first();
        currentItem->setHidden(true);

        // Mostrar label flotante con el nombre del archivo
        updateFloatingLabel(displayName);
    }

    // Actualizar selección en la lista
    fileList->setCurrentRow(currentIndex);

    if (filePath.endsWith(".mp3")) {
        // Configuración para audio
        BackgroundPlayer->setSource(QUrl("qrc:/imagenes/nexosbackgroud.mp4"));

        // Asegurarse de que el video de fondo esté configurado correctamente
        BackgroundPlayer->setVideoOutput(BackgroundVideo);
        BackgroundVideo->setParent(ui->groupBox_Video);
        BackgroundVideo->setGeometry(5, 5, ui->groupBox_Video->width() - 10, ui->groupBox_Video->height() - 10);

        BackgroundVideo->setVisible(true);
        BackgroundPlayer->play();

        Player->setSource(QUrl::fromLocalFile(filePath));
        Player->play();
    } else {
        // Configuración para video
        BackgroundPlayer->stop();
        BackgroundVideo->setVisible(false);

        Video = new QVideoWidget(ui->groupBox_Video);
        Video->setGeometry(5, 5, ui->groupBox_Video->width() - 10, ui->groupBox_Video->height() - 10);
        Player->setVideoOutput(Video);
        Video->show();

        Player->setSource(QUrl::fromLocalFile(filePath));
        Player->play();
    }
}

QString MainWindow::findBackgroundVideo() {
    BackgroundPlayer->setSource(QUrl("qrc:/imagenes/nexosbackgroud.mp4"));
}

void MainWindow::clearPlaylist() {
    if (playlist.isEmpty()) {
        QMessageBox::information(this, "Información", "No hay archivos que limpiar");
        return;
    }

    // Crear un cuadro de diálogo de confirmación
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Limpiar Lista");
    msgBox.setText("¿Está seguro que desea limpiar la lista de reproducción?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);

    // Personalizar el texto de los botones
    QAbstractButton *yesButton = msgBox.button(QMessageBox::Yes);
    yesButton->setText("Sí");

    QAbstractButton *noButton = msgBox.button(QMessageBox::No);
    noButton->setText("No");

    msgBox.exec(); // Mostrar el cuadro de diálogo

    if (msgBox.clickedButton() == yesButton) {
        // Detener la reproducción
        Player->stop();
        BackgroundPlayer->stop();

        // Limpiar la lista
        fileList->clear();
        fileMap.clear();
        playlist.clear();

        // Resetear índices y estado
        currentIndex = -1;
        BackgroundVideo->setVisible(false);

        // Resetear labels y slider
        ui->label_current_Time->setText("00:00:00");
        ui->label_Total_Time->setText("00:00:00");
        ui->horizontalSlider_Duration->setValue(0);
    }
}
void MainWindow::createFloatingLabel()
{
    // Crear el label flotante
    m_floatingLabel = new QLabel(this);
    m_floatingLabel->setStyleSheet(
        "QLabel {"
        "   background-color: transparent;" // Fondo completamente transparente
        "   color: white;"
        "   padding: 10px;"
        "   border-radius: 10px;"
        "   font-size: 16px;"
        "}"
        );
    m_floatingLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_floatingLabel->setAttribute(Qt::WA_TranslucentBackground); // Hacer el fondo completamente transparente
    m_floatingLabel->hide();

    // Crear timer para el efecto marquee
    m_marqueeTimer = new QTimer(this);
    connect(m_marqueeTimer, &QTimer::timeout, this, &MainWindow::updateMarqueeText);
}
void MainWindow::updateFloatingLabel(const QString& fullKey)
{
    if (!m_floatingLabel) {
        createFloatingLabel();
    }

    // Coordenadas basadas en el diseño del UI
    int labelWidth = 350;  // Ancho especificado
    int labelHeight = 40;  // Altura

    // Coordenadas exactas
    int labelX = 500;  // X
    int labelY = 580;  // Y

    m_floatingLabel->setGeometry(labelX, labelY, labelWidth, labelHeight);
    m_floatingLabel->setStyleSheet(
        "QLabel {"
        "   background-color: rgba(0, 0, 0, 180);" // Fondo semi-transparente
        "   color: white;"
        "   padding: 5px;"
        "   border-radius: 10px;"
        "   font-size: 14px;"
        "}"
        );
    m_floatingLabel->setAlignment(Qt::AlignCenter);
    m_floatingLabel->show();

    // Extraer solo el nombre del archivo del fullKey
    m_currentFileName = fullKey.split(". ").last();
    m_scrollPosition = 0;

    // Iniciar el timer para el efecto marquee
    m_marqueeTimer->start(100); // Actualizar cada 100 ms
}
void MainWindow::updateMarqueeText()
{
    if (!m_floatingLabel || m_currentFileName.isEmpty()) return;

    // Preparar el texto para el marquee
    QString displayText = m_currentFileName + "     " + m_currentFileName;

    // Ajustar el corte de texto al ancho del label
    int visibleChars = m_floatingLabel->width() / 6; // Reducir el divisor para más rapidez
    QString shownText = displayText.mid(m_scrollPosition, visibleChars);
    m_floatingLabel->setText(shownText);

    // Incrementar la posición de desplazamiento
    m_scrollPosition++;

    // Reiniciar si se ha completado el ciclo
    if (m_scrollPosition > m_currentFileName.length() + 5) {
        m_scrollPosition = 0;
    }
}
void MainWindow::addFile()
{
    QFileDialog dialog(this);
    dialog.setWindowTitle(tr("Seleccionar archivos o carpetas"));
    dialog.setFileMode(QFileDialog::ExistingFiles);
    dialog.setNameFilter(tr("Archivos multimedia (*.mp3 *.mp4)"));
    dialog.setViewMode(QFileDialog::Detail);
    dialog.setDirectory(QDir::homePath());

    dialog.resize(800, 600);

    QStringList fileNames;
    QPushButton* folderButton = new QPushButton("Seleccionar Carpeta", &dialog);
    connect(folderButton, &QPushButton::clicked, [&]() {
        QString dir = QFileDialog::getExistingDirectory(&dialog, "Seleccionar Carpeta", QDir::homePath());
        if (!dir.isEmpty()) {
            QDir directory(dir);
            QStringList filters;
            filters << "*.mp3" << "*.mp4";
            QStringList files = directory.entryList(filters, QDir::Files | QDir::NoDotAndDotDot, QDir::Name);

            foreach(QString file, files) {
                fileNames << directory.absoluteFilePath(file);
            }
            dialog.close();
        }
    });

    QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(dialog.layout());
    if (layout) {
        layout->addWidget(folderButton);
    }

    if (dialog.exec() == QDialog::Accepted) {
        fileNames.append(dialog.selectedFiles());
    }

    // Crear un mapa temporal para ordenar
    QMap<int, QPair<QString, QString>> numberedFiles;
    int nextNumber = fileList->count() + 1;

    // Procesar los archivos seleccionados
    for (const QString& fileName : fileNames) {
        QFileInfo fileInfo(fileName);

        // Usar el siguiente número disponible
        numberedFiles[nextNumber] = qMakePair(fileInfo.fileName(), fileName);
        nextNumber++;
    }

    // Agregar los archivos a la lista existente
    for (auto it = numberedFiles.begin(); it != numberedFiles.end(); ++it) {
        int number = it.key();
        QString originalFileName = it.value().first;
        QString fullFileName = it.value().second;

        // Almacenar el número internamente, pero mostrar solo el nombre del archivo
        fileList->addItem(originalFileName);

        // Crear una clave que incluya el número para mantener la lógica interna
        QString internalKey = QString("%1. %2").arg(number).arg(originalFileName);
        fileMap[internalKey] = fullFileName;
        playlist.append(fullFileName);
    }

    // Si no hay reproducción actual, reproducir el primer archivo
    if (Player->mediaStatus() != QMediaPlayer::PlayingState && !playlist.isEmpty()) {
        currentNumber = 1;
        playFile(playlist.first());
    }
}

void MainWindow::onFileSelected(QListWidgetItem *item)
{
    QString displayName = item->text();

    // Buscar la clave completa en el mapa que coincida con el nombre del archivo
    QString fullKey;
    for (auto it = fileMap.begin(); it != fileMap.end(); ++it) {
        if (it.key().endsWith(displayName)) {
            fullKey = it.key();
            break;
        }
    }

    QString fullFileName = fileMap.value(fullKey);

    if (!fullFileName.isEmpty()) {
        // Extraer el número del principio de la clave interna
        currentNumber = fullKey.split(".")[0].toInt();
        playFile(fullFileName);
    }
}

void MainWindow::on_pushButton_Next_clicked()
{

    if (playlist.isEmpty()) {
        QMessageBox::information(this, "Información", "Primero añada archivos a la lista");
        return;
    }

    // Incrementar el número actual
    currentNumber++;

    // Si supera el número máximo, volver al primero
    if (currentNumber > fileList->count()) {
        currentNumber = 1;
    }

    // Encontrar el archivo correspondiente al número actual
    QString searchName = QString("%1.").arg(currentNumber);

    for (auto it = fileMap.begin(); it != fileMap.end(); ++it) {
        if (it.key().startsWith(searchName)) {
            // Encontrar el ítem correspondiente en la lista
            QString fileName = it.key().split(". ").last();
            QList<QListWidgetItem*> items = fileList->findItems(fileName, Qt::MatchExactly);

            if (!items.isEmpty()) {
                fileList->setCurrentItem(items.first());
                playFile(it.value());
                break;
            }
        }
    }
}

void MainWindow::on_pushButton_Previous_clicked()
{
    if (playlist.isEmpty()) {
        QMessageBox::information(this, "Información", "Primero añada archivos a la lista");
        return;
    }

    // Decrementar el número actual
    currentNumber--;

    // Si es menor que 1, ir al último
    if (currentNumber < 1) {
        currentNumber = fileList->count();
    }

    // Encontrar el archivo correspondiente al número actual
    QString searchName = QString("%1.").arg(currentNumber);

    for (auto it = fileMap.begin(); it != fileMap.end(); ++it) {
        if (it.key().startsWith(searchName)) {
            // Encontrar el ítem correspondiente en la lista
            QString fileName = it.key().split(". ").last();
            QList<QListWidgetItem*> items = fileList->findItems(fileName, Qt::MatchExactly);

            if (!items.isEmpty()) {
                fileList->setCurrentItem(items.first());
                playFile(it.value());
                break;
            }
        }
    }
}
void MainWindow::savePlaylist()
{
    QSettings settings("Nexos Media", "NEXOS");

    // Guardar número de archivos
    settings.setValue("playlistCount", playlist.size());

    // Guardar cada ruta de archivo
    for (int i = 0; i < playlist.size(); ++i) {
        settings.setValue(QString("playlist_file_%1").arg(i), playlist[i]);
    }
}

void MainWindow::loadSavedPlaylist()
{
    QSettings settings("Nexos Media", "NEXOS");

    // Obtener número de archivos
    int count = settings.value("playlistCount", 0).toInt();

    // Cargar cada archivo
    for (int i = 0; i < count; ++i) {
        QString filePath = settings.value(QString("playlist_file_%1").arg(i)).toString();

        // Verificar si el archivo existe antes de agregarlo
        if (QFileInfo::exists(filePath)) {
            addFileToPlaylist(filePath);
        }
    }
}
void MainWindow::addFileToPlaylist(const QString& filePath)
{
    QFileInfo fileInfo(filePath);

    // Verificar si el archivo ya está en la lista
    for (const QString& existingPath : playlist) {
        if (QFileInfo(existingPath) == fileInfo) {
            return; // Ya existe, no agregar duplicados
        }
    }

    // Agregar a la lista
    int nextNumber = fileList->count() + 1;
    QString internalKey = QString("%1. %2").arg(nextNumber).arg(fileInfo.fileName());

    fileList->addItem(fileInfo.fileName());
    fileMap[internalKey] = filePath;
    playlist.append(filePath);
}
void MainWindow::closeEvent(QCloseEvent *event)
{
    if (!playlist.isEmpty()) {
        // Crear un cuadro de diálogo de confirmación
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("NEXOS - Guardar Lista");
        msgBox.setText("¿Desea guardar la lista de reproducción actual?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);

        // Personalizar el texto de los botones
        QAbstractButton *yesButton = msgBox.button(QMessageBox::Yes);
        yesButton->setText("Sí");

        QAbstractButton *noButton = msgBox.button(QMessageBox::No);
        noButton->setText("No");

        msgBox.exec(); // Mostrar el cuadro de diálogo

        if (msgBox.clickedButton() == yesButton) {
            savePlaylist(); // Solo guarda si el usuario elige "Sí"
        } else {
            // Limpiar la lista si el usuario elige "No"
            clearPlaylist(); // Llama a tu método para limpiar la lista
        }
    }

    // Llamar al método de la clase padre para manejar el cierre
    event->accept(); // Aceptar el evento de cierre
}
