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
void MainWindow::on_actionOpen_triggered()    // Este método se activa cuando el usuario selecciona la opción para abrir un archivo.
{
    QString FileName = QFileDialog::getOpenFileName(this, tr("Seleccione Un video"), "", tr("MP4 Files (*.mp4), MP3 Files (*.mp3)")); /* Muestra un cuadro de diálogo para seleccionar
    un archivo de video o audio (.mp4 o .mp3). */

    if (FileName.isEmpty()) {     // Si el usuario no seleccionó ningún archivo (se canceló la acción)...
        return;                   // Salir del método sin hacer nada más.
    }

    QFileInfo fileInfo(FileName);               // Crea un objeto QFileInfo con la ruta del archivo seleccionado.
    QString displayName = fileInfo.fileName();  // Obtiene el nombre del archivo para mostrarlo en la interfaz de usuario.

    if (FileName.endsWith(".mp3")) {        // Si el archivo seleccionado es de tipo .mp3...

        BackgroundPlayer->setSource(QUrl("qrc:/imagenes/nexosbackgroud.mp4"));  // Asigna un video de fondo predeterminado al BackgroundPlayer.

        BackgroundVideo->setVisible(true);    // Hace visible el video de fondo.
        BackgroundPlayer->play();            // Reproduce el video de fondo.

        Player->setSource(QUrl::fromLocalFile(FileName));   // Asigna el archivo .mp3 como fuente del reproductor de audio.
        Player->play();    // Reproduce el archivo de audio.
    } else {       // Si el archivo es un video .mp4...
        // Reproducción de un archivo de video
        BackgroundVideo->setVisible(false);   // Oculta el video de fondo.
        Video = new QVideoWidget();           // Crea un nuevo widget de video.
        Video->setGeometry(5, 5, ui->groupBox_Video->width() - 10, ui->groupBox_Video->height() - 10);  // Establece la geometría del widget de video dentro de un grupo en la interfaz.
        Video->setParent(ui->groupBox_Video);     // Establece el widget de video como hijo del contenedor groupBox_Video.
        Player->setVideoOutput(Video);            // Asigna el widget de video como la salida del reproductor de medios.
        Player->setSource(QUrl::fromLocalFile(FileName));     // Asigna el archivo de video como fuente del reproductor de medios.
        Video->setVisible(true);    // Hace visible el widget de video.
        Video->show();              // Muestra el widget de video.
    }

    // Mostrar marquee para el archivo abierto directamente
    updateFloatingLabel(displayName);     // Llama a updateFloatingLabel() para mostrar el nombre del archivo de manera flotante en la interfaz
}
// Control del botón de Play/Pause                     
void MainWindow::on_pushButton_Play_Pause_clicked()    // Este método se activa cuando el usuario hace clic en el botón de Play/Pause.
{
    if (IS_Pause) {            // Si el estado es de pausa...
        IS_Pause = false;      // Cambia el estado a reproducción.
        Player->play();        // Reanuda la reproducción.
        ui->pushButton_Play_Pause->setIcon(QIcon(":/imagenes/pausa.png"));   // Cambia el icono del botón a "pausa".
    } else {              // Si el estado no está en pausa (está reproduciendo)...
        IS_Pause = true;  // Cambia el estado a pausa.
        Player->pause();  // Pausa la reproducción.
        ui->pushButton_Play_Pause->setIcon(QIcon(":/imagenes/play.png"));   // Cambia el icono del botón a "play".
    }

    // Asegurar que el video de fondo se muestre correctamente
    if (Player->mediaStatus() == QMediaPlayer::PlayingState) {        // Si el estado del reproductor es "reproduciendo"..
        QString currentFile = playlist[currentIndex]; // Obtiene el archivo actual de la lista de reproducción.
        if (currentFile.endsWith(".mp3")) {           // Si el archivo actual es de tipo .mp3...
            BackgroundPlayer->setSource(QUrl("qrc:/imagenes/nexosbackgroud.mp4"));  / Establece un video de fondo predeterminado.

            BackgroundVideo->setVisible(true);   // Muestra el video de fondo.
            BackgroundPlayer->play();            // Reproduce el video de fondo.
        } else {                                 // Si el archivo actual no es de tipo .mp3 (es un archivo de video)...
            BackgroundPlayer->stop();            // Detiene la reproducción del video de fondo.
            BackgroundVideo->setVisible(false);  // Oculta el video de fondo.
        }
    }
}

