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

#include <QDateTime>
#include "ext2properties.h"

Ext2Properties::Ext2Properties(QWidget *parent)
            : QDialog(parent)
{
    properties = new Ui::PropertiesDialog();
    properties->setupUi(this);
}

void Ext2Properties::set_properties(Ext2File *file)
{
    lloff_t sizeblock;
    uint16_t mode = file->inode.i_mode;
    QString filename = QString::fromStdString(file->file_name);
    QString  sizestr;
    QString szstrblock;

    properties->fileName->setText(filename);
    set_mode(mode);

    properties->typeval->setText(QString::fromUtf8(get_type_string(file->file_type)));

    sizeblock = ((file->file_size + file->partition->get_blocksize() - 1) /
                 file->partition->get_blocksize()) * file->partition->get_blocksize();
    if(file->file_size < 1024)
    {

        sizestr = QString("%1 Bytes").arg(file->file_size);
        szstrblock = QString("%1 Bytes").arg(sizeblock);

    }
    else if ((file->file_size >= 1024) && (file->file_size < (1024 * 1024)))
    {
        sizestr = QString("%1 KB (%L2 Bytes)").arg((double)file->file_size/1024, 0, 'g', 3).arg(file->file_size);
        szstrblock = QString("%1 KB (%L2 Bytes)").arg((double)sizeblock/1024, 0, 'g', 3).arg(sizeblock);
    }
    else if ((file->file_size >= (1024 * 1024)) && (file->file_size < (1024 * 1024 * 1024)))
    {
        sizestr = QString("%1 MB (%L2 Bytes)").arg((double)file->file_size/(1024 * 1024), 0, 'g', 3).arg(file->file_size);
        szstrblock = QString("%1 MB (%L2 Bytes)").arg((double)sizeblock/(1024 * 1024), 0, 'g', 3).arg(sizeblock);
    }
    else if (file->file_size >= (1024 * 1024 *1024))
    {
        sizestr = QString("%1 GB (%L2 Bytes)").arg((double)file->file_size/(1024 * 1024 * 1024), 0, 'g', 3).arg(file->file_size);
        szstrblock = QString("%1 GB (%L2 Bytes)").arg((double)sizeblock/(1024 * 1024 * 1024), 0, 'g', 3).arg(sizeblock);
    }

    properties->sizeval->setText(sizestr);
    properties->szdiskval->setText(szstrblock);

    QDateTime time;
    time.setTime_t(file->inode.i_ctime);
    properties->createdval->setText(time.toString());

    time.setTime_t(file->inode.i_mtime);
    properties->modifiedval->setText(time.toString());

    time.setTime_t(file->inode.i_atime);
    properties->accessedval->setText(time.toString());
}

void Ext2Properties::set_mode(uint16_t mode)
{
    if(mode & EXT2_S_IRUSR)
    {
        properties->check_or->setChecked(true);
    }
    else
    {
        properties->check_or->setChecked(false);
    }
    if(mode & EXT2_S_IWUSR)
    {
        properties->check_ow->setChecked(true);
    }
    else
    {
        properties->check_ow->setChecked(false);
    }
    if(mode & EXT2_S_IXUSR)
    {
        properties->check_ox->setChecked(true);
    }
    else
    {
        properties->check_ox->setChecked(false);
    }

    if(mode & EXT2_S_IRGRP)
    {
        properties->check_gr->setChecked(true);
    }
    else
    {
        properties->check_gr->setChecked(false);
    }
    if(mode & EXT2_S_IWGRP)
    {
        properties->check_gw->setChecked(true);
    }
    else
    {
        properties->check_gw->setChecked(false);
    }
    if(mode & EXT2_S_IXGRP)
    {
        properties->check_ge->setChecked(true);
    }
    else
    {
        properties->check_ge->setChecked(false);
    }

    if(mode & EXT2_S_IROTH)
    {
        properties->check_otr->setChecked(true);
    }
    else
    {
        properties->check_otr->setChecked(false);
    }
    if(mode & EXT2_S_IWOTH)
    {
        properties->check_otw->setChecked(true);
    }
    else
    {
        properties->check_otw->setChecked(false);
    }
    if(mode & EXT2_S_IXOTH)
    {
        properties->check_ote->setChecked(true);
    }
    else
    {
        properties->check_ote->setChecked(false);
    }
}

void Ext2Properties::on_check_or_toggled(bool checked)
{
    // DONT do anything
}
