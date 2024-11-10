#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , Player(nullptr)
    , BackgroundPlayer(nullptr)
    , Video(nullptr)
    , BackgroundVideo(nullptr)
    , audioOutput(nullptr)
    , IS_Pause(false)
    , IS_Muted(false)
    , mDuration(0)
    , fileList(nullptr)
    , currentNumber(0)
    , currentIndex(-1)
    , m_floatingLabel(nullptr)
    , m_marqueeTimer(nullptr)
    , m_scrollPosition(0)
{
    ui->setupUi(this);
    resize(1100, 630);

    // Inicialización del reproductor de medios y configuración de salida de audio
    Player = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);
    Player->setAudioOutput(audioOutput);

    // Inicializar el QVideoWidget una vez
    BackgroundVideo = new QVideoWidget(this);
    initializeVideoWidget();

    // Creación del panel lateral (Dock) para la lista de archivos multimedia
    QDockWidget *dock = new QDockWidget(tr("Lista de archivos"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    // Widget contenedor para la lista y el botón
    QWidget *dockContents = new QWidget();
    QVBoxLayout *dockLayout = new QVBoxLayout(dockContents);

    // Lista de archivos
    fileList = new QListWidget();
    fileList->setDragEnabled(true); // Permitir arrastrar
    fileList->setDropIndicatorShown(true); // Mostrar el indicador de caída
    fileList->setAcceptDrops(true); // Aceptar caídas

    // Botón para añadir archivos
    QPushButton *addFileButton = new QPushButton("Añadir archivo");
    connect(addFileButton, &QPushButton::clicked, this, &MainWindow::addFile);

    // Añadir widgets al layout
    dockLayout->addWidget(fileList);
    dockLayout->addWidget(addFileButton);

    // Conectar selección de archivo a la reproducción correspondiente
    connect(fileList, &QListWidget::itemClicked, this, &MainWindow::onFileSelected);

    // Configurar el widget del dock
    dock->setWidget(dockContents);
    addDockWidget(Qt::LeftDockWidgetArea, dock);

    // Inicialización del reproductor y widget de video de fondo
    BackgroundPlayer = new QMediaPlayer(this);
    BackgroundVideo = new QVideoWidget(ui->groupBox_Video);
    BackgroundVideo->setGeometry(5, 5, ui->groupBox_Video->width() - 10, ui->groupBox_Video->height() - 10);
    BackgroundPlayer->setVideoOutput(BackgroundVideo);
    BackgroundVideo->setVisible(false);

    // Configuración para reproducción en bucle del video de fondo
    connect(BackgroundPlayer, &QMediaPlayer::mediaStatusChanged, this, [=](QMediaPlayer::MediaStatus status) {
        if (status == QMediaPlayer::EndOfMedia) {
            BackgroundPlayer->play();
        }
    });

    // Asignación de iconos a los botones del reproductor
    ui->pushButton_Play_Pause->setIcon(QIcon(":/imagenes/play.png"));
    ui->pushButton_Stop->setIcon(QIcon(":/imagenes/stop.png"));
    ui->pushButton_Seek_Backward->setIcon(QIcon(":/imagenes/retroceder.png"));
    ui->pushButton_Seek_Forward->setIcon(QIcon(":/imagenes/avanzar.png"));
    ui->pushButton_Volume->setIcon(QIcon(":/imagenes/sound.png"));
    ui->pushButton_Next->setIcon(QIcon(":/imagenes/next.png"));
    ui->pushButton_Previous->setIcon(QIcon(":/imagenes/prev.png"));

    // Configuración inicial de volumen
    ui->horizontalSlider_Volume->setMinimum(0);
    ui->horizontalSlider_Volume->setMaximum(100);
    ui->horizontalSlider_Volume->setValue(30);
    audioOutput->setVolume(ui->horizontalSlider_Volume->value() / 100.0);

    // Conexión de las señales del reproductor
    connect(Player, &QMediaPlayer::durationChanged, this, &MainWindow::durationChanged);
    connect(Player, &QMediaPlayer::positionChanged, this, &MainWindow::positionChanged);
    connect(Player, &QMediaPlayer::errorOccurred, this, &MainWindow::onMediaError);
    connect(Player, &QMediaPlayer::mediaStatusChanged, this, &MainWindow::onMediaStatusChanged);

    // Atajos de teclado
    QShortcut *playPauseShortcut = new QShortcut(QKeySequence("Space"), this);
    connect(playPauseShortcut, &QShortcut::activated, this, &MainWindow::on_pushButton_Play_Pause_clicked);

    QShortcut *stopShortcut = new QShortcut(QKeySequence("Ctrl+S"), this);
    connect(stopShortcut, &QShortcut::activated, this, &MainWindow::on_pushButton_Stop_clicked);

    QShortcut *muteShortcut = new QShortcut(QKeySequence("M"), this);
    connect(muteShortcut, &QShortcut::activated, this, &MainWindow::on_pushButton_Volume_clicked);

    QShortcut *seekForwardShortcut = new QShortcut(QKeySequence("Right"), this);
    connect(seekForwardShortcut, &QShortcut::activated, this, &MainWindow::on_pushButton_Seek_Forward_clicked);

    QShortcut *seekBackwardShortcut = new QShortcut(QKeySequence("Left"), this);
    connect(seekBackwardShortcut, &QShortcut::activated, this, &MainWindow::on_pushButton_Seek_Backward_clicked);

    QShortcut *increaseVolumeShortcut = new QShortcut(QKeySequence("Up"), this);
    connect(increaseVolumeShortcut, &QShortcut::activated, this, &MainWindow::increaseVolume);

    QShortcut *decreaseVolumeShortcut = new QShortcut(QKeySequence("Down"), this);
    connect(decreaseVolumeShortcut, &QShortcut::activated, this, &MainWindow::decreaseVolume);

    // Desactivar características de desacople
    dock->setFeatures(QDockWidget::NoDockWidgetFeatures);

    // Agregar botones para mover arriba y abajo en la lista
    QPushButton *moveUpButton = new QPushButton("Mover Arriba", dockContents);
    QPushButton *moveDownButton = new QPushButton("Mover Abajo", dockContents);
    dockLayout->addWidget(moveUpButton);
    dockLayout->addWidget(moveDownButton);
    connect(moveUpButton, &QPushButton::clicked, this, &MainWindow::moveItemUp);
    connect(moveDownButton, &QPushButton::clicked, this, &MainWindow::moveItemDown);

    connect(ui->pushButton_Next, &QPushButton::clicked, this, &MainWindow::on_pushButton_Next_clicked);
    connect(ui->pushButton_Previous, &QPushButton::clicked, this, &MainWindow::on_pushButton_Previous_clicked);

    QString backgroundVideoPath = findBackgroundVideo();
    if (!backgroundVideoPath.isEmpty()) {
        BackgroundPlayer->setSource(QUrl::fromLocalFile(backgroundVideoPath));
    } else {
        QMessageBox::warning(this, "Advertencia",
                             "No se pudo encontrar el video de fondo predeterminado. "
                             "La reproducción de audio no mostrará video de fondo.");
    }

    QPushButton *clearPlaylistButton = new QPushButton("Limpiar Lista", dockContents);
    dockLayout->addWidget(clearPlaylistButton);
    connect(clearPlaylistButton, &QPushButton::clicked, this, &MainWindow::clearPlaylist);

    // Atajo para limpiar la lista
    QShortcut *clearPlaylistShortcut = new QShortcut(QKeySequence("Ctrl+L"), this);
    connect(clearPlaylistShortcut, &QShortcut::activated, this, &MainWindow::clearPlaylist);
      createFloatingLabel();
     setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);
      loadSavedPlaylist();



      setWindowTitle("N E X O S");
 QSettings settings("Nexos Media", "NEXOS");

}