// Control del botón de Stop
void MainWindow::on_pushButton_Stop_clicked()  // Este método se activa cuando el usuario hace clic en el botón de "Stop".
{
    // Detener la reproducción
    Player->stop();         // Detiene la reproducción del archivo de audio o video.

    // Detener el marquee
    if (m_marqueeTimer) {         // Si el temporizador de desplazamiento (marquee) existe...
        m_marqueeTimer->stop();   // Detiene el temporizador.
    }

    // Ocultar el label flotante
    if (m_floatingLabel) {           // Si la etiqueta flotante existe...
        m_floatingLabel->hide();     // Oculta la etiqueta flotante.
    }

    // Resetear el estado de reproducción
    IS_Pause = true;       // Establece el estado de pausa a verdadero.
    ui->pushButton_Play_Pause->setIcon(QIcon(":/imagenes/play.png"));    // Cambia el icono del botón a "play".

    // Detener el video de fondo
    BackgroundPlayer->stop();             // Detiene la reproducción del video de fondo.
    BackgroundVideo->setVisible(false);   // Oculta el video de fondo.
}
// Control del volumen
void MainWindow::on_horizontalSlider_Volume_valueChanged(int value)      // Este método se activa cuando el usuario cambia el valor del deslizador de volumen.
{
    audioOutput->setVolume(value / 100.0);      // Ajusta el volumen del audio según el valor del deslizador (valor entre 0 y 100).
    if (value == 0) {        // Si el volumen es 0 (silenciado)...
        IS_Muted = true;     // Establece el estado de "silenciado" como verdadero.
        ui->pushButton_Volume->setIcon(QIcon(":/imagenes/mute.png"));   // Cambia el icono del botón a "mute".
    } else {                 // Si el volumen no es 0...
        IS_Muted = false;    // Establece el estado de "silenciado" como falso.
        ui->pushButton_Volume->setIcon(QIcon(":/imagenes/sound.png"));  // Cambia el icono del botón a "sound".
    }
}
void MainWindow::on_horizontalSlider_Duration_valueChanged(int value)      // Este método se activa cuando el usuario cambia la posición del deslizador de duración.
{
    Player->setPosition(value * 1000);   // Establece la posición de reproducción en el reproductor, ajustada a la nueva posición del deslizador (en milisegundos)
}
// Retroceso en el video  
void MainWindow::on_pushButton_Seek_Backward_clicked()    // Este método se activa cuando el usuario hace clic en el botón de retroceder (seek backward).
{
    ui->horizontalSlider_Duration->setValue(ui->horizontalSlider_Duration->value() - 20);   // Disminuye la posición del deslizador en 20.
    Player->setPosition(ui->horizontalSlider_Duration->value() * 1000);   // Ajusta la posición de reproducción al nuevo valor del deslizador.
}

// Avance en el video
void MainWindow::on_pushButton_Seek_Forward_clicked()         // Este método se activa cuando el usuario hace clic en el botón de avanzar (seek forward).
{
    ui->horizontalSlider_Duration->setValue(ui->horizontalSlider_Duration->value() + 20);   // Aumenta la posición del deslizador en 20
    Player->setPosition(ui->horizontalSlider_Duration->value() * 1000);      // Ajusta la posición de reproducción al nuevo valor del deslizador
}

void MainWindow::initializeVideoWidget() {            // Este método inicializa el widget de video
    BackgroundVideo->setGeometry(5, 5, ui->groupBox_Video->width() - 10, ui->groupBox_Video->height() - 10);   // Establece la geometría del widget de video dentro del contenedor
    BackgroundVideo->setVisible(false); // Inicialmente, el video de fondo no es visible.
    Player->setVideoOutput(BackgroundVideo); // Asigna el widget de video de fondo al reproductor.
}


