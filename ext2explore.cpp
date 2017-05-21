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
#include <QMessageBox>

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

    if(app->get_detected_disks() <= 0)
    {
        QMessageBox::information(this, "Cannot Read Disk",
                                 "Unable to read disk. Please make sure you are running this application as an Administrator.",
                                 QMessageBox::Ok);
    }

    ui->setupUi(this);

    filemodel->sort(0);
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
    connect(ui->list, SIGNAL(clicked(QModelIndex)), this, SLOT(on_action_item_clicked(const QModelIndex &)));
    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(ext2_context_menu(const QPoint &)));

    codec = QTextCodec::codecForName("utf-8");
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
    //QByteArray ba;

    if(!parent->hasChildren())
        return;

    nrows = parent->rowCount();
    for(int i = 0; i < nrows; i++)
    {
        item = parent->child(i);
        if(!item)
            continue;
        //ba = item->text().toAscii();
        //const char *c_str2 = ba.data();
        //LOG("Deleting %s\n", c_str2);

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
    QString text;
    void *ptr;

    parts = app->get_partitions();
    for(i = parts.begin(); i != parts.end(); i++)
    {
        temp = (*i);

        // check if it is already in the view
        if(temp->onview)
            continue;

        text = QString("%1 [%2]").arg(temp->get_linux_name().c_str(), temp->get_volume_name().c_str());
        item = new QStandardItem(QIcon(QString(":/icons/resource/disk.png")), text);
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
        str = QString(":/icons/resource/file_folder.png");
    else
        str = QString(":/icons/resource/file_unknown.png");

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
    delete ui;
    delete filemodel;
    delete app;

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
        break;  // FIXME: Getting the first selected item only.
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
                                            tr("Open Disk Image"), QString(""), tr("All Disk Image Files (*)"));
    //LOG("Opening file %s as a disk image. \n", filename.toAscii());

    result = app->add_loopback(filename.toUtf8());
    if(result <= 0)
    {
        LOG("No valid Ext2 Partitions found in the disk image.");
        return;
    }

    init_root_fs();
}

void Ext2Explore::on_action_item_clicked(const QModelIndex &index)
{
    QStandardItem *parentItem;
    QVariant fileData;
    Ext2File *parentFile;

    parentItem = filemodel->itemFromIndex(index);
    fileData = parentItem->data(Qt::UserRole);
    parentFile = (Ext2File *) fileData.value<void *>();

    statusBar()->showMessage(QString(parentFile->file_name.c_str()));
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
    if(!EXT2_S_ISDIR(parentFile->inode.i_mode)) {
        LOG("File %s is not a directory\n", parentFile->file_name.c_str());
        return;
    }

    ui->list->setRootIndex(index);
    if(parentFile->onview)
        return;

    part = parentFile->partition;

    LOG("Opened file %s\n",parentFile->file_name.c_str());
    dir = part->open_dir(parentFile);
    while((files = part->read_dir(dir)) != NULL)
    {
        LOG("Found File %s inode %d \n", files->file_name.c_str(), files->inode_num);

        children = new QStandardItem(QIcon(handle_mime(files->file_name, files->inode.i_mode)),
                                     codec->toUnicode(files->file_name.c_str()));
        children->setData(qVariantFromValue((void *)files), Qt::UserRole);
        children->setEditable(false);
        parentItem->appendRow(children);
        parentFile->onview = true;
    }

    part->close_dir(dir);
    parentItem->sortChildren(0);
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

    if(EXT2_S_ISDIR(file->inode.i_mode))
    {
        filename = QFileDialog::getExistingDirectory(this, tr("Save folder in"),
                                                     QString(""),
                                                     QFileDialog::ShowDirsOnly);
    }
    else
    {
        filename = QFileDialog::getSaveFileName(this, tr("Save File/Folder"),
                                                QString::fromStdString(file->file_name),
                                                tr("All Files (*)"));
    }
    if(filename.isEmpty())
        return;

    copyfile.set_file(file);
    copyfile.set_name(filename);
    copyfile.start_copy();
}
