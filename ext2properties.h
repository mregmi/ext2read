/**
 * Copyright (C) 2010 by Manish Regmi   (regmi dot manish at gmail.com)
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

#ifndef EXT2PROPERTIES_H
#define EXT2PROPERTIES_H

#include <QDialog>

#include "ui_properties.h"
#include "ext2read.h"

class Ext2Properties : public QDialog
{
     Q_OBJECT
public:
    Ext2Properties(QWidget *parent = 0);
    void set_properties(Ext2File *file);
    void set_mode(uint16_t mode);

private:
    Ui::PropertiesDialog *properties;

private slots:
    void on_check_or_toggled(bool checked);
};

#endif // EXT2PROPERTIES_H