void MainWindow::onMediaStatusChanged(QMediaPlayer::MediaStatus status)   // Este método se ejecuta cuando el estado del reproductor de medios cambia.
{
    if (status == QMediaPlayer::EndOfMedia) {                   // Si el estado indica que se ha llegado al final del archivo...
        // Mostrar de nuevo el archivo actual en la lista
        for (int i = 0; i < fileList->count(); ++i) {           // Recorre todos los elementos de la lista de archivos.
            QListWidgetItem* item = fileList->item(i);           // Obtiene un ítem de la lista.
            if (!item->isHidden()) {                            // Si el ítem no está oculto...
                item->setHidden(false);                         // Lo hace visible nuevamente.
                break;                                          // Sale del bucle después de encontrar el primer archivo visible.
            }
        }

        // Ir al siguiente archivo
        on_pushButton_Next_clicked();       // Llama al método para cambiar al siguiente archivo en la lista.
    }

    // Actualizar el botón de Play/Pause según el estado del reproductor
    if (Player->mediaStatus() == QMediaPlayer::PlayingState) {                     // Si el reproductor está reproduciendo...
        ui->pushButton_Play_Pause->setIcon(QIcon(":/imagenes/pausa.png"));         // Cambia el icono del botón a "pausa".
        IS_Pause = false;                                                          // Establece que no está en pausa.
    } else if (Player->mediaStatus() == QMediaPlayer::PausedState) {               // Si el reproductor está en pausa...
        ui->pushButton_Play_Pause->setIcon(QIcon(":/imagenes/play.png"));          // Cambia el icono del botón a "play".
        IS_Pause = true;                                                           // Establece que está en pausa
    }
}
void MainWindow::increaseVolume() {         // Este método aumenta el volumen.
    int currentVolume = ui->horizontalSlider_Volume->value();      // Obtiene el valor actual del deslizador de volumen.
    if (currentVolume < 100) {       // Si el volumen no está al máximo...
        currentVolume += 5; // Aumenta el volumen en 5 unidades
        ui-> horizontalSlider_Volume->setValue(currentVolume);      // Actualiza el valor del deslizador.
        audioOutput->setVolume(currentVolume / 100.0);    // Ajusta el volumen del audio.
    }
}

void MainWindow::decreaseVolume() {       // Este método disminuye el volumen.
    int currentVolume = ui->horizontalSlider_Volume->value();    // Obtiene el valor actual del deslizador de volumen
    if (currentVolume > 0) {  // Si el volumen no está en cero...
        currentVolume -= 5; // Disminuye el volumen en 5 unidades
        ui->horizontalSlider_Volume->setValue(currentVolume);  // Actualiza el valor del deslizador.
        audioOutput->setVolume(currentVolume / 100.0);     // Ajusta el volumen del audio.
    }
}
// Método para inicializar el índice de la lista de reproducción
void MainWindow::initializePlaylistIndex()    // Este método inicializa el índice de la lista de reproducción.
{
    if (playlist.isEmpty()) {      // Si la lista de reproducción está vacía...
        currentIndex = -1;         // Establece el índice como -1 (sin archivo seleccionado).
        return;                    // Sale del método.
    }

    // Buscar el primer archivo válido
    for (int i = 0; i < playlist.size(); ++i) {    // Recorre todos los archivos de la lista de reproducción.
        QFileInfo fileInfo(playlist[i]);           // Obtiene la información del archivo actual.
        if (fileInfo.exists()) {                   // Si el archivo existe...
            currentIndex = i;                      // Establece el índice al archivo válido.
            return;                                 // Sale del método.
        }
    }
    // Si no se encontró ningún archivo válido
    currentIndex = -1;   // Si no se encontró un archivo válido, establece el índice como -1.
}

