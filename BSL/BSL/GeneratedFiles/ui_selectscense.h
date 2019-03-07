/********************************************************************************
** Form generated from reading UI file 'selectscense.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SELECTSCENSE_H
#define UI_SELECTSCENSE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_selectScenseClass
{
public:
    QVBoxLayout *verticalLayout;
    QListWidget *listWidget;

    void setupUi(QDialog *selectScenseClass)
    {
        if (selectScenseClass->objectName().isEmpty())
            selectScenseClass->setObjectName(QStringLiteral("selectScenseClass"));
        selectScenseClass->resize(274, 192);
        verticalLayout = new QVBoxLayout(selectScenseClass);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        listWidget = new QListWidget(selectScenseClass);
        listWidget->setObjectName(QStringLiteral("listWidget"));
        listWidget->setIconSize(QSize(45, 45));
        listWidget->setGridSize(QSize(64, 64));
        listWidget->setViewMode(QListView::IconMode);

        verticalLayout->addWidget(listWidget);


        retranslateUi(selectScenseClass);
        QObject::connect(listWidget, SIGNAL(doubleClicked(QModelIndex)), selectScenseClass, SLOT(treeDoubleClicked(QModelIndex)));
        QObject::connect(listWidget, SIGNAL(clicked(QModelIndex)), selectScenseClass, SLOT(treeDoubleClicked(QModelIndex)));

        QMetaObject::connectSlotsByName(selectScenseClass);
    } // setupUi

    void retranslateUi(QDialog *selectScenseClass)
    {
        selectScenseClass->setWindowTitle(QApplication::translate("selectScenseClass", "\351\200\211\346\213\251\345\234\272\346\231\257", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class selectScenseClass: public Ui_selectScenseClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SELECTSCENSE_H
