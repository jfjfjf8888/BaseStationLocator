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
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_BSLClass
{
public:
    QAction *loadSencesAction;
    QAction *resetAction;
    QAction *loadFileAction;
    QAction *runAction;
    QAction *exitAction;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QToolButton *loadSencesToolButton;
    QToolButton *resetToolButton;
    QToolButton *loadFileToolButton;
    QToolButton *runToolButton;
    QSpacerItem *verticalSpacer_2;
    QFrame *line;
    QLabel *label;
    QMenuBar *menuBar;
    QMenu *menu_F;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *BSLClass)
    {
        if (BSLClass->objectName().isEmpty())
            BSLClass->setObjectName(QStringLiteral("BSLClass"));
        BSLClass->resize(1004, 646);
        loadSencesAction = new QAction(BSLClass);
        loadSencesAction->setObjectName(QStringLiteral("loadSencesAction"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/BSL/Icon/load.png"), QSize(), QIcon::Normal, QIcon::Off);
        loadSencesAction->setIcon(icon);
        resetAction = new QAction(BSLClass);
        resetAction->setObjectName(QStringLiteral("resetAction"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/BSL/Icon/reset.png"), QSize(), QIcon::Normal, QIcon::Off);
        resetAction->setIcon(icon1);
        loadFileAction = new QAction(BSLClass);
        loadFileAction->setObjectName(QStringLiteral("loadFileAction"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/BSL/Icon/open.png"), QSize(), QIcon::Normal, QIcon::Off);
        loadFileAction->setIcon(icon2);
        runAction = new QAction(BSLClass);
        runAction->setObjectName(QStringLiteral("runAction"));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/BSL/Icon/launch.png"), QSize(), QIcon::Normal, QIcon::Off);
        runAction->setIcon(icon3);
        exitAction = new QAction(BSLClass);
        exitAction->setObjectName(QStringLiteral("exitAction"));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/BSL/Icon/exit.png"), QSize(), QIcon::Normal, QIcon::Off);
        exitAction->setIcon(icon4);
        centralWidget = new QWidget(BSLClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        loadSencesToolButton = new QToolButton(centralWidget);
        loadSencesToolButton->setObjectName(QStringLiteral("loadSencesToolButton"));
        loadSencesToolButton->setIcon(icon);
        loadSencesToolButton->setIconSize(QSize(32, 32));
        loadSencesToolButton->setAutoRaise(true);

        verticalLayout->addWidget(loadSencesToolButton);

        resetToolButton = new QToolButton(centralWidget);
        resetToolButton->setObjectName(QStringLiteral("resetToolButton"));
        resetToolButton->setIcon(icon1);
        resetToolButton->setIconSize(QSize(32, 32));
        resetToolButton->setAutoRaise(true);

        verticalLayout->addWidget(resetToolButton);

        loadFileToolButton = new QToolButton(centralWidget);
        loadFileToolButton->setObjectName(QStringLiteral("loadFileToolButton"));
        loadFileToolButton->setIcon(icon2);
        loadFileToolButton->setIconSize(QSize(32, 32));
        loadFileToolButton->setAutoRaise(true);

        verticalLayout->addWidget(loadFileToolButton);

        runToolButton = new QToolButton(centralWidget);
        runToolButton->setObjectName(QStringLiteral("runToolButton"));
        runToolButton->setIcon(icon3);
        runToolButton->setIconSize(QSize(32, 32));
        runToolButton->setAutoRaise(true);

        verticalLayout->addWidget(runToolButton);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);


        horizontalLayout->addLayout(verticalLayout);

        line = new QFrame(centralWidget);
        line->setObjectName(QStringLiteral("line"));
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);

        horizontalLayout->addWidget(line);

        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        QFont font;
        font.setPointSize(20);
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);
        label->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(label);

        BSLClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(BSLClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1004, 23));
        menu_F = new QMenu(menuBar);
        menu_F->setObjectName(QStringLiteral("menu_F"));
        BSLClass->setMenuBar(menuBar);
        statusBar = new QStatusBar(BSLClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        BSLClass->setStatusBar(statusBar);

        menuBar->addAction(menu_F->menuAction());
        menu_F->addAction(loadSencesAction);
        menu_F->addAction(resetAction);
        menu_F->addAction(loadFileAction);
        menu_F->addAction(runAction);
        menu_F->addSeparator();
        menu_F->addAction(exitAction);

        retranslateUi(BSLClass);
        QObject::connect(loadSencesAction, SIGNAL(triggered()), loadSencesToolButton, SLOT(click()));
        QObject::connect(resetAction, SIGNAL(triggered()), resetToolButton, SLOT(click()));
        QObject::connect(loadFileAction, SIGNAL(triggered()), loadFileToolButton, SLOT(click()));
        QObject::connect(runAction, SIGNAL(triggered()), runToolButton, SLOT(click()));
        QObject::connect(exitAction, SIGNAL(triggered()), BSLClass, SLOT(close()));

        QMetaObject::connectSlotsByName(BSLClass);
    } // setupUi

    void retranslateUi(QMainWindow *BSLClass)
    {
        BSLClass->setWindowTitle(QApplication::translate("BSLClass", "\346\227\240\347\272\277\344\277\241\345\217\267\346\272\220\345\256\232\347\263\273\347\273\237", Q_NULLPTR));
        loadSencesAction->setText(QApplication::translate("BSLClass", "\345\212\240\350\275\275\345\234\272\346\231\257", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        loadSencesAction->setStatusTip(QApplication::translate("BSLClass", "\345\212\240\350\275\275\347\224\261EvaluatorGUI\347\224\237\346\210\220\347\232\204\345\234\272\346\231\257", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        resetAction->setText(QApplication::translate("BSLClass", "\351\207\215\347\275\256", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        resetAction->setStatusTip(QApplication::translate("BSLClass", "\346\270\205\347\251\272\346\211\200\346\234\211\346\225\260\346\215\256", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        loadFileAction->setText(QApplication::translate("BSLClass", "\345\212\240\350\275\275\346\226\207\344\273\266", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        loadFileAction->setStatusTip(QApplication::translate("BSLClass", "\345\212\240\350\275\275\346\265\213\350\257\225\346\226\207\344\273\266", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        runAction->setText(QApplication::translate("BSLClass", "\345\220\257\345\212\250\345\256\232\344\275\215", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        runAction->setStatusTip(QApplication::translate("BSLClass", "\345\274\200\345\247\213\345\256\232\344\275\215", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        exitAction->setText(QApplication::translate("BSLClass", "\351\200\200\345\207\272", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        exitAction->setStatusTip(QApplication::translate("BSLClass", "\351\200\200\345\207\272", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_TOOLTIP
        loadSencesToolButton->setToolTip(QApplication::translate("BSLClass", "\345\212\240\350\275\275\347\224\261EvaluatorGUI\347\224\237\346\210\220\347\232\204\345\234\272\346\231\257", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        loadSencesToolButton->setStatusTip(QApplication::translate("BSLClass", "\345\212\240\350\275\275\347\224\261EvaluatorGUI\347\224\237\346\210\220\347\232\204\345\234\272\346\231\257", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        loadSencesToolButton->setText(QApplication::translate("BSLClass", "\345\212\240\350\275\275\345\234\272\346\231\257", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        resetToolButton->setToolTip(QApplication::translate("BSLClass", "\346\270\205\347\251\272\346\211\200\346\234\211\346\225\260\346\215\256", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        resetToolButton->setStatusTip(QApplication::translate("BSLClass", "\346\270\205\347\251\272\346\211\200\346\234\211\346\225\260\346\215\256", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        resetToolButton->setText(QApplication::translate("BSLClass", "\351\207\215\347\275\256", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        loadFileToolButton->setToolTip(QApplication::translate("BSLClass", "\345\212\240\350\275\275\346\265\213\350\257\225\346\226\207\344\273\266", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        loadFileToolButton->setStatusTip(QApplication::translate("BSLClass", "\345\212\240\350\275\275\346\265\213\350\257\225\346\226\207\344\273\266", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        loadFileToolButton->setText(QApplication::translate("BSLClass", "\345\212\240\350\275\275\346\226\207\344\273\266", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        runToolButton->setToolTip(QApplication::translate("BSLClass", "\345\274\200\345\247\213\345\256\232\344\275\215", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        runToolButton->setStatusTip(QApplication::translate("BSLClass", "\345\274\200\345\247\213\345\256\232\344\275\215", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        runToolButton->setText(QApplication::translate("BSLClass", "\345\220\257\345\212\250\345\256\232\344\275\215", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        label->setToolTip(QApplication::translate("BSLClass", "\347\234\237\345\256\236\345\234\272\346\231\257\346\230\276\347\244\272\345\214\272\345\237\237", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        label->setStatusTip(QApplication::translate("BSLClass", "\347\234\237\345\256\236\345\234\272\346\231\257\346\230\276\347\244\272\345\214\272\345\237\237", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        label->setText(QApplication::translate("BSLClass", "\350\257\267\345\212\240\350\275\275\345\234\272\346\231\257\357\274\201", Q_NULLPTR));
        menu_F->setTitle(QApplication::translate("BSLClass", "\346\226\207\344\273\266[&F]", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class BSLClass: public Ui_BSLClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BSL_H
