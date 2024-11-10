#include "mainwindow.h"
#include "pantallacarga.h"
#include <QApplication>
#include <QIcon>
#include <QPixmap>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Cargar la imagen original para el ícono
    QPixmap originalPixmap(":/imagenes/Nexoslogo.jpeg");

    // Crear un ícono con múltiples resoluciones
    QIcon appIcon;
    appIcon.addPixmap(originalPixmap.scaled(16, 16, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    appIcon.addPixmap(originalPixmap.scaled(32, 32, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    appIcon.addPixmap(originalPixmap.scaled(48, 48, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    appIcon.addPixmap(originalPixmap.scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    appIcon.addPixmap(originalPixmap.scaled(128, 128, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    appIcon.addPixmap(originalPixmap.scaled(256, 256, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    // Establecer ícono a nivel de aplicación
    a.setWindowIcon(appIcon);

    // Crear pantalla de carga
    PantallaCarga pantallaCarga;
    MainWindow w;

    // Conectar señal de finalización de la carga
    QObject::connect(&pantallaCarga, &PantallaCarga::cargaCompletada, [&]() {
        w.show();
    });

    pantallaCarga.show();

    return a.exec();
}
