/********************************************************************************
** Form generated from reading UI file 'ext2explore.ui'
**
** Created: Wed Mar 17 22:51:35 2010
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EXT2EXPLORE_H
#define UI_EXT2EXPLORE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QListView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QTreeWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Ext2Explore
{
public:
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QTreeWidget *fs_treeview;
    QListView *fs_listview;
    QMenuBar *menuBar;
    QStatusBar *statusBar;
    QToolBar *mainToolBar;

    void setupUi(QMainWindow *Ext2Explore)
    {
        if (Ext2Explore->objectName().isEmpty())
            Ext2Explore->setObjectName(QString::fromUtf8("Ext2Explore"));
        Ext2Explore->resize(600, 400);
        centralWidget = new QWidget(Ext2Explore);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        fs_treeview = new QTreeWidget(centralWidget);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QString::fromUtf8("1"));
        fs_treeview->setHeaderItem(__qtreewidgetitem);
        fs_treeview->setObjectName(QString::fromUtf8("fs_treeview"));

        horizontalLayout->addWidget(fs_treeview);

        fs_listview = new QListView(centralWidget);
        fs_listview->setObjectName(QString::fromUtf8("fs_listview"));

        horizontalLayout->addWidget(fs_listview);

        Ext2Explore->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(Ext2Explore);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 21));
        Ext2Explore->setMenuBar(menuBar);
        statusBar = new QStatusBar(Ext2Explore);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        Ext2Explore->setStatusBar(statusBar);
        mainToolBar = new QToolBar(Ext2Explore);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        Ext2Explore->addToolBar(Qt::TopToolBarArea, mainToolBar);

        mainToolBar->addSeparator();

        retranslateUi(Ext2Explore);

        QMetaObject::connectSlotsByName(Ext2Explore);
    } // setupUi

    void retranslateUi(QMainWindow *Ext2Explore)
    {
        Ext2Explore->setWindowTitle(QApplication::translate("Ext2Explore", "Ext2Explore", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Ext2Explore: public Ui_Ext2Explore {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EXT2EXPLORE_H
