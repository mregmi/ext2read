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
#include "ext2copyfile.h"

#include "ui_ext2explore.h"

Ext2Explore::Ext2Explore(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Ext2Explore)
{
    QRect rect;
    filemodel = new QStandardItemModel(this);
    app = new Ext2Read();

    ui->setupUi(this);

    ui->tree->setModel(filemodel);
    ui->tree->header()->hide();
    ui->tree->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tree->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->list->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->list->setSelectionMode( QAbstractItemView::SingleSelection );
    ui->list->setModel(filemodel);
    ui->list->setViewMode(QListView::IconMode);
    ui->list->setIconSize(QSize(50,60));
    ui->list->setMovement(QListView::Static);
    ui->list->setWordWrap(true);
    ui->list->setWrapping(true);

    root = filemodel->invisibleRootItem();
    selectionModel = ui->list->selectionModel();
    ui->tree->setSelectionModel(selectionModel);

    this->setContextMenuPolicy(Qt::CustomContextMenu);
    init_root_fs();

    connect(ui->tree, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(on_action_item_dbclicked(const QModelIndex &)));
    connect(ui->list, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(on_action_item_dbclicked(const QModelIndex &)));
    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(ext2_context_menu(const QPoint &)));

}

Ext2Explore::~Ext2Explore()
{    
    delete ui;
    delete filemodel;
    delete app;
}

void Ext2Explore::delete_children(QStandardItem *parent)
{
    int nrows;
    QVariant fileData;
    QStandardItem *item;
    Ext2File *file;
    QByteArray ba;

    if(!parent->hasChildren())
        return;

    nrows = parent->rowCount();
    for(int i = 0; i < nrows; i++)
    {
        item = parent->child(i);
        if(!item)
            continue;
        ba = item->text().toAscii();
        const char *c_str2 = ba.data();
        LOG("Deleting %s\n", c_str2);

        delete_children(item);
        fileData = item->data(Qt::UserRole);
        file = (Ext2File *) fileData.value<void *>();
        delete file;
    }
    parent->removeRows(0, nrows);
}

void Ext2Explore::init_root_fs()
{
    Ext2Partition *temp;
    list<Ext2Partition *> parts;
    list<Ext2Partition *>::iterator i;
    QStandardItem *item;
    void *ptr;

    parts = app->get_partitions();
    for(i = parts.begin(); i != parts.end(); i++)
    {
        temp = (*i);

        // check if it is already in the view
        if(temp->onview)
            continue;

        item = new QStandardItem(QIcon(":/icons/resource/disk.png"),
                                 QString(temp->get_linux_name().c_str()));
        if(!temp->get_root())
        {
            LOG("Root folder for %s is invalid. \n", temp->get_linux_name().c_str());
            delete item;
            continue;
        }

        ptr = temp->get_root();
        item->setData(qVariantFromValue(ptr), Qt::UserRole);
        item->setEditable(false);
        root->appendRow(item);

        temp->onview = true;
    }
}

// TODO: Handle more mime types according to file extension
QString Ext2Explore::handle_mime(string file, uint16_t mode)
{
    QString str;

    if(EXT2_S_ISDIR(mode))
        str = ":/icons/resource/file_folder.png";
    else
        str = ":/icons/resource/file_unknown.png";

    return str;
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
    QModelIndexList indexes = selectionModel->selectedIndexes();
    QModelIndex index;
    QStandardItem *item;
    QVariant fileData;
    Ext2File *file;

    foreach(index, indexes) {
        item = filemodel->itemFromIndex(index);
        fileData = item->data(Qt::UserRole);
        file = (Ext2File *) fileData.value<void *>();
        property.set_properties(file);
    }
    property.show();
}

void Ext2Explore::on_action_About_triggered()
{
    about.show();
}

void Ext2Explore::on_action_Rescan_System_triggered()
{
    delete_children(root);
    delete app;

    app = new Ext2Read();
    init_root_fs();
}

void Ext2Explore::on_actionOpen_Image_triggered()
{
    int result;
    QString filename;

    filename = QFileDialog::getOpenFileName(this,
         tr("Open Disk Image"), "", tr("All Disk Image Files (*)"));
    //LOG("Opening file %s as a disk image. \n", filename.toAscii());

    result = app->add_loopback(filename.toAscii());
    if(result <= 0)
    {
        LOG("No valid Ext2 Partitions found in the disk image.");
        return;
    }

    init_root_fs();
}

void Ext2Explore::on_action_item_dbclicked(const QModelIndex &index)
{
    QStandardItem *children;
    QStandardItem *parentItem;
    Ext2File *parentFile;
    Ext2File *files;
    QVariant fileData;
    EXT2DIRENT *dir;
    Ext2Partition *part;

    parentItem = filemodel->itemFromIndex(index);
    fileData = parentItem->data(Qt::UserRole);
    parentFile = (Ext2File *) fileData.value<void *>();

     ui->list->setRootIndex(index);
    if(parentFile->onview)
        return;

    part = parentFile->partition;

    LOG("Opened file %s\n",parentFile->file_name.c_str());
    dir = part->open_dir(parentFile);
    while((files = part->read_dir(dir)) != NULL)
    {
//        LOG("Found File %s inode %d \n", files->file_name.c_str(), files->inode_num);

        children = new QStandardItem(QIcon(handle_mime(files->file_name, files->inode.i_mode)),
                                 QString(files->file_name.c_str()));
        children->setData(qVariantFromValue((void *)files), Qt::UserRole);
        children->setEditable(false);
        parentItem->appendRow(children);
        parentFile->onview = true;
    }

    part->close_dir(dir);
}

void Ext2Explore::ext2_context_menu(const QPoint &point)
{
    QMenu menu(ui->splitter);

    menu.addAction(ui->action_Save);
    menu.addSeparator();
    menu.addAction(ui->actionCut);
    menu.addAction(ui->actionC_opy);
    menu.addAction(ui->actionPaste);
    menu.addSeparator();
    menu.addAction(ui->actionRename);
    menu.addAction(ui->actionDelete);
    menu.addSeparator();
    menu.addAction(ui->actionP_roperties);

    menu.exec(this->mapToGlobal(point));
}

void Ext2Explore::on_action_Save_triggered()
{
    QString filename;
    QModelIndexList indexes = selectionModel->selectedIndexes();
    QModelIndex index;
    QStandardItem *item;
    QVariant fileData;
    Ext2File *file;



    if(indexes.length() <= 0)
        return;

    index = indexes[0];

    item = filemodel->itemFromIndex(index);
    fileData = item->data(Qt::UserRole);
    file = (Ext2File *) fileData.value<void *>();

    filename = QFileDialog::getSaveFileName(this, tr("Save File/Folder"),
                                            "",
                                            tr("All Files (*.*)"));

    Ext2CopyFile cp(file, filename);
    cp.start_copy();
}