MainWindow::~MainWindow()
{
    // Guardar la lista antes de cerrar
    savePlaylist();

    // Resto del código de limpieza existente
    if (m_marqueeTimer) {
        m_marqueeTimer->stop();
        delete m_marqueeTimer;
    }

    if (m_floatingLabel) {
        delete m_floatingLabel;
    }
    if (m_labelAnimation) {
        delete m_labelAnimation;
    }
    delete ui;
}

void MainWindow::onMediaError(QMediaPlayer::Error error)
{
    QString errorMessage;

    switch (error) {
    case QMediaPlayer::NoError:
        return; // No hay error
    case QMediaPlayer::ResourceError:
        errorMessage = tr("Error de recurso: No se puede acceder al archivo.");
        break;
    case QMediaPlayer::FormatError:
        errorMessage = tr("Error de formato: El archivo no es compatible.");
        break;
    default:
        errorMessage = tr("Error desconocido.");
        break;
    }

    QMessageBox::warning(this, tr("Error de Reproducción"), errorMessage);
}


void MainWindow::on_pushButton_Volume_clicked()
{
    IS_Muted = !IS_Muted; // Cambia el estado de silencio
    audioOutput->setVolume(IS_Muted ? 0 : ui->horizontalSlider_Volume->value() / 100.0);
    ui->pushButton_Volume->setIcon(IS_Muted ? QIcon(":/imagenes/mute.png") : QIcon(":/imagenes/sound.png"));
}

// Método que actualiza la duración máxima del video en el deslizador de progreso
void MainWindow::durationChanged(qint64 duration)
{
    mDuration = duration / 1000;
    ui->horizontalSlider_Duration->setMaximum(mDuration);
}

