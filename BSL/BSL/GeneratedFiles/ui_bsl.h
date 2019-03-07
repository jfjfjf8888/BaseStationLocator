/********************************************************************************
** Form generated from reading UI file 'bsl.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BSL_H
#define UI_BSL_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_BSLClass
{
public:
    QWidget *centralWidget;

    void setupUi(QMainWindow *BSLClass)
    {
        if (BSLClass->objectName().isEmpty())
            BSLClass->setObjectName(QStringLiteral("BSLClass"));
        BSLClass->resize(600, 400);
        centralWidget = new QWidget(BSLClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        BSLClass->setCentralWidget(centralWidget);

        retranslateUi(BSLClass);

        QMetaObject::connectSlotsByName(BSLClass);
    } // setupUi

    void retranslateUi(QMainWindow *BSLClass)
    {
        BSLClass->setWindowTitle(QApplication::translate("BSLClass", "BSL", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class BSLClass: public Ui_BSLClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BSL_H
