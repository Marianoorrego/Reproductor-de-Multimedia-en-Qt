//este archivo se genera automaticamente por el mainwindow.ui

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>             // Importa QtCore para trabajar con tipos de datos comunes
#include <QtWidgets/QWidgetAction>     // Importa QWidgetAction para trabajar con acciones de widgets
#include <QtWidgets/QApplication>      // Importa QApplication, que gestiona la ejecución de la aplicación
#include <QtWidgets/QGroupBox>         // Importa QGroupBox para crear grupos de widgets
#include <QtWidgets/QLabel>            // Importa QLabel para mostrar texto
#include <QtWidgets/QMainWindow>       // Importa QMainWindow para crear la ventana principal
#include <QtWidgets/QMenu>             // Importa QMenu para crear menús
#include <QtWidgets/QMenuBar>          // Importa QMenuBar para crear la barra de menú
#include <QtWidgets/QPushButton>       // Importa QPushButton para crear botones
#include <QtWidgets/QSlider>           // Importa QSlider para crear controles deslizantes
#include <QtWidgets/QWidget>           // Importa QWidget, la clase base para todos los widgets
#include <QVBoxLayout>                 // Importa QVBoxLayout para diseñar la interfaz con un layout vertical
#include <QPushButton>                 // Importa QPushButton
QT_BEGIN_NAMESPACE

class Ui_MainWindow                // Definición de la clase Ui_MainWindow, que contendrá todos los elementos de la interfaz de usuario
{
 // Declaración de los elementos de la interfaz de usuario
public:
    QAction *actionOpen;                         // Acción para abrir archivos
    QWidget *centralwidget;                      // Widget central de la ventana
    QGroupBox *groupBox_Video;                   // Grupo para contener los controles de video
    QPushButton *pushButton_Seek_Backward;       // Botón para retroceder en el video
    QPushButton *pushButton_Play_Pause;          // Botón para reproducir o pausar el video
    QPushButton *pushButton_Stop;                // Botón para detener el video
    QPushButton *pushButton_Seek_Forward;        // Botón para adelantar el video
    QPushButton *pushButton_Volume;              // Botón para ajustar el volumen
    QSlider *horizontalSlider_Volume;            // Control deslizante para ajustar el volumen
    QSlider *horizontalSlider_Duration;          // Control deslizante para ajustar la duración
    QLabel *label_current_Time;                  // Etiqueta para mostrar el tiempo actual
    QLabel *label_Total_Time;                    // Etiqueta para mostrar el tiempo total
    QMenuBar *menubar;                           // Barra de menús
    QMenu *menuSelect_Video_File;                // Menú para seleccionar un archivo de video
    QPushButton *pushButton_Next;                // Botón para reproducir el siguiente video
    QPushButton *pushButton_Previous;            // Botón para reproducir el video anterior


