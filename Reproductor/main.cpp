#include "mainwindow.h"          // Incluye el archivo de cabecera de la clase MainWindow (ventana principal)
#include "pantallacarga.h"       // Incluye el archivo de cabecera de la clase PantallaCarga (pantalla de carga)
#include <QApplication>          // Incluye la clase QApplication, que gestiona la ejecución de la aplicación
#include <QIcon>                 // Incluye la clase QIcon para gestionar íconos en la aplicación
#include <QPixmap>               // Incluye la clase QPixmap para manejar imágenes y gráficos

// Función principal de la aplicación
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);                // Crea una instancia de la aplicación con los argumentos de línea de comandos
    a.setApplicationName("NEXOS");             // Establece el nombre de la aplicación
    a.setApplicationVersion("1.0");            // Establece la versión de la aplicación
    a.setOrganizationName("Nexos Media");      // Establece el nombre de la organización para la aplicación

    // Cargar la imagen original para el ícono de la aplicación
    QPixmap originalPixmap(":/imagenes/Nexoslogo.jpeg");     // Carga una imagen desde los recursos de la aplicación 

    // Crear un ícono con múltiples resoluciones para adaptarse a diferentes tamaños de ventana
    QIcon appIcon;
    appIcon.addPixmap(originalPixmap.scaled(16, 16, Qt::KeepAspectRatio, Qt::SmoothTransformation));   // Escala la imagen a 16x16 píxeles manteniendo la proporción y suavizando la transformación
    appIcon.addPixmap(originalPixmap.scaled(32, 32, Qt::KeepAspectRatio, Qt::SmoothTransformation));   // Escala a 32x32 píxeles
    appIcon.addPixmap(originalPixmap.scaled(48, 48, Qt::KeepAspectRatio, Qt::SmoothTransformation));   // Escala a 48x48 píxeles
    appIcon.addPixmap(originalPixmap.scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation));   // Escala a 64x64 píxeles
    appIcon.addPixmap(originalPixmap.scaled(128, 128, Qt::KeepAspectRatio, Qt::SmoothTransformation)); // Escala a 128x128 píxeles
    appIcon.addPixmap(originalPixmap.scaled(256, 256, Qt::KeepAspectRatio, Qt::SmoothTransformation)); // Escala a 256x256 píxeles

   // Establecer el ícono de la aplicación
    a.setWindowIcon(appIcon);   // Asocia el ícono creado al nivel de la ventana de la aplicación

     // Crear la pantalla de carga (PantallaCarga) y la ventana principal (MainWindow)
    PantallaCarga pantallaCarga;      // Crea una instancia de la pantalla de carga
    MainWindow w;                     // Crea una instancia de la ventana principal

   // Conectar la señal 'cargaCompletada' de la pantalla de carga a la acción de mostrar la ventana principal
    QObject::connect(&pantallaCarga, &PantallaCarga::cargaCompletada, [&]() {     // Conecta la señal cargaCompletada de PantallaCarga
        w.show();      // Muestra la ventana principal cuando la carga se completa
    });

    // Mostrar la pantalla de carga
    pantallaCarga.show();           // Muestra la pantalla de carga en la interfaz de usuario
 
    return a.exec();             // Inicia el bucle de eventos de la aplicación y espera interacciones del usuario

}
