/**
 * Ext2read
 * File: main.c
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
 * This is the entry point for GUI in QT.
 **/

#include "mainui.h"
#include "viewmodel.h"
#include "ext2read.h"

#include <QApplication>
#include <QDir>

extern "C" struct sys sys;

int main(int argc, char *argv[])
{
	DIRENTRY *entry;
	QApplication app(argc, argv);

	Q_INIT_RESOURCE(ext2explore);

	MainUi mainWin;
	entry = mainWin.ScanSystem();
	mainWin.model = new ViewModel(entry,sys.linuxparts);
	mainWin.tree->setModel(mainWin.model);
	mainWin.show();

	return app.exec();
}

