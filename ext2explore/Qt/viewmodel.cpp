/**
 * Ext2read
 * File: viewmodel.cpp
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
 * Implements the Model for the QT's View Model architecture.
 **/

#include "viewmodel.h"

/* Constructor */
TreeItem::TreeItem(const DIRENTRY &entry, TreeItem *p)
{
	fileData = entry;
	parent = p;
}

/* Destructor: Delete all the children */
TreeItem::~TreeItem()
{
	qDeleteAll(child);
}

/* Add a children */
void TreeItem::appendChild(TreeItem *item)
{
	child.append(item);
}

/* Returns the child that corresponds to row number */
TreeItem *TreeItem::getChild(int row)
{
	return child.value(row);
}

/* returns the number of children */
int TreeItem::childCount() const
{
	return child.count();
}

/* returns the item's location (row number) within its parent's list of items */
int TreeItem::row() const
{
	if (parent)
		return parent->child.indexOf(const_cast<TreeItem *>(this));

	return 0;
}


/* Return the Number of Columns. This is fixed for our app for name, size, type and create date */
int TreeItem::columnCount() const
{
	return 3;
}

/* Return the data given by column number */
QVariant TreeItem::data(int column) const
{
	QString data = "";

	switch(column)
	{
	case 0:
		data = fileData.fileName;
		break;
//	case 1:
//		data.
	}
}


/* return the item's parent*/
TreeItem *TreeItem::getParent()
{
	return parent;
}


/* Constructor for model: */
ViewModel::ViewModel(DIRENTRY *entry, int nEntries, QObject *parent)
	: QAbstractItemModel(parent)
{
	TreeItem *item;
	DIRENTRY dummy;

	memset(&dummy, 0, sizeof(dummy));

	root = new TreeItem(dummy);

	listroot = root;

	for(int i = 0; i < nEntries; i++)
	{
		LOG("Creating item %s \n", entry[i].fileName);
		item = new TreeItem(entry[i], root);
		root->appendChild(item);
	}
}

/* Destructor */
ViewModel::~ViewModel()
{
	delete root;
}


QModelIndex ViewModel::index(int row, int column, const QModelIndex &parent) const
{
	TreeItem *parentItem;

	if (!parent.isValid())
            parentItem = root;
        else
            parentItem = static_cast<TreeItem *>(parent.internalPointer());

	TreeItem *childItem = parentItem->getChild(row);
	if (childItem)
		return createIndex(row, column, childItem);
	else
		return QModelIndex();
}


QModelIndex ViewModel::parent(const QModelIndex &index) const
{
	if (!index.isValid())
		return QModelIndex();

	TreeItem *childItem = static_cast<TreeItem *>(index.internalPointer());
	TreeItem *parentItem = childItem->getParent();

	if (parentItem == root)
		return QModelIndex();

	return createIndex(parentItem->row(), 0, parentItem);
}


int ViewModel::rowCount(const QModelIndex &parent) const
{
	TreeItem *parentItem;

	if (!parent.isValid())
		parentItem = root;
	else
		parentItem = static_cast<TreeItem *>(parent.internalPointer());

	return parentItem->childCount();
}


int ViewModel::columnCount(const QModelIndex &parent) const
{
	if (parent.isValid())
		return static_cast<TreeItem *>(parent.internalPointer())->columnCount();
	else
		return root->columnCount();
}


QVariant ViewModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	if (role != Qt::DisplayRole)
		return QVariant();

	TreeItem *item = static_cast<TreeItem *>(index.internalPointer());

	return item->data(index.column());
}


Qt::ItemFlags ViewModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return Qt::ItemIsEnabled;

	return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}


QVariant ViewModel::headerData(int section, Qt::Orientation orientation,
								int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
		return root->data(section);

	return QVariant();
}
