/********************************************************************************
** Form generated from reading UI file 'test_gui.ui'
**
** Created by: Qt User Interface Compiler version 5.1.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TEST_GUI_H
#define UI_TEST_GUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_test_guiClass
{
public:
    QWidget *centralWidget;
    QPushButton *emitDataButton;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QDoubleSpinBox *spin1_lat;
    QLabel *label;
    QVBoxLayout *verticalLayout_2;
    QDoubleSpinBox *spin2_lon;
    QLabel *label_2;
    QVBoxLayout *verticalLayout_3;
    QDoubleSpinBox *spin3_alt;
    QLabel *label_3;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *test_guiClass)
    {
        if (test_guiClass->objectName().isEmpty())
            test_guiClass->setObjectName(QStringLiteral("test_guiClass"));
        test_guiClass->resize(600, 143);
        centralWidget = new QWidget(test_guiClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        emitDataButton = new QPushButton(centralWidget);
        emitDataButton->setObjectName(QStringLiteral("emitDataButton"));
        emitDataButton->setGeometry(QRect(220, 70, 161, 31));
        layoutWidget = new QWidget(centralWidget);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(11, 11, 581, 43));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        spin1_lat = new QDoubleSpinBox(layoutWidget);
        spin1_lat->setObjectName(QStringLiteral("spin1_lat"));
        spin1_lat->setDecimals(6);
        spin1_lat->setMinimum(-90);
        spin1_lat->setMaximum(90);

        verticalLayout->addWidget(spin1_lat);

        label = new QLabel(layoutWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label);


        horizontalLayout->addLayout(verticalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        spin2_lon = new QDoubleSpinBox(layoutWidget);
        spin2_lon->setObjectName(QStringLiteral("spin2_lon"));
        spin2_lon->setDecimals(6);
        spin2_lon->setMinimum(-180);
        spin2_lon->setMaximum(180);

        verticalLayout_2->addWidget(spin2_lon);

        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(label_2);


        horizontalLayout->addLayout(verticalLayout_2);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        spin3_alt = new QDoubleSpinBox(layoutWidget);
        spin3_alt->setObjectName(QStringLiteral("spin3_alt"));
        spin3_alt->setDecimals(6);
        spin3_alt->setMaximum(100000);

        verticalLayout_3->addWidget(spin3_alt);

        label_3 = new QLabel(layoutWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(label_3);


        horizontalLayout->addLayout(verticalLayout_3);

        test_guiClass->setCentralWidget(centralWidget);
        mainToolBar = new QToolBar(test_guiClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        test_guiClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(test_guiClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        test_guiClass->setStatusBar(statusBar);

        retranslateUi(test_guiClass);

        QMetaObject::connectSlotsByName(test_guiClass);
    } // setupUi

    void retranslateUi(QMainWindow *test_guiClass)
    {
        test_guiClass->setWindowTitle(QApplication::translate("test_guiClass", "test_gui", 0));
        emitDataButton->setText(QApplication::translate("test_guiClass", "EMIT DATA", 0));
        spin1_lat->setSuffix(QApplication::translate("test_guiClass", " deg", 0));
        label->setText(QApplication::translate("test_guiClass", "Latitude", 0));
        spin2_lon->setSuffix(QApplication::translate("test_guiClass", " deg", 0));
        label_2->setText(QApplication::translate("test_guiClass", "Longitude", 0));
        spin3_alt->setSpecialValueText(QString());
        spin3_alt->setSuffix(QApplication::translate("test_guiClass", " m", 0));
        label_3->setText(QApplication::translate("test_guiClass", "Altitude", 0));
    } // retranslateUi

};

namespace Ui {
    class test_guiClass: public Ui_test_guiClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TEST_GUI_H
