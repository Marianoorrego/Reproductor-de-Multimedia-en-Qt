#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    resize(1100, 630);
    // Inicialización del reproductor de medios y configuración de salida de audio
    Player = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);
    Player->setAudioOutput(audioOutput);

    // Creación del panel lateral (Dock) para la lista de archivos multimedia
    QDockWidget *dock = new QDockWidget(tr("Lista de archivos"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    // Widget contenedor para la lista y el botón
    QWidget *dockContents = new QWidget();
    QVBoxLayout *dockLayout = new QVBoxLayout(dockContents);

    // Lista de archivos
    fileList = new QListWidget();

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

    ui->pushButton_Next->setIcon(QIcon(":/imagenes/next.png")); // Icono para el botón Siguiente
    ui->pushButton_Random->setIcon(QIcon(":/imagenes/random.png")); // Icono para el botón Aleatorio

    // Configuración inicial de volumen
    ui->horizontalSlider_Volume->setMinimum(0);
    ui->horizontalSlider_Volume->setMaximum(100);
    ui->horizontalSlider_Volume->setValue(30);
    audioOutput->setVolume(ui->horizontalSlider_Volume->value() / 100.0);

    // Conexión de las señales del reproductor
    connect(Player, &QMediaPlayer::durationChanged, this, &MainWindow::durationChanged);
    connect(Player, &QMediaPlayer::positionChanged, this, &MainWindow::positionChanged);

 connect(fileList, &QListWidget::itemClicked, this, &MainWindow::onFileSelected);

    ui->horizontalSlider_Duration->setRange(0, Player->duration() / 1000);

    connect(Player, &QMediaPlayer::errorOccurred, this, &MainWindow::onMediaError);
    connect(Player, &QMediaPlayer::mediaStatusChanged, this, &MainWindow::onMediaStatusChanged);

    connect(ui->pushButton_Next, &QPushButton::clicked, this, &MainWindow::on_pushButton_Next_clicked);
    connect(ui->pushButton_Random, &QPushButton::clicked, this, &MainWindow::on_pushButton_Random_clicked);

    // Atajo para el botón de Play/Pause
    QShortcut *playPauseShortcut = new QShortcut(QKeySequence("Space"), this);
    connect(playPauseShortcut, &QShortcut::activated, this, &MainWindow::on_pushButton_Play_Pause_clicked);

    // Atajo para el botón de Stop
    QShortcut *stopShortcut = new QShortcut(QKeySequence("Ctrl+S"), this);
    connect(stopShortcut, &QShortcut::activated, this, &MainWindow::on_pushButton_Stop_clicked);

    // Atajo para el botón de Mute/Unmute
    QShortcut *muteShortcut = new QShortcut(QKeySequence("M"), this);
    connect(muteShortcut, &QShortcut::activated, this, &MainWindow::on_pushButton_Volume_clicked);

    // Atajo para avanzar en el video
    QShortcut *seekForwardShortcut = new QShortcut(QKeySequence("Right"), this);
    connect(seekForwardShortcut, &QShortcut::activated, this, &MainWindow::on_pushButton_Seek_Forward_clicked);

    // Atajo para retroceder en el video
    QShortcut *seekBackwardShortcut = new QShortcut(QKeySequence("Left"), this);
    connect(seekBackwardShortcut, &QShortcut::activated, this, &MainWindow::on_pushButton_Seek_Backward_clicked);

    // Atajo para aumentar el volumen
    QShortcut *increaseVolumeShortcut = new QShortcut(QKeySequence("Up"), this);
    connect(increaseVolumeShortcut, &QShortcut::activated, this, &MainWindow::increaseVolume);

    // Atajo para disminuir el volumen
    QShortcut *decreaseVolumeShortcut = new QShortcut(QKeySequence("Down"), this);
    connect(decreaseVolumeShortcut, &QShortcut::activated, this, &MainWindow::decreaseVolume);



    // Agregar botón para mover arriba y abajo en la lista
  QPushButton *moveUpButton = new QPushButton("Mover Arriba", dockContents);
    QPushButton *moveDownButton = new QPushButton("Mover Abajo", dockContents);
    dockLayout->addWidget(moveUpButton);
    dockLayout->addWidget(moveDownButton);
    connect(moveUpButton, &QPushButton::clicked, this, &MainWindow::moveItemUp);
    connect(moveDownButton, &QPushButton::clicked, this, &MainWindow::moveItemDown);
}


MainWindow::~MainWindow()
{
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


void MainWindow::addFile()
{
    QFileDialog dialog(this);
    dialog.setWindowTitle(tr("Seleccionar archivos o carpetas"));
    dialog.setFileMode(QFileDialog::ExistingFiles);
    dialog.setNameFilter(tr("Archivos multimedia (*.mp3 *.mp4)"));
    dialog.setViewMode(QFileDialog::Detail);
    dialog.setDirectory(QDir::homePath());

    // Hacer la ventana más grande
    dialog.resize(800, 600);

    // Agregar botón para seleccionar carpetas
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
            dialog.close();  // Cerramos el diálogo en lugar de usar done()
        }
    });

    QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(dialog.layout());
    if (layout) {
        layout->addWidget(folderButton);
    }

    if (dialog.exec() == QDialog::Accepted) {
        fileNames.append(dialog.selectedFiles());
    }

    // Procesar los archivos seleccionados
    for (const QString& fileName : fileNames) {
        QFileInfo fileInfo(fileName);
        fileList->addItem(fileInfo.fileName());
        fileMap[fileInfo.fileName()] = fileName;
        playlist.append(fileName); // Agregar a la lista de reproducción
    }
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


    if (FileName.endsWith(".mp3")) {
        // Reproducir archivo de audio y mostrar video de fondo
        BackgroundPlayer->setSource(QUrl::fromLocalFile("C:/Users/maria/Desktop/Proyecto final/Video/imagenes/Disco.mp4"));
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
}

