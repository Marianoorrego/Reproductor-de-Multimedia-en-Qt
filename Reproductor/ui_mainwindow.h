//este archivo se genera automaticamente por el mainwindow.ui

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QWidgetAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QWidget>
#include <QVBoxLayout>
#include <QPushButton>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{

public:
    QAction *actionOpen;
    QWidget *centralwidget;
    QGroupBox *groupBox_Video;
    QPushButton *pushButton_Seek_Backward;
    QPushButton *pushButton_Play_Pause;
    QPushButton *pushButton_Stop;
    QPushButton *pushButton_Seek_Forward;
    QPushButton *pushButton_Volume;
    QSlider *horizontalSlider_Volume;
    QSlider *horizontalSlider_Duration;
    QLabel *label_current_Time;
    QLabel *label_Total_Time;
    QMenuBar *menubar;
    QMenu *menuSelect_Video_File;
    QPushButton *pushButton_Next;
    QPushButton *pushButton_Previous;


    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800, 575);
        MainWindow->setStyleSheet(QString::fromUtf8(""));
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        groupBox_Video = new QGroupBox(centralwidget);
        groupBox_Video->setObjectName(QString::fromUtf8("groupBox_Video"));
        groupBox_Video->setGeometry(QRect(5, 5, 790, 450));
        groupBox_Video->setStyleSheet(QString::fromUtf8("QGroupBox\n"
                                                        "{\n"
                                                        "	border:1px solid beige;\n"
                                                        "}"));
        pushButton_Seek_Backward = new QPushButton(centralwidget);
        pushButton_Seek_Backward->setObjectName(QString::fromUtf8("pushButton_Seek_Backward"));
        pushButton_Seek_Backward->setGeometry(QRect(330, 500, 51, 41));
        pushButton_Play_Pause = new QPushButton(centralwidget);
        pushButton_Play_Pause->setObjectName(QString::fromUtf8("pushButton_Play_Pause"));
        pushButton_Play_Pause->setGeometry(QRect(380, 500, 51, 41));
        pushButton_Stop = new QPushButton(centralwidget);
        pushButton_Stop->setObjectName(QString::fromUtf8("pushButton_Stop"));
        pushButton_Stop->setGeometry(QRect(280, 500, 51, 41));
        pushButton_Seek_Forward = new QPushButton(centralwidget);
        pushButton_Seek_Forward->setObjectName(QString::fromUtf8("pushButton_Seek_Forward"));
        pushButton_Seek_Forward->setGeometry(QRect(430, 500, 51, 41));
        pushButton_Volume = new QPushButton(centralwidget);
        pushButton_Volume->setObjectName(QString::fromUtf8("pushButton_Volume"));
        pushButton_Volume->setGeometry(QRect(490, 500, 51, 41));
        horizontalSlider_Volume = new QSlider(centralwidget);
        horizontalSlider_Volume->setObjectName(QString::fromUtf8("horizontalSlider_Volume"));
        horizontalSlider_Volume->setGeometry(QRect(550, 510, 131, 22));
        horizontalSlider_Volume->setOrientation(Qt::Horizontal);
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
        horizontalSlider_Duration = new QSlider(centralwidget);
        horizontalSlider_Duration->setObjectName(QString::fromUtf8("horizontalSlider_Duration"));
        horizontalSlider_Duration->setGeometry(QRect(80, 460, 631, 22));
        horizontalSlider_Duration->setOrientation(Qt::Horizontal);
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


        label_current_Time = new QLabel(centralwidget);
        label_current_Time->setObjectName(QString::fromUtf8("label_current_Time"));
        label_current_Time->setGeometry(QRect(10, 460, 71, 16));
        QFont font;
        font.setPointSize(12);
        font.setBold(true);
        label_current_Time->setFont(font);
        label_Total_Time = new QLabel(centralwidget);
        label_Total_Time->setObjectName(QString::fromUtf8("label_Total_Time"));
        label_Total_Time->setGeometry(QRect(720, 460, 71, 16));
        label_Total_Time->setFont(font);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 22));
        menuSelect_Video_File = new QMenu(menubar);
        menuSelect_Video_File->setObjectName(QString::fromUtf8("menuSelect_Video_File"));
        MainWindow->setMenuBar(menubar);

        pushButton_Next = new QPushButton(centralwidget);
        pushButton_Next->setObjectName(QString::fromUtf8("pushButton_Next"));
        pushButton_Next->setGeometry(QRect(230, 500, 51, 41));

        pushButton_Previous = new QPushButton(centralwidget);
        pushButton_Previous->setObjectName(QString::fromUtf8("pushButton_Previous"));
        pushButton_Previous->setGeometry(QRect(180, 500, 51, 41));



        menubar->addAction(menuSelect_Video_File->menuAction());
        menuSelect_Video_File->addAction(actionOpen);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        actionOpen->setText(QCoreApplication::translate("MainWindow", "Abrir", nullptr));
        groupBox_Video->setTitle(QString());
        pushButton_Seek_Backward->setText(QString());
        pushButton_Play_Pause->setText(QString());
        pushButton_Stop->setText(QString());
        pushButton_Seek_Forward->setText(QString());
        pushButton_Volume->setText(QString());
        label_current_Time->setText(QCoreApplication::translate("MainWindow", "00:00:00", nullptr));
        label_Total_Time->setText(QCoreApplication::translate("MainWindow", "00:00:00", nullptr));
        menuSelect_Video_File->setTitle(QCoreApplication::translate("MainWindow", "Archivo", nullptr));


        pushButton_Next->setText(QCoreApplication::translate("MainWindow", "", nullptr));
        pushButton_Previous->setText(QCoreApplication::translate("MainWindow", "", nullptr));
    } // retranslateUi

};
namespace Ui {
class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
