/********************************************************************************
** Form generated from reading UI file 'Lab2.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LAB2_H
#define UI_LAB2_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Lab2Class
{
public:
    QWidget *centralWidget;
    QPushButton *pushButton;
    QStatusBar *statusBar;
    QMenuBar *menuBar;
    QMenu *menuHuffman;

    void setupUi(QMainWindow *Lab2Class)
    {
        if (Lab2Class->objectName().isEmpty())
            Lab2Class->setObjectName(QString::fromUtf8("Lab2Class"));
        Lab2Class->resize(600, 400);
        centralWidget = new QWidget(Lab2Class);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(220, 110, 93, 28));
        Lab2Class->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(Lab2Class);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        Lab2Class->setStatusBar(statusBar);
        menuBar = new QMenuBar(Lab2Class);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 26));
        menuHuffman = new QMenu(menuBar);
        menuHuffman->setObjectName(QString::fromUtf8("menuHuffman"));
        Lab2Class->setMenuBar(menuBar);

        menuBar->addAction(menuHuffman->menuAction());

        retranslateUi(Lab2Class);

        QMetaObject::connectSlotsByName(Lab2Class);
    } // setupUi

    void retranslateUi(QMainWindow *Lab2Class)
    {
        Lab2Class->setWindowTitle(QCoreApplication::translate("Lab2Class", "Lab2", nullptr));
        pushButton->setText(QCoreApplication::translate("Lab2Class", "PushButton", nullptr));
        menuHuffman->setTitle(QCoreApplication::translate("Lab2Class", "Huffman\345\216\213\347\274\251", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Lab2Class: public Ui_Lab2Class {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LAB2_H