// Método que actualiza la posición del video en el deslizador de progreso
void MainWindow::positionChanged(qint64 duration)
{
    // Solo actualizar el slider si no está siendo arrastrado
    if (!ui->horizontalSlider_Duration->isSliderDown())
    {
        ui->horizontalSlider_Duration->blockSignals(true);  // Bloquear señales temporalmente
        ui->horizontalSlider_Duration->setValue(duration / 1000);
        ui->horizontalSlider_Duration->blockSignals(false); // Desbloquear señales
    }

    // Actualizar las etiquetas de tiempo solo cada segundo completo
    static qint64 lastSecond = -1;
    qint64 currentSecond = duration / 1000;
    if (currentSecond != lastSecond)
    {
        updateDuration(currentSecond);
        lastSecond = currentSecond;
    }
}

// Actualización del tiempo actual y total del video
void MainWindow::updateDuration(qint64 Duration)
{
    if (Duration || mDuration)
    {
        QTime CurrentTime((Duration / 3600) % 60, (Duration / 60) % 60, Duration % 60);
        QTime TotalTime((mDuration / 3600) % 60, (mDuration / 60) % 60, mDuration % 60);
        QString Format = mDuration > 3600 ? "hh:mm:ss" : "mm:ss";

        ui->label_current_Time->setText(CurrentTime.toString(Format));
        ui->label_Total_Time->setText(TotalTime.toString(Format));
    }
}

// Método para abrir un archivo de video o audio desde el explorador
void MainWindow::on_actionOpen_triggered()
{
    QString FileName = QFileDialog::getOpenFileName(this, tr("Seleccione Un video"), "", tr("MP4 Files (*.mp4);;MP3 Files (*.mp3)"));

    if (FileName.isEmpty()) {
        return;
    }

    QFileInfo fileInfo(FileName);
    QString displayName = fileInfo.fileName();

    if (FileName.endsWith(".mp3")) {
        // Reproducir archivo de audio y mostrar video de fondo
        QString applicationDir = QCoreApplication::applicationDirPath();
        QString backgroundVideoPath = applicationDir + "/Video/imagenes/musica.mp4";

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
            QString applicationDir = QCoreApplication::applicationDirPath();
            QString backgroundVideoPath = applicationDir + "/Video/imagenes/musica.mp4";

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

void MainWindow::moveItemUp()
{
    if (playlist.isEmpty()) {
        QMessageBox::information(this, "Información", "Primero añada archivos a la lista");
        return;
    }
    int currentIndex = fileList->currentRow();
    if (currentIndex > 0) {
        QString currentItemText = fileList->item(currentIndex)->text();
        QString previousItemText = fileList->item(currentIndex - 1)->text();
        fileList->item(currentIndex - 1)->setText(currentItemText);
        fileList->item(currentIndex)->setText(previousItemText);
        fileList->setCurrentRow(currentIndex - 1);
    }
}

void MainWindow::moveItemDown()
{
    if (playlist.isEmpty()) {
        QMessageBox::information(this, "Información", "Primero añada archivos a la lista");
        return;
    }
    int currentIndex = fileList->currentRow();
    if (currentIndex < fileList->count() - 1) {
        QString currentItemText = fileList->item(currentIndex)->text();
        QString nextItemText = fileList->item(currentIndex + 1)->text();
        fileList->item(currentIndex + 1)->setText(currentItemText);
        fileList->item(currentIndex)->setText(nextItemText);
        fileList->setCurrentRow(currentIndex + 1);
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
        QString applicationDir = QCoreApplication::applicationDirPath();
        QString backgroundVideoPath = applicationDir + "/Video/imagenes/musica.mp4";

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
    QString applicationDir = QCoreApplication::applicationDirPath();
    QString defaultPath = applicationDir + "/Video/imagenes/musica.mp4";

    QStringList possiblePaths = {
        defaultPath,
        applicationDir + "/../Video/imagenes/musica.mp4",
        applicationDir + "/../../Video/imagenes/musica.mp4",
        QDir::homePath() + "/Desktop/Proyecto final/Video/imagenes/musica.mp4"
    };

    for (const QString& path : possiblePaths) {
        QFileInfo fileInfo(path);
        if (fileInfo.exists()) {
            qDebug() << "Video de fondo encontrado: " << path;
            return path;
        }
    }

    qWarning() << "No se encontró ningún video de fondo";
    return QString();
}

void MainWindow::clearPlaylist() {
    if (playlist.isEmpty()) {
        QMessageBox::information(this, "Información", "No hay archivos que limpiar");
        return;
    }

    // Mostrar un cuadro de diálogo de confirmación
    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "Limpiar Lista",
        "¿Está seguro que desea limpiar la lista de reproducción?",
        QMessageBox::Yes | QMessageBox::No
        );

    if (reply == QMessageBox::Yes) {
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
        QMessageBox::StandardButton reply = QMessageBox::question(
            this,
            "NEXOS - Guardar Lista",
            "¿Desea guardar la lista de reproducción actual?",
            QMessageBox::Yes | QMessageBox::No
            );

        if (reply == QMessageBox::Yes) {
            savePlaylist();
        }
    }

    // Llamar al método de la clase padre para manejar el cierre
    QMainWindow::closeEvent(event);
}