// Método para reproducir un archivo de la lista
void MainWindow::playFile(const QString& filePath)   // Este método reproduce un archivo dado su path.
{
    if (filePath.isEmpty()) return;         // Si la ruta del archivo está vacía, no hace nada.

    // Limpiar configuraciones anteriores
    if (Video) {          // Si ya existe un widget de video...
        delete Video;     // Elimina el widget de video.
        Video = nullptr;  // Asigna a Video un valor nulo.
    }

    // Ocultar todos los archivos visibles antes de mostrar el nuevo
    for (int i = 0; i < fileList->count(); ++i) {    // Recorre todos los archivos de la lista.
        fileList->item(i)->setHidden(false);          // Asegura que todos los archivos estén visibles
    }

    // Encontrar el item correspondiente al archivo
    QString displayName;         // Variable para almacenar el nombre del archivo a mostrar.
    for (auto it = fileMap.begin(); it != fileMap.end(); ++it) {       // Recorre el mapa de archivos.
        if (it.value() == filePath) {        // Si el valor del archivo coincide con la ruta proporcionada...
            displayName = it.key();   // Usa la clave (nombre del archivo) para mostrarlo.
            break;  //Sale del bucle después de encontrar el archivo.
        }
    }

    // Ocultar el archivo actual de la lista
    QList<QListWidgetItem*> items = fileList->findItems(displayName.split(". ").last(), Qt::MatchExactly);  // Busca el archivo en la lista de elementos.
    if (!items.isEmpty()) {    // Si el archivo se encuentra...
        QListWidgetItem* currentItem = items.first();  // Obtiene el primer ítem encontrado.
        currentItem->setHidden(true);   // Lo oculta en la lista.

        // Mostrar label flotante con el nombre del archivo
        updateFloatingLabel(displayName);   // Muestra el nombre del archivo en una etiqueta flotante.
    }

    // Actualizar selección en la lista
    fileList->setCurrentRow(currentIndex);   // Establece el ítem seleccionado en la lista.

    if (filePath.endsWith(".mp3")) {    // Si el archivo es un archivo de audio (.mp3)...
        // Configuración para audio
        BackgroundPlayer->setSource(QUrl("qrc:/imagenes/nexosbackgroud.mp4"));  // Establece un video de fondo predeterminado.

        // Asegurarse de que el video de fondo esté configurado correctamente
        BackgroundPlayer->setVideoOutput(BackgroundVideo);   // Asocia el reproductor de fondo al widget de video
        BackgroundVideo->setParent(ui->groupBox_Video);      // Establece el grupo de video como contenedor para el widget de video.
        BackgroundVideo->setGeometry(5, 5, ui->groupBox_Video->width() - 10, ui->groupBox_Video->height() - 10);   // Ajusta la geometría del video de fondo.

        BackgroundVideo->setVisible(true);    // Hace visible el video de fondo.
        BackgroundPlayer->play();             // Comienza a reproducir el video de fondo.

        Player->setSource(QUrl::fromLocalFile(filePath));    // Establece el archivo de audio como fuente para el reproductor.
        Player->play();   // Comienza a reproducir el archivo de audio.
    } else {    // Si el archivo es un archivo de video...
        // Configuración para video
        BackgroundPlayer->stop();   // Detiene el video de fondo.
        BackgroundVideo->setVisible(false);   // Hace invisible el video de fondo.
 
        Video = new QVideoWidget(ui->groupBox_Video);    // Crea un nuevo widget de video para mostrar el video.
        Video->setGeometry(5, 5, ui->groupBox_Video->width() - 10, ui->groupBox_Video->height() - 10);   // Ajusta la geometría del widget de video.
        Player->setVideoOutput(Video);    // Asocia el widget de video al reproductor.
        Video->show();     // Muestra el widget de video.

        Player->setSource(QUrl::fromLocalFile(filePath));   // Establece el archivo de video como fuente para el reproductor.
        Player->play();    // Comienza a reproducir el archivo de video.
    }
}

QString MainWindow::findBackgroundVideo() {
    BackgroundPlayer->setSource(QUrl("qrc:/imagenes/nexosbackgroud.mp4"));   // Establece el origen del archivo de video de fondo en el reproductor
}

