/********************************************************************************
** Form generated from reading UI file 'setting.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETTING_H
#define UI_SETTING_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_settingDialog
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QComboBox *communicationStandardComboBox;
    QSpacerItem *horizontalSpacer_3;
    QHBoxLayout *horizontalLayout;
    QLabel *label_2;
    QLineEdit *predictiveStepSizelineEdit;
    QLabel *label_3;
    QHBoxLayout *hboxLayout;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *okButton;
    QPushButton *cancelButton;
    QSpacerItem *horizontalSpacer;

    void setupUi(QDialog *settingDialog)
    {
        if (settingDialog->objectName().isEmpty())
            settingDialog->setObjectName(QStringLiteral("settingDialog"));
        settingDialog->resize(276, 104);
        verticalLayout = new QVBoxLayout(settingDialog);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label = new QLabel(settingDialog);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout_2->addWidget(label);

        communicationStandardComboBox = new QComboBox(settingDialog);
        communicationStandardComboBox->setObjectName(QStringLiteral("communicationStandardComboBox"));
        communicationStandardComboBox->setMinimumSize(QSize(208, 0));

        horizontalLayout_2->addWidget(communicationStandardComboBox);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label_2 = new QLabel(settingDialog);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout->addWidget(label_2);

        predictiveStepSizelineEdit = new QLineEdit(settingDialog);
        predictiveStepSizelineEdit->setObjectName(QStringLiteral("predictiveStepSizelineEdit"));

        horizontalLayout->addWidget(predictiveStepSizelineEdit);

        label_3 = new QLabel(settingDialog);
        label_3->setObjectName(QStringLiteral("label_3"));

        horizontalLayout->addWidget(label_3);


        verticalLayout->addLayout(horizontalLayout);

        hboxLayout = new QHBoxLayout();
        hboxLayout->setSpacing(6);
        hboxLayout->setObjectName(QStringLiteral("hboxLayout"));
        hboxLayout->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(horizontalSpacer_2);

        okButton = new QPushButton(settingDialog);
        okButton->setObjectName(QStringLiteral("okButton"));

        hboxLayout->addWidget(okButton);

        cancelButton = new QPushButton(settingDialog);
        cancelButton->setObjectName(QStringLiteral("cancelButton"));

        hboxLayout->addWidget(cancelButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(horizontalSpacer);


        verticalLayout->addLayout(hboxLayout);


        retranslateUi(settingDialog);
        QObject::connect(okButton, SIGNAL(clicked()), settingDialog, SLOT(accept()));
        QObject::connect(cancelButton, SIGNAL(clicked()), settingDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(settingDialog);
    } // setupUi

    void retranslateUi(QDialog *settingDialog)
    {
        settingDialog->setWindowTitle(QApplication::translate("settingDialog", "\345\217\202\346\225\260\350\256\276\347\275\256", Q_NULLPTR));
        label->setText(QApplication::translate("settingDialog", "\345\210\266\345\274\217\357\274\232", Q_NULLPTR));
        label_2->setText(QApplication::translate("settingDialog", "\346\255\245\351\225\277\357\274\232", Q_NULLPTR));
        label_3->setText(QApplication::translate("settingDialog", "\347\261\263", Q_NULLPTR));
        okButton->setText(QApplication::translate("settingDialog", "\347\241\256\345\256\232", Q_NULLPTR));
        cancelButton->setText(QApplication::translate("settingDialog", "\345\217\226\346\266\210", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class settingDialog: public Ui_settingDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTING_H
