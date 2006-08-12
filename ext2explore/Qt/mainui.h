/**
 * Ext2read
 * File: mainui.h
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

#ifndef __MAIN_UI_H
#define __MAIN_UI_H

#include "ui_mainui.h"

#include "viewmodel.h"

#include <QtGui>

class MainUi : public QMainWindow
{
	Q_OBJECT

public:
	MainUi(QWidget *parent = 0);
	~MainUi();

	void ScanSystem();
private:


private:
	ViewModel *model;
	QListView *list;
	QTreeView *tree;

	Ui::MainWindow win;
};

#endif  //__MAIN_UI_H