void MainWindow::clearPlaylist() {    // Si la lista de reproducción está vacía, mostrar mensaje y salir
    if (playlist.isEmpty()) {
        QMessageBox::information(this, "Información", "No hay archivos que limpiar");
        return;
    }

    // Crear un cuadro de diálogo de confirmación
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Limpiar Lista");   // Título de la ventana
    msgBox.setText("¿Está seguro que desea limpiar la lista de reproducción?");    // Texto de confirmación
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);         // Botones de Sí y No

    // Personalizar el texto de los botones
    QAbstractButton *yesButton = msgBox.button(QMessageBox::Yes);
    yesButton->setText("Sí");   // Cambiar texto del botón "Sí"

    QAbstractButton *noButton = msgBox.button(QMessageBox::No);
    noButton->setText("No");    // Cambiar texto del botón "No"

    msgBox.exec();    // Mostrar el cuadro de diálogo y esperar respuesta


     // Si el usuario hace clic en "Sí"
    if (msgBox.clickedButton() == yesButton) {
        // Detener la reproducción
        Player->stop();   // Detener cualquier reproducción activa
        BackgroundPlayer->stop();

        // Limpiar la lista de archivos, el mapa de archivos y la lista de reproducción
        fileList->clear();
        fileMap.clear();
        playlist.clear();

         // Resetear los índices y el estado de visualización
        currentIndex = -1;
        BackgroundVideo->setVisible(false);

        // Resetear los valores de los labels y el slider
        ui->label_current_Time->setText("00:00:00");   // Tiempo actual a 00:00:00
        ui->label_Total_Time->setText("00:00:00");     // Tiempo total a 00:00:00
        ui->horizontalSlider_Duration->setValue(0);    // Slider de duración a 0
    }
}
void MainWindow::createFloatingLabel()    // Crear un QLabel que funcionará como un label flotante
{ 
    // Crear el label flotante
    m_floatingLabel = new QLabel(this);
    m_floatingLabel->setStyleSheet(
        "QLabel {"
        "   background-color: transparent;" // Fondo completamente transparente
        "   color: white;"                  // Texto blanco
        "   padding: 10px;"                 // Espaciado interno
        "   border-radius: 10px;"           // Bordes redondeados
        "   font-size: 16px;"               // Tamaño de fuente
        "}"
        );
    m_floatingLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);       // Alineación del texto
    m_floatingLabel->setAttribute(Qt::WA_TranslucentBackground); // Hacer el fondo completamente transparente
    m_floatingLabel->hide();      // Ocultar el label inicialmente

   // Crear un timer para el efecto de desplazamiento del texto
    m_marqueeTimer = new QTimer(this);  se crea un objeto QTimer llamado m_marqueeTimer
    connect(m_marqueeTimer, &QTimer::timeout, this, &MainWindow::updateMarqueeText);  // Conectar el timer a la función de actualización del texto
}
void MainWindow::updateFloatingLabel(const QString& fullKey)   
  // Si el label flotante no existe, crearlo
{
    if (!m_floatingLabel) {    
        createFloatingLabel();
    }
  // Definir las dimensiones del label flotante
    int labelWidth = 350;  // Ancho especificado
    int labelHeight = 40;  // Altura

    // Coordenadas para posicionar el label en el UI
    int labelX = 500;  // Posición en el eje X
    int labelY = 580;  // Posición en el eje Y

      // Configurar las coordenadas y dimensiones del label
    m_floatingLabel->setGeometry(labelX, labelY, labelWidth, labelHeight);
      // Aplicar el estilo al label flotante
    m_floatingLabel->setStyleSheet(
        "QLabel {"
        "   background-color: rgba(0, 0, 0, 180);" // Fondo semi-transparente negro
        "   color: white;"          // Texto blanco
        "   padding: 5px;"          // Espaciado interno
        "   border-radius: 10px;"   // Bordes redondeados
        "   font-size: 14px;"        // Tamaño de fuente
        "}"
        );
    m_floatingLabel->setAlignment(Qt::AlignCenter);  // Alineación centrada del texto
    m_floatingLabel->show();      // Mostrar el label

    // Extraer solo el nombre del archivo del fullKey
    m_currentFileName = fullKey.split(". ").last();   // Tomar la última parte después del punto
    m_scrollPosition = 0;           // Resetear la posición de desplazamiento

    // Iniciar el timer para el efecto de desplazamiento
    m_marqueeTimer->start(100); // Actualizar cada 100 milisegundos
}
void MainWindow::updateMarqueeText()
{
    if (!m_floatingLabel || m_currentFileName.isEmpty()) return;  // Si no existe el label flotante o el nombre del archivo está vacío, no hacer nada

     // Preparar el texto que se mostrará en el efecto marquee
    QString displayText = m_currentFileName + "     " + m_currentFileName;     // Duplicar el nombre del archivo para que se desplace

    // Ajustar el corte de texto al ancho del label, cuántos caracteres se pueden mostrar en el label
    int visibleChars = m_floatingLabel->width() / 6; // Dividir el ancho del label entre el tamaño de los caracteres
    QString shownText = displayText.mid(m_scrollPosition, visibleChars);    // Obtener los caracteres visibles del texto
    m_floatingLabel->setText(shownText);          // Actualizar el texto del label flotante

    // Incrementar la posición de desplazamiento
    m_scrollPosition++;

    // Reiniciar si se ha completado el ciclo
    if (m_scrollPosition > m_currentFileName.length() + 5) {
        m_scrollPosition = 0;  // Reiniciar el desplazamiento
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
