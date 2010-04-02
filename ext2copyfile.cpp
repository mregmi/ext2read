#include <QMessageBox>
#include <QDir>

#include "ext2copyfile.h"

Ext2CopyFile::Ext2CopyFile(Ext2File *parent, QString &dest)
{
    filename = dest;
    file = parent;
    cancelOperation = false;

    progress = new Ui::ProgressDialog();
    progress->setupUi(this);
    progress->progressBar->setMaximum(100);
    progress->progressBar->setMinimum(0);
}

Ext2CopyFile::~Ext2CopyFile()
{
    delete progress;
    delete proc;
}

void Ext2CopyFile::start_copy()
{
    proc = new Ext2CopyProcess(file, filename);
    qRegisterMetaType<QString>("QString&");
    QObject::connect(proc, SIGNAL(sig_updateui(QString&,QString&,QString&,int,int)),
            this, SLOT(slot_updateui(QString&,QString&,QString&,int,int)));
    QObject::connect(proc, SIGNAL(sig_copydone()), this, SLOT(slot_copydone()));
    QObject::connect(this, SIGNAL(signal_cancelprocess()), proc, SLOT(slot_cancelprocess()));
    this->show();
    proc->start();
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

void Ext2CopyFile::slot_updateui(QString &file, QString &from, QString &to, int copied, int total)
{
    int value;

    if(!file.isEmpty())
        progress->FileLabel->setText(file);
    if(!from.isEmpty())
        progress->Fromlabel->setText(from);
    if(!to.isEmpty())
        progress->ToLabel->setText(to);

    value = (copied * 100) / total;
    progress->progressBar->setValue(value);
}

void Ext2CopyFile::slot_copydone()
{
    delete this;
}

Ext2CopyProcess::Ext2CopyProcess(Ext2File *parent, QString &dest)
    : QThread()
{
    filename = dest;
    file = parent;
    blksize = parent->partition->get_blocksize();
    buffer = new char [blksize];
    filetosave = NULL;
    cancelOperation = false;
}

Ext2CopyProcess::~Ext2CopyProcess()
{
    delete [] buffer;
    if(filetosave)
        delete filetosave;
}

void Ext2CopyProcess::slot_cancelprocess()
{
    cancelOperation = true;
}

void Ext2CopyProcess::run()
{
    if(EXT2_S_ISDIR(file->inode.i_mode))
    {
        copy_folder(filename, file);
        emit sig_copydone();
        return ;
    }
    else if(!EXT2_S_ISREG(file->inode.i_mode))
        return ;

    copy_file(filename, file);
    emit sig_copydone();
}

bool Ext2CopyProcess::copy_file(QString &destfile, Ext2File *srcfile)
{
    lloff_t blocks, blkindex;
    QString qsrc;
    QString nullstr("");
    QByteArray ba;
    int extra;
    int ret;

    filetosave = new QFile(destfile);
    if (!filetosave->open(QIODevice::ReadWrite | QIODevice::Truncate))
    {
        LOG("Error creating file %s.\n", srcfile->file_name.c_str());
        return false;
    }
    //ba = destfile.toAscii();
    //const char *c_str2 = ba.data();

    //LOG("Copying file %s as %s\n", srcfile->file_name.c_str(), c_str2);
    qsrc = srcfile->file_name.c_str();
    blocks = srcfile->file_size / blksize;
    for(blkindex = 0; blkindex < blocks; blkindex++)
    {
        if(cancelOperation)
        {
            return false;
        }
        ret = srcfile->partition->read_data_block(&srcfile->inode, blkindex, buffer);
        if(ret < 0)
        {
            filetosave->close();
            return false;
        }
        filetosave->write(buffer, blksize);
        this->curcopied = blkindex;
        this->curtotal = blocks;
        emit sig_updateui(qsrc, destfile, filename, (int)blkindex, (int)blocks);
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

bool Ext2CopyProcess::copy_folder(QString &path, Ext2File *parent)
{
    QDir dir(path);
    QString filetosave;
    QString rootname = path;
    EXT2DIRENT *dirent;
    Ext2Partition *part = parent->partition;
    Ext2File *child;
    QByteArray ba;
    bool ret;


    if(!EXT2_S_ISDIR(parent->inode.i_mode))
        return false;

    dir.mkdir(QString(parent->file_name.c_str()));
    ba = path.toAscii();
    const char *c_str2 = ba.data();
    LOG("Creating Folder %s as %s\n", parent->file_name.c_str(), c_str2);

    dirent = part->open_dir(parent);
    while((child = part->read_dir(dirent)) != NULL)
    {
        filetosave = rootname;
        filetosave.append("/");
        filetosave.append(parent->file_name.c_str());
        if(EXT2_S_ISDIR(child->inode.i_mode))
        {

            ret = copy_folder(filetosave, child);
            if((ret == false) && (cancelOperation == true))
            {
                part->close_dir(dirent);
                return false;
            }
            continue;
        }
        else if(!EXT2_S_ISREG(child->inode.i_mode))
        {
            part->close_dir(dirent);
            return false;
        }

        filetosave.append("/");
        filetosave.append(child->file_name.c_str());
        ret = copy_file(filetosave, child);
        if((ret == false) && (cancelOperation == true))
        {
            part->close_dir(dirent);
            return false;
        }
    }
    return true;
}



void Ext2CopyFile::on_buttonBox_clicked(QAbstractButton* button)
{
     cancelOperation = showMessageBox();
     if(cancelOperation)
         emit signal_cancelprocess();
}