// Controla el cambio de la posición del video con el deslizador
void MainWindow::on_horizontalSlider_Duration_valueChanged(int value)
{
    Player->setPosition(value * 1000);
}

// Control del botón de Play/Pause
void MainWindow::on_pushButton_Play_Pause_clicked()
{
    if (IS_Pause == true)
    {
        IS_Pause = false;
        Player->play();
        ui->pushButton_Play_Pause->setIcon(QIcon(":/imagenes/pausa.png"));
    }
    else
    {
        IS_Pause = true;
        Player->pause();
        ui->pushButton_Play_Pause->setIcon(QIcon(":/imagenes/play.png"));
    }
}

// Control del botón de Stop
void MainWindow::on_pushButton_Stop_clicked()
{
    Player->stop();
}



// Control del volumen
void MainWindow::on_horizontalSlider_Volume_valueChanged(int value)
{
    audioOutput->setVolume(value / 100.0);
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

// Manejo de la selección de un archivo desde la lista para su reproducción
void MainWindow::onFileSelected(QListWidgetItem *item)
{
    QString fileName = item->text();
    QString fullFileName = fileMap.value(fileName);

    if (!fullFileName.isEmpty()) {
        currentIndex = fileList->row(item); // Actualizar el índice actual
        playlist.clear(); // Limpiar la lista de reproducción
        for (int i = 0; i < fileList->count(); ++i) {
            playlist.append(fileMap.value(fileList->item(i)->text ())); // Llenar la lista de reproducción
        }

        if (fullFileName.endsWith(".mp4")) {
            BackgroundVideo->setVisible(false);
            Video = new QVideoWidget();
            Video->setGeometry(5, 5, ui->groupBox_Video->width() - 10, ui->groupBox_Video->height() - 10);
            Video->setParent(ui->groupBox_Video);
            Player->setVideoOutput(Video);
            Player->setSource(QUrl::fromLocalFile(fullFileName));
            Video->setVisible(true);
            Player->play();
        } else if (fullFileName.endsWith(".mp3")) {
            BackgroundPlayer->setSource(QUrl::fromLocalFile("C:/Users/maria/Desktop/Proyecto final/Video/imagenes/Disco.mp4"));
            BackgroundVideo->setVisible(true);
            BackgroundPlayer->play();
            Player->setSource(QUrl::fromLocalFile(fullFileName));
            Player->play();
        }
    }
}
void MainWindow::onMediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    if (status == QMediaPlayer::EndOfMedia) {
        // Reproducir el siguiente archivo
        currentIndex++;
        if (currentIndex < playlist.size()) {
            QString nextFile = playlist[currentIndex];
            Player->setSource(QUrl::fromLocalFile(nextFile));

            // Verificar si el siguiente archivo es un audio o un video
            if (nextFile.endsWith(".mp3")) {
                // Reproducir video de fondo
                BackgroundPlayer->setSource(QUrl::fromLocalFile("C:/Users/maria/Desktop/Proyecto final/Video/imagenes/Disco.mp4"));
                BackgroundVideo->setVisible(true);
                BackgroundPlayer->play();
            } else {
                // Detener el video de fondo
                BackgroundPlayer->stop();
                BackgroundVideo->setVisible(false);

                // Configurar y mostrar el video
                Video = new QVideoWidget();
                Video->setGeometry(5, 5, ui->groupBox_Video->width() - 10, ui->groupBox_Video->height() - 10);
                Video->setParent(ui->groupBox_Video);
                Player->setVideoOutput(Video);
                Player->setSource(QUrl::fromLocalFile(nextFile));
                Video->setVisible(true);
                Video->show();
            }

            Player->play();
            fileList->setCurrentRow(currentIndex); // Resaltar el archivo actual en la lista
        } else {
            // Si no hay más archivos, puedes detener la reproducción o reiniciar
            Player->stop();
            currentIndex = -1; // Reiniciar el índice
            BackgroundPlayer->stop(); // Asegúrate de detener el video de fondo
            BackgroundVideo->setVisible(false); // Ocultar el video de fondo
        }
    }
}
void MainWindow::increaseVolume() {
    int currentVolume = ui->horizontalSlider_Volume->value();
    if (currentVolume < 100) {
        currentVolume += 5; // Aumenta el volumen en 5 unidades
        ui->horizontalSlider_Volume->setValue(currentVolume);
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
void MainWindow::on_pushButton_Next_clicked()
{
    // Verificar si la lista de reproducción está vacía
    if (playlist.isEmpty()) {
        QMessageBox::warning(this, tr("Error"), tr("PRIMERO INSERTE ARCHIVOS"));
        return; // Salir de la función si no hay archivos
    }

    if (currentIndex < playlist.size() - 1) {
        currentIndex++;
        QString nextFile = playlist[currentIndex];
        Player->setSource(QUrl::fromLocalFile(nextFile));

        // Verificar si el siguiente archivo es un audio o un video
        if (nextFile.endsWith(".mp3")) {
            // Reproducir video de fondo
            BackgroundPlayer->setSource(QUrl::fromLocalFile("C:/Users/maria/Desktop/Proyecto final/Video/imagenes/Disco.mp4"));
            BackgroundVideo->setVisible(true);
            BackgroundPlayer->play();
        } else {
            // Detener el video de fondo
            BackgroundPlayer->stop();
            BackgroundVideo->setVisible(false);

            // Configurar y mostrar el video
            Video = new QVideoWidget();
            Video->setGeometry(5, 5, ui->groupBox_Video->width() - 10, ui->groupBox_Video->height() - 10);
            Video->setParent(ui->groupBox_Video);
            Player->setVideoOutput(Video);
            Player->setSource(QUrl::fromLocalFile(nextFile));
            Video->setVisible(true);
            Video->show();
        }

        Player->play();
        fileList->setCurrentRow(currentIndex);
    }
}

void MainWindow::on_pushButton_Random_clicked()
{
    // Verificar si la lista de reproducción está vacía
    if (playlist.isEmpty()) {
        QMessageBox::warning(this, tr("Error"), tr("PRIMERO INSERTE ARCHIVOS"));
        return; // Salir de la función si no hay archivos
    }

    isRandom = !isRandom;
    if (isRandom) {
        ui->pushButton_Random->setText("Desactivar Aleatorio");

        // Reproducir un archivo aleatorio de la lista
        int randomIndex = QRandomGenerator::global()->generate() % playlist.size();
        currentIndex = randomIndex;
        QString randomFile = playlist[currentIndex];
        Player->setSource(QUrl::fromLocalFile(randomFile));

        // Verificar si el archivo aleatorio es un audio o un video
        if (randomFile.endsWith(".mp3")) {
            // Reproducir video de fondo
            BackgroundPlayer->setSource(QUrl::fromLocalFile("C:/Users/maria/Desktop/Proyecto final/Video/imagenes/Disco.mp4"));
            BackgroundVideo->setVisible(true);
            BackgroundPlayer->play();
        } else {
            // Detener el video de fondo
            BackgroundPlayer->stop();
            BackgroundVideo->setVisible(false);

            // Configurar y mostrar el video
            Video = new QVideoWidget();
            Video->setGeometry(5, 5, ui->groupBox_Video->width() - 10, ui->groupBox_Video->height() - 10);
            Video->setParent(ui->groupBox_Video);
            Player->setVideoOutput(Video);
            Player->setSource(QUrl::fromLocalFile(randomFile));
            Video->setVisible(true);
            Video->show();
        }

        Player->play();
        fileList->setCurrentRow(currentIndex);
    } else {
        ui->pushButton_Random->setText("Aleatorio");
    }
}
void MainWindow::moveItemUp()
{
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
    int currentIndex = fileList->currentRow();
    if (currentIndex < fileList->count() - 1) {
        QString currentItemText = fileList->item(currentIndex)->text();
        QString nextItemText = fileList->item(currentIndex + 1)->text();
        fileList->item(currentIndex + 1)->setText(currentItemText);
        fileList->item(currentIndex)->setText(nextItemText);
        fileList->setCurrentRow(currentIndex + 1);
    }
}







