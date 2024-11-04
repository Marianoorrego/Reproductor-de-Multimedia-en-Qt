QT       += core gui multimedia multimediawidgets  # Incluye los módulos de Qt necesarios para el proyecto: núcleo, interfaz gráfica, multimedia y widgets multimedia
RESOURCES += resources.qrc
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets  # Agrega el módulo de widgets si la versión de Qt es mayor a la 4

CONFIG += c++17  # Configura el proyecto para usar la versión de C++ 17

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # Deshabilita todas las APIs obsoletas antes de Qt 6.0.0

# Archivos fuente del proyecto
SOURCES += \
    main.cpp \
    mainwindow.cpp

# Archivos de cabecera del proyecto
HEADERS += \
    mainwindow.h \
    ui_mainwindow.h

# Archivos de diseño de la interfaz (formato .ui)
FORMS += \
    mainwindow.ui

# Configuración para despliegue en sistemas específicos
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# Archivos distribuidos con el proyecto (medios multimedia y recursos de imagen)
DISTFILES += \
    imagenes/Disco.mp4 \
    imagenes/avanzar.png \
    imagenes/fullscreen.png \
    imagenes/mute.png \
    imagenes/pausa.png \
    imagenes/play.png \
    imagenes/retroceder.png \
    imagenes/sound.png \
    imagenes/stop.png

RESOURCES += \
    resources.qrc
