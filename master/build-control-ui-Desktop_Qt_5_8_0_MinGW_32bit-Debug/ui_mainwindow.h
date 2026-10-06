/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QWidget *widgetSetting;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QLabel *label;
    QComboBox *comboMode;
    QPushButton *btnEnable;
    QPushButton *btnStop;
    QPushButton *btnBrake;
    QPushButton *btnResetPos;
    QGroupBox *groupBox_2;
    QLabel *label_2;
    QLabel *label_3;
    QSpinBox *setting_num;
    QLabel *feedback_num;
    QGroupBox *groupBox_3;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label_8;
    QSpinBox *kp_box;
    QSpinBox *ki_box;
    QSpinBox *kd_box;
    QSpinBox *imax_box;
    QSpinBox *imin_box;
    QGroupBox *groupBox_4;
    QLabel *label_9;
    QLabel *label_10;
    QLabel *label_11;
    QLabel *label_12;
    QLCDNumber *lblActualPos;
    QLCDNumber *lblActualSpd;
    QLCDNumber *lblPwmOutput;
    QLCDNumber *lblPosErrot;
    QWidget *widgetChart;
    QMenuBar *menubar;
    QMenu *menu;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1293, 604);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        widgetSetting = new QWidget(centralwidget);
        widgetSetting->setObjectName(QStringLiteral("widgetSetting"));
        widgetSetting->setEnabled(true);
        verticalLayout = new QVBoxLayout(widgetSetting);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        groupBox = new QGroupBox(widgetSetting);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setMinimumSize(QSize(291, 0));
        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(0, 20, 72, 15));
        comboMode = new QComboBox(groupBox);
        comboMode->setObjectName(QStringLiteral("comboMode"));
        comboMode->setGeometry(QRect(90, 20, 131, 22));
        btnEnable = new QPushButton(groupBox);
        btnEnable->setObjectName(QStringLiteral("btnEnable"));
        btnEnable->setGeometry(QRect(0, 50, 93, 28));
        btnStop = new QPushButton(groupBox);
        btnStop->setObjectName(QStringLiteral("btnStop"));
        btnStop->setGeometry(QRect(190, 50, 93, 28));
        btnBrake = new QPushButton(groupBox);
        btnBrake->setObjectName(QStringLiteral("btnBrake"));
        btnBrake->setGeometry(QRect(0, 80, 93, 28));
        btnResetPos = new QPushButton(groupBox);
        btnResetPos->setObjectName(QStringLiteral("btnResetPos"));
        btnResetPos->setGeometry(QRect(190, 80, 93, 28));

        verticalLayout->addWidget(groupBox);

        groupBox_2 = new QGroupBox(widgetSetting);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setMinimumSize(QSize(291, 0));
        label_2 = new QLabel(groupBox_2);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(10, 40, 72, 15));
        label_3 = new QLabel(groupBox_2);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(10, 90, 72, 15));
        setting_num = new QSpinBox(groupBox_2);
        setting_num->setObjectName(QStringLiteral("setting_num"));
        setting_num->setGeometry(QRect(90, 40, 181, 22));
        feedback_num = new QLabel(groupBox_2);
        feedback_num->setObjectName(QStringLiteral("feedback_num"));
        feedback_num->setGeometry(QRect(90, 90, 72, 15));

        verticalLayout->addWidget(groupBox_2);

        groupBox_3 = new QGroupBox(widgetSetting);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setMinimumSize(QSize(291, 0));
        label_4 = new QLabel(groupBox_3);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(20, 20, 72, 15));
        label_5 = new QLabel(groupBox_3);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(20, 40, 72, 15));
        label_6 = new QLabel(groupBox_3);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(20, 60, 72, 15));
        label_7 = new QLabel(groupBox_3);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(20, 80, 72, 15));
        label_8 = new QLabel(groupBox_3);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(20, 100, 72, 15));
        kp_box = new QSpinBox(groupBox_3);
        kp_box->setObjectName(QStringLiteral("kp_box"));
        kp_box->setGeometry(QRect(90, 20, 181, 22));
        ki_box = new QSpinBox(groupBox_3);
        ki_box->setObjectName(QStringLiteral("ki_box"));
        ki_box->setGeometry(QRect(90, 40, 181, 22));
        kd_box = new QSpinBox(groupBox_3);
        kd_box->setObjectName(QStringLiteral("kd_box"));
        kd_box->setGeometry(QRect(90, 60, 181, 22));
        imax_box = new QSpinBox(groupBox_3);
        imax_box->setObjectName(QStringLiteral("imax_box"));
        imax_box->setGeometry(QRect(90, 80, 181, 22));
        imin_box = new QSpinBox(groupBox_3);
        imin_box->setObjectName(QStringLiteral("imin_box"));
        imin_box->setGeometry(QRect(90, 100, 181, 22));

        verticalLayout->addWidget(groupBox_3);

        groupBox_4 = new QGroupBox(widgetSetting);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        groupBox_4->setMinimumSize(QSize(291, 0));
        label_9 = new QLabel(groupBox_4);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setGeometry(QRect(10, 30, 72, 15));
        label_10 = new QLabel(groupBox_4);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setGeometry(QRect(10, 60, 72, 15));
        label_11 = new QLabel(groupBox_4);
        label_11->setObjectName(QStringLiteral("label_11"));
        label_11->setGeometry(QRect(10, 90, 72, 15));
        label_12 = new QLabel(groupBox_4);
        label_12->setObjectName(QStringLiteral("label_12"));
        label_12->setGeometry(QRect(10, 120, 72, 15));
        lblActualPos = new QLCDNumber(groupBox_4);
        lblActualPos->setObjectName(QStringLiteral("lblActualPos"));
        lblActualPos->setGeometry(QRect(110, 20, 64, 23));
        lblActualSpd = new QLCDNumber(groupBox_4);
        lblActualSpd->setObjectName(QStringLiteral("lblActualSpd"));
        lblActualSpd->setGeometry(QRect(110, 60, 64, 23));
        lblPwmOutput = new QLCDNumber(groupBox_4);
        lblPwmOutput->setObjectName(QStringLiteral("lblPwmOutput"));
        lblPwmOutput->setGeometry(QRect(110, 90, 64, 23));
        lblPosErrot = new QLCDNumber(groupBox_4);
        lblPosErrot->setObjectName(QStringLiteral("lblPosErrot"));
        lblPosErrot->setGeometry(QRect(110, 120, 64, 23));

        verticalLayout->addWidget(groupBox_4);


        horizontalLayout->addWidget(widgetSetting);

        widgetChart = new QWidget(centralwidget);
        widgetChart->setObjectName(QStringLiteral("widgetChart"));

        horizontalLayout->addWidget(widgetChart);

        horizontalLayout->setStretch(0, 1);
        horizontalLayout->setStretch(1, 3);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 1293, 26));
        menu = new QMenu(menubar);
        menu->setObjectName(QStringLiteral("menu"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menu->menuAction());

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("MainWindow", "\344\274\272\346\234\215\350\277\220\350\241\214\346\216\247\345\210\266", Q_NULLPTR));
        label->setText(QApplication::translate("MainWindow", "\346\216\247\345\210\266\346\250\241\345\274\217", Q_NULLPTR));
        comboMode->clear();
        comboMode->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "0-\345\274\200\347\216\257\345\215\240\347\251\272\346\257\224", Q_NULLPTR)
         << QApplication::translate("MainWindow", "1-\345\215\225\351\200\237\345\272\246\351\227\255\347\216\257", Q_NULLPTR)
         << QApplication::translate("MainWindow", "2-\344\275\215\347\275\256\351\200\237\345\272\246\345\217\214\347\216\257", Q_NULLPTR)
        );
        btnEnable->setText(QApplication::translate("MainWindow", "\344\275\277\350\203\275\350\277\220\350\241\214", Q_NULLPTR));
        btnStop->setText(QApplication::translate("MainWindow", "\346\255\243\345\270\270\345\201\234\346\255\242", Q_NULLPTR));
        btnBrake->setText(QApplication::translate("MainWindow", "\347\264\247\346\200\245\345\210\271\350\275\246", Q_NULLPTR));
        btnResetPos->setText(QApplication::translate("MainWindow", "\347\274\226\347\240\201\345\231\250\345\233\236\351\233\266", Q_NULLPTR));
        groupBox_2->setTitle(QApplication::translate("MainWindow", "\350\277\220\345\212\250\347\233\256\346\240\207\344\270\213\345\217\221", Q_NULLPTR));
        label_2->setText(QApplication::translate("MainWindow", "\350\256\276\345\256\232\345\200\274", Q_NULLPTR));
        label_3->setText(QApplication::translate("MainWindow", "\345\217\215\351\246\210\345\200\274", Q_NULLPTR));
        feedback_num->setText(QApplication::translate("MainWindow", "TextLabel", Q_NULLPTR));
        groupBox_3->setTitle(QApplication::translate("MainWindow", "\345\217\214\347\216\257pid\345\217\202\346\225\260\346\225\264\345\256\232", Q_NULLPTR));
        label_4->setText(QApplication::translate("MainWindow", "Kp", Q_NULLPTR));
        label_5->setText(QApplication::translate("MainWindow", "Ki", Q_NULLPTR));
        label_6->setText(QApplication::translate("MainWindow", "Kd", Q_NULLPTR));
        label_7->setText(QApplication::translate("MainWindow", "Imax", Q_NULLPTR));
        label_8->setText(QApplication::translate("MainWindow", "Imin", Q_NULLPTR));
        groupBox_4->setTitle(QApplication::translate("MainWindow", "\345\256\236\346\227\266\347\212\266\346\200\201\347\233\221\346\216\247\347\234\213\346\235\277", Q_NULLPTR));
        label_9->setText(QApplication::translate("MainWindow", "\345\256\236\351\231\205\344\275\215\347\275\256\357\274\232", Q_NULLPTR));
        label_10->setText(QApplication::translate("MainWindow", "\345\256\236\351\231\205\350\275\254\351\200\237", Q_NULLPTR));
        label_11->setText(QApplication::translate("MainWindow", "pwm\350\276\223\345\207\272", Q_NULLPTR));
        label_12->setText(QApplication::translate("MainWindow", "\350\267\237\350\270\252\350\257\257\345\267\256\357\274\232", Q_NULLPTR));
        menu->setTitle(QApplication::translate("MainWindow", "\344\270\273\351\241\265\351\235\242", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
