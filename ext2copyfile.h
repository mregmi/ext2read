#ifndef EXT2COPYFILE_H
#define EXT2COPYFILE_H

#include <QDialog>
#include <QFile>

#include "ui_ext2progress.h"
#include "ext2read.h"

class Ext2CopyFile : public QDialog
{
    Q_OBJECT

    Ui::ProgressDialog *progress;
    QString filename;
    Ext2File *file;
    QFile *filetosave;
    char *buffer;
    int blksize;
    bool cancelOperation;

public:
    Ext2CopyFile(Ext2File *parent, QString &dest);
    ~Ext2CopyFile();

    void start_copy();

private:
    void copy_folder(QString path, Ext2File *file);
    bool copy_file(QString &destfile, Ext2File *srcfile);
    bool showMessageBox();

private slots:
    void on_buttonBox_accepted();
};

#endif // EXT2COPYFILE_H
