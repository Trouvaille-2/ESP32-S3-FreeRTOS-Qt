/********************************************************************************
** Form generated from reading UI file 'dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOG_H
#define UI_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *btnRefresh;
    QPushButton *btnConnect;
    QPushButton *btnclear;
    QLabel *lblStatus;
    QComboBox *comboBaud;
    QComboBox *comboPort;
    QWidget *layoutWidget1;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_2;
    QWidget *layoutWidget2;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QStringLiteral("Dialog"));
        Dialog->resize(400, 300);
        layoutWidget = new QWidget(Dialog);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(40, 230, 295, 30));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        btnRefresh = new QPushButton(layoutWidget);
        btnRefresh->setObjectName(QStringLiteral("btnRefresh"));

        horizontalLayout->addWidget(btnRefresh);

        btnConnect = new QPushButton(layoutWidget);
        btnConnect->setObjectName(QStringLiteral("btnConnect"));

        horizontalLayout->addWidget(btnConnect);

        btnclear = new QPushButton(layoutWidget);
        btnclear->setObjectName(QStringLiteral("btnclear"));

        horizontalLayout->addWidget(btnclear);

        lblStatus = new QLabel(Dialog);
        lblStatus->setObjectName(QStringLiteral("lblStatus"));
        lblStatus->setGeometry(QRect(130, 180, 131, 16));
        comboBaud = new QComboBox(Dialog);
        comboBaud->setObjectName(QStringLiteral("comboBaud"));
        comboBaud->setGeometry(QRect(170, 130, 151, 21));
        comboPort = new QComboBox(Dialog);
        comboPort->setObjectName(QStringLiteral("comboPort"));
        comboPort->setGeometry(QRect(170, 60, 151, 21));
        layoutWidget1 = new QWidget(Dialog);
        layoutWidget1->setObjectName(QStringLiteral("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(40, 120, 86, 33));
        horizontalLayout_3 = new QHBoxLayout(layoutWidget1);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(layoutWidget1);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout_3->addWidget(label_2);

        layoutWidget2 = new QWidget(Dialog);
        layoutWidget2->setObjectName(QStringLiteral("layoutWidget2"));
        layoutWidget2->setGeometry(QRect(40, 50, 113, 33));
        horizontalLayout_2 = new QHBoxLayout(layoutWidget2);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(layoutWidget2);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout_2->addWidget(label);


        retranslateUi(Dialog);

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QApplication::translate("Dialog", "Dialog", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        Dialog->setToolTip(QApplication::translate("Dialog", "<html><head/><body><p>\347\263\273\347\273\237</p></body></html>", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        Dialog->setWhatsThis(QApplication::translate("Dialog", "<html><head/><body><p>\347\263\273\347\273\237</p></body></html>", Q_NULLPTR));
#endif // QT_NO_WHATSTHIS
        btnRefresh->setText(QApplication::translate("Dialog", "\345\210\267\346\226\260\344\270\262\345\217\243", Q_NULLPTR));
        btnConnect->setText(QApplication::translate("Dialog", "\350\277\236\346\216\245", Q_NULLPTR));
        btnclear->setText(QApplication::translate("Dialog", "\345\205\263\351\227\255", Q_NULLPTR));
        lblStatus->setText(QApplication::translate("Dialog", "\350\257\267\351\200\211\346\213\251\344\270\262\345\217\243\345\271\266\350\277\236\346\216\245", Q_NULLPTR));
        label_2->setText(QApplication::translate("Dialog", "<html><head/><body><p><span style=\" font-size:16pt; font-style:italic;\">\346\263\242\347\211\271\347\216\207</span></p></body></html>", Q_NULLPTR));
        label->setText(QApplication::translate("Dialog", "<html><head/><body><p><span style=\" font-size:16pt; font-style:italic;\">\344\270\262\345\217\243\351\200\211\346\213\251</span></p></body></html>", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_H
