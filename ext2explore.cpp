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

#include <QFileDialog>

#include "ext2explore.h"
#include "ui_ext2explore.h"

Ext2Explore::Ext2Explore(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Ext2Explore)
{
    filemodel = new QStandardItemModel(this);
    app = new Ext2Read();

    ui->setupUi(this);

    ui->tree->setModel(filemodel);
    ui->list->setModel(filemodel);
    root = filemodel->invisibleRootItem();
}

Ext2Explore::~Ext2Explore()
{
    delete ui;
    delete filemodel;
    delete app;
}

void Ext2Explore::init_root_fs()
{
    Ext2Partition *temp;
    list<Ext2Partition *> parts;
    list<Ext2Partition *>::iterator i;
    QStandardItem *item;

    for(i = parts.begin(); i != parts.end(); i++)
    {
        temp = (*i);
        item = new QStandardItem()
    }
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

void Ext2Explore::on_action_Exit_triggered()
{
    close();
}

void Ext2Explore::on_actionP_roperties_triggered()
{
    property.show();
}

void Ext2Explore::on_action_About_triggered()
{
    about.show();
}

void Ext2Explore::on_action_Rescan_System_triggered()
{
    delete app;

    app = new Ext2Read();
}

void Ext2Explore::on_actionOpen_Image_triggered()
{
    QString filename;

    filename = QFileDialog::getOpenFileName(this,
         tr("Open Disk Image"), "", tr("All Disk Image Files (*)"));
    //LOG("Opening file %s as a disk image. \n", filename.toAscii());

    app->add_loopback(filename.toAscii());
}
