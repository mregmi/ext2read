/**
 * Ext2read
 * File: ext2explore.cpp
 **/
/**
 * Copyright (C) 2005, 2010 by Manish Regmi   (regmi dot manish at gmail.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the
 * Free Software Foundation, Inc.,
 * 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 **/

#include "ext2explore.h"
#include "ui_ext2explore.h"

Ext2Explore::Ext2Explore(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Ext2Explore)
{
    splitter = new QSplitter(Qt::Horizontal, parent);
 //   setCentralWidget(splitter);

    filemodel = new QStandardItemModel(this);

    //fs_treeview->setModel(filemodel);
    //fs_listview->setModel(filemodel);

   // splitter->addWidget(ui->fs_treeview);
    //splitter->addWidget(ui->fs_listview);
   // fs_listview->setResizeMode(QlistView::AllColumns);

    app = new Ext2Read();

    ui->setupUi(this);

}

Ext2Explore::~Ext2Explore()
{
    delete ui;
    delete splitter;
    //delete fs_treeview;
    //delete fs_listview;
    delete filemodel;

    delete app;
}

void Ext2Explore::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
