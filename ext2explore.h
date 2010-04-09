/**
 * Ext2read
 * File: ext2explore.h
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


#ifndef EXT2EXPLORE_H
#define EXT2EXPLORE_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QListView>
#include <QTreeView>
#include <QSplitter>
#include <QTextCodec>

#include "ext2read.h"
#include "ext2properties.h"
#include "ext2about.h"
#include "ext2copyfile.h"

namespace Ui {
    class Ext2Explore;
}

class Ext2Explore : public QMainWindow {
    Q_OBJECT
public:
    Ext2Explore(QWidget *parent = 0);
    ~Ext2Explore();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::Ext2Explore *ui;
    Ext2Properties property;
    Ext2About about;
    Ext2CopyFile copyfile;

    QItemSelectionModel *selectionModel;
    QStandardItemModel *filemodel;
    QStandardItem *root;
    Ext2Read *app;
    QTextCodec *codec;

    void init_root_fs();
    QString handle_mime(string file, uint16_t mode);
    void delete_children(QStandardItem *parent);
    void delete_file_tree();

private slots:
    void on_action_Save_triggered();
    void on_actionOpen_Image_triggered();
    void on_action_Rescan_System_triggered();
    void on_action_About_triggered();
    void on_actionP_roperties_triggered();
    void on_action_Exit_triggered();
    void on_action_item_dbclicked(const QModelIndex &index);
    void ext2_context_menu(const QPoint &);
};

#endif // EXT2EXPLORE_H