    void setupUi(QMainWindow *MainWindow)           // Método para configurar la interfaz de usuario
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));       // Verifica si el nombre de la ventana está vacío, y si es así lo asigna
        MainWindow->resize(800, 575);                                         // Establece el tamaño de la ventana principal
        MainWindow->setStyleSheet(QString::fromUtf8(""));                     // Define el estilo de la ventana principal
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName(QString::fromUtf8("actionOpen"));           // Crea la acción "Abrir" que se usará para abrir archivos
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));     // Crea el widget central para la ventana principal
        groupBox_Video = new QGroupBox(centralwidget);                        // Crea un grupo para los controles de video dentro del widget central
        groupBox_Video->setObjectName(QString::fromUtf8("groupBox_Video"));
        groupBox_Video->setGeometry(QRect(5, 5, 790, 450));                   // Define la geometría (posición y tamaño) del grupo
        groupBox_Video->setStyleSheet(QString::fromUtf8("QGroupBox\n"
                                                        "{\n"
                                                        "	border:1px solid beige;\n"
                                                        "}"));                // Define el estilo del grupo de video


        // Botones y controles de video

        // Botón de retroceso (Seek Backward) en el reproductor de video
        pushButton_Seek_Backward = new QPushButton(centralwidget);                                  // Crea el botón y lo asigna al widget central
        pushButton_Seek_Backward->setObjectName(QString::fromUtf8("pushButton_Seek_Backward"));     // Asigna un nombre único para identificar el botón
        pushButton_Seek_Backward->setGeometry(QRect(330, 500, 51, 41));                             // Define la posición y tamaño del botón

        // Botón de reproducir/pausar (Play/Pause) en el reproductor de video
        pushButton_Play_Pause = new QPushButton(centralwidget);                                      // Crea el botón y lo asigna al widget central  
        pushButton_Play_Pause->setObjectName(QString::fromUtf8("pushButton_Play_Pause"));            // Asigna un nombre único para identificar el botón
        pushButton_Play_Pause->setGeometry(QRect(380, 500, 51, 41));                                 // Define la posición y tamaño del botón

        // Botón de detener (Stop) en el reproductor de video
        pushButton_Stop = new QPushButton(centralwidget);                                            // Crea el botón y lo asigna al widget central
        pushButton_Stop->setObjectName(QString::fromUtf8("pushButton_Stop"));                        // Asigna un nombre único para identificar el botón
        pushButton_Stop->setGeometry(QRect(280, 500, 51, 41));                                       // Define la posición y tamaño del botón

        // Botón de avance (Seek Forward) en el reproductor de video
        pushButton_Seek_Forward = new QPushButton(centralwidget);                                    // Crea el botón y lo asigna al widget central
        pushButton_Seek_Forward->setObjectName(QString::fromUtf8("pushButton_Seek_Forward"));        // Asigna un nombre único para identificar el botón
        pushButton_Seek_Forward->setGeometry(QRect(430, 500, 51, 41));                               // Define la posición (x, y) y tamaño (ancho, alto) del botón en la interfaz

        // Botón de control de volumen (Mute/Unmute) en el reproductor de video
        pushButton_Volume = new QPushButton(centralwidget);                                          // Crea el botón y lo asigna al widget central
        pushButton_Volume->setObjectName(QString::fromUtf8("pushButton_Volume"));                    // Asigna un nombre único para identificar el botón
        pushButton_Volume->setGeometry(QRect(490, 500, 51, 41));                                     // Define la posición y tamaño del botón

        // Deslizadores para volumen y duración
        horizontalSlider_Volume = new QSlider(centralwidget);                                        // Crear un nuevo objeto QSlider dentro del widget central para el volumen
        horizontalSlider_Volume->setObjectName(QString::fromUtf8("horizontalSlider_Volume"));        // Asigna el nombre "horizontalSlider_Volume" para identificar este deslizador
        horizontalSlider_Volume->setGeometry(QRect(550, 510, 131, 22));                              // Establece posición y tamaño del deslizador de volumen, coordenadas (550, 510) y dimensiones 131x22 píxeles
        horizontalSlider_Volume->setOrientation(Qt::Horizontal);                                     // Define la orientación del deslizador como horizontal
        horizontalSlider_Volume->setStyleSheet( 
            "QSlider::groove:horizontal {"
            "    border: 1px solid #999999;"
            "    height: 8px;"
            "    background: beige;"
            "    margin: 2px 0;"
            "}"
            "QSlider::handle:horizontal {"
            "    background: #5c5c5c;"
            "    border: 1px solid #5c5c5c;"
            "    width: 18px;"
            "    margin: -2px 0;"
            "    border-radius: 3px;"
            "}"
            );

        horizontalSlider_Duration = new QSlider(centralwidget);                                    // Crear un nuevo objeto QSlider dentro del widget central para la duración
        horizontalSlider_Duration->setObjectName(QString::fromUtf8("horizontalSlider_Duration"));  // Asigna el nombre "horizontalSlider_Duration" para identificar este deslizador
        horizontalSlider_Duration->setGeometry(QRect(80, 460, 631, 22));                           // Establece posición y tamaño del deslizador de duración, coordenadas (80, 460) y dimensiones 631x22 píxeles
        horizontalSlider_Duration->setOrientation(Qt::Horizontal);                                 // Define la orientación del deslizador como horizontal
        horizontalSlider_Duration->setStyleSheet(
            "QSlider::groove:horizontal {"
            "    border: 1px solid #999999;"
            "    height: 8px;"
            "    background: beige;"
            "    margin: 2px 0;"
            "}"
            "QSlider::handle:horizontal {"
            "    background: #5c5c5c;"
            "    border: 1px solid #5c5c5c;"
            "    width: 18px;"
            "    margin: -2px 0;"
            "    border-radius: 3px;"
            "}"
            );

       // Crear la etiqueta para mostrar el tiempo actual del archivo
        label_current_Time = new QLabel(centralwidget);                                // Crear un nuevo objeto QLabel dentro del widget central para mostrar el tiempo actual
        label_current_Time->setObjectName(QString::fromUtf8("label_current_Time"));    / Asigna el nombre "label_current_Time" para identificar esta etiqueta
        label_current_Time->setGeometry(QRect(10, 460, 71, 16));                       // Define la posición y tamaño de la etiqueta de tiempo actual
        QFont font;
        font.setPointSize(12);                 // Define el tamaño de la fuente
        font.setBold(true);                    // Define que la fuente será negrita
        label_current_Time->setFont(font);     // Aplica la fuente al texto de la etiqueta

        // Crear la etiqueta para mostrar el tiempo total del archivo
        label_Total_Time = new QLabel(centralwidget);                              // Crear un nuevo objeto QLabel dentro del widget central para mostrar el tiempo total
        label_Total_Time->setObjectName(QString::fromUtf8("label_Total_Time"));    // Asigna el nombre "label_Total_Time" para identificar esta etiqueta
        label_Total_Time->setGeometry(QRect(720, 460, 71, 16));                    // Define la posición y tamaño de la etiqueta de tiempo tota
        label_Total_Time->setFont(font);
        
        MainWindow->setCentralWidget(centralwidget);                      // Establece el widget central en la ventana principal
        menubar = new QMenuBar(MainWindow);                               // Crear un nuevo objeto QMenuBar para la ventana principal
        menubar->setObjectName(QString::fromUtf8("menubar"));             // Asigna el nombre "menubar" para identificar esta barra de menús
        menubar->setGeometry(QRect(0, 0, 800, 22));                       // Define la geometría de la barra de menú

        // Crear el menú para seleccionar el archivo de video
        menuSelect_Video_File = new QMenu(menubar);                                           // Crear un nuevo objeto QMenu dentro de la barra de menús para seleccionar el archivo de video
        menuSelect_Video_File->setObjectName(QString::fromUtf8("menuSelect_Video_File"));     // Asigna el nombre "menuSelect_Video_File" para identificar este menú
        MainWindow->setMenuBar(menubar);                                                      // Establecer la barra de menús en la ventana principal

        // Crear el botón para ir al siguiente video
        pushButton_Next = new QPushButton(centralwidget);                                    // Crear un nuevo objeto QPushButton dentro del widget central para ir al siguiente video
        pushButton_Next->setObjectName(QString::fromUtf8("pushButton_Next"));                // Asigna el nombre "pushButton_Next" para identificar este botón
        pushButton_Next->setGeometry(QRect(230, 500, 51, 41));                               // Establece la posición y el tamaño del botón

        // Crear el botón para retroceder al video anterior
        pushButton_Previous = new QPushButton(centralwidget);                               // Crear un nuevo objeto QPushButton dentro del widget central para retroceder al video anterior
        pushButton_Previous->setObjectName(QString::fromUtf8("pushButton_Previous"));       // Asigna el nombre "pushButton_Previous" para identificar este botón
        pushButton_Previous->setGeometry(QRect(180, 500, 51, 41));                          // Establece la posición y el tamaño del botón


        // Configurar la barra de menús para agregar el menú de selección de video
        menubar->addAction(menuSelect_Video_File->menuAction());    // Añade el menú de selección de video a la barra de menús
 
        // Configurar la barra de menús para agregar el menú de selección de video
        menuSelect_Video_File->addAction(actionOpen);         // Agrega la acción "actionOpen" al menú, lo que permite abrir un archivo de video


        retranslateUi(MainWindow);                     // Llama a la función retranslateUi para configurar los textos de la interfaz en el idioma adecuado

        // Conecta automáticamente las señales y slots
        QMetaObject::connectSlotsByName(MainWindow);  // Conecta los slots con sus señales correspondientes, utilizando los nombres de los widgets y slots en la interfaz
    } // Fin de setupUi

   // Función para traducir y actualizar los textos de los widgets en la interfaz
    void retranslateUi(QMainWindow *MainWindow)
    {
        
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));   // Establece el título de la ventana principal
        actionOpen->setText(QCoreApplication::translate("MainWindow", "Abrir", nullptr));               // Establece el texto para la acción "Abrir" en el menú de selección de archivo
        groupBox_Video->setTitle(QString());                                                            // Asigna un título vacío al groupBox de video
        
         // Establece texto vacío en los botones de control de video (adelantar, reproducir/pausar, detener, retroceder, volumen)
        pushButton_Seek_Backward->setText(QString());           // Botón de retroceso en la reproducción                                        
        pushButton_Play_Pause->setText(QString());              // Botón de reproducir/pausa
        pushButton_Stop->setText(QString());                    // Botón de detener
        pushButton_Seek_Forward->setText(QString());            // Botón de avance en la reproducción
        pushButton_Volume->setText(QString());                  // Botón de control de volumen

        // Configura las etiquetas de tiempo inicial y total en formato de reloj
        label_current_Time->setText(QCoreApplication::translate("MainWindow", "00:00:00", nullptr));      // Tiempo actual en la reproducción
        label_Total_Time->setText(QCoreApplication::translate("MainWindow", "00:00:00", nullptr));        // Tiempo total del video
        menuSelect_Video_File->setTitle(QCoreApplication::translate("MainWindow", "Archivo", nullptr));   // Establece el título del menú de selección de archivo de video


        // Establece texto vacío en los botones de siguiente y anterior
        pushButton_Next->setText(QCoreApplication::translate("MainWindow", "", nullptr));           // Botón de siguiente archivo
        pushButton_Previous->setText(QCoreApplication::translate("MainWindow", "", nullptr));       // Botón de archivo anterior
    } // Fin de  retranslateUi

};   // Fin de la clase
namespace Ui {     // Namespace Ui que agrupa clases relacionadas con la interfaz de usuario
class MainWindow: public Ui_MainWindow {};   // Clase Ui_MainWindow, utilizada para gestionar los elementos de la interfaz principal
} // Fin del namespace Ui

QT_END_NAMESPACE   // Fin de definición del namespace de Qt

#endif // UI_MAINWINDOW_H  // Fin de la protección de inclusión
