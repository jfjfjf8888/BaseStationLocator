/********************************************************************************
** Form generated from reading UI file 'predictResultDisplay.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PREDICTRESULTDISPLAY_H
#define UI_PREDICTRESULTDISPLAY_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_predictResultWidget
{
public:

    void setupUi(QWidget *predictResultWidget)
    {
        if (predictResultWidget->objectName().isEmpty())
            predictResultWidget->setObjectName(QStringLiteral("predictResultWidget"));
        predictResultWidget->resize(583, 403);

        retranslateUi(predictResultWidget);

        QMetaObject::connectSlotsByName(predictResultWidget);
    } // setupUi

    void retranslateUi(QWidget *predictResultWidget)
    {
        predictResultWidget->setWindowTitle(QApplication::translate("predictResultWidget", "\345\234\272\346\231\257\344\270\216\345\256\232\344\275\215", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class predictResultWidget: public Ui_predictResultWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PREDICTRESULTDISPLAY_H
