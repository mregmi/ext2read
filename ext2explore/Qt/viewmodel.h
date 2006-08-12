/**
 * Ext2read
 * File: viewmodel.h
 **/
/**
 * Copyright (C) 2005 by Manish Regmi   (regmi dot manish at gmail.com)
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
/**
 * Describes the Model for the QT's View Model architecture.
 **/

#ifndef __VIEW_MODEL_H
#define __VIEW_MODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>


#include <QObject>
#include "ext2read.h"


class TreeItem
{

public:
	TreeItem(const DIRENTRY &entry, TreeItem *p = 0);
	~TreeItem();

	void appendChild(TreeItem *child);

	TreeItem *getChild(int row);
	int childCount() const;
	int columnCount() const;
	QVariant data(int column) const;
	int row() const;
	TreeItem *getParent();

private:
	TreeItem *parent;
	QList<TreeItem *> child;
	DIRENTRY fileData;
};

class ViewModel : public QAbstractItemModel
{
	Q_OBJECT

public:
	ViewModel(DIRENTRY *entry, int nEntries, QObject *parent = 0);
	~ViewModel();

	QVariant data(const QModelIndex &index, int role) const;
	Qt::ItemFlags flags(const QModelIndex &index) const;
	QVariant headerData(int section, Qt::Orientation orientation,
				int role = Qt::DisplayRole) const;
	QModelIndex index(int row, int column,
				const QModelIndex &parent = QModelIndex()) const;
	QModelIndex parent(const QModelIndex &index) const;
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent = QModelIndex()) const;

private:
	TreeItem *root;
	TreeItem *listroot;

};

#endif  // __VIEW_MODEL_H

