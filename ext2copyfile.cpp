#include <QMessageBox>

#include "ext2copyfile.h"

Ext2CopyFile::Ext2CopyFile(Ext2File *parent, QString &dest)
{
    filename = dest;
    file = parent;

    blksize = parent->partition->get_blocksize();
    buffer = new char [blksize];
    filetosave = NULL;
    cancelOperation = false;
}

Ext2CopyFile::~Ext2CopyFile()
{
    delete [] buffer;
    if(filetosave)
        delete filetosave;
}

bool Ext2CopyFile::copy_file(QString &destfile, Ext2File *srcfile)
{
    lloff_t blocks, blkindex;
    int extra;
    int ret;

    filetosave = new QFile(destfile);
    if (!filetosave->open(QIODevice::ReadWrite | QIODevice::Truncate))
    {
        LOG("Error creating file %s.\n", srcfile->file_name.c_str());
        return false;
    }

    blocks = srcfile->file_size / blksize;
    for(blkindex = 0; blkindex < blocks; blkindex++)
    {
        if(cancelOperation)
        {
            return showMessageBox();
        }
        ret = srcfile->partition->read_data_block(&srcfile->inode, blkindex, buffer);
        if(ret < 0)
        {
            filetosave->close();
            return false;
        }
        filetosave->write(buffer, blksize);
    }

    extra = srcfile->file_size % blksize;
    if(extra)
    {
        ret = srcfile->partition->read_data_block(&srcfile->inode, blkindex, buffer);
        if(ret < 0)
        {
            filetosave->close();
            return false;
        }
        filetosave->write(buffer, extra);
    }
    filetosave->close();
    return true;
}

bool Ext2CopyFile::showMessageBox()
{
    QMessageBox msgBox;
    msgBox.setText("You pressed the cancel button on the progress dialog box.");
    msgBox.setInformativeText("Are you sure you want to cancel copying?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    int ret = msgBox.exec();

    switch(ret) {
    case QMessageBox::Yes:
        return true;
    case QMessageBox::No:
        return false;
    }

    return false;
}

void Ext2CopyFile::on_buttonBox_accepted()
{
    cancelOperation = true;
}
