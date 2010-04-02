#ifndef EXT2COPYFILE_H
#define EXT2COPYFILE_H

#include <QDialog>
#include <QFile>
#include <QThread>

#include "ui_ext2progress.h"
#include "ext2read.h"

class Ext2CopyProcess : public QThread
{
    Q_OBJECT
private:
    QString filename;
    Ext2File *file;
    QFile *filetosave;
    char *buffer;
    int blksize;
    bool cancelOperation;

    bool copy_folder(QString &path, Ext2File *file);
    bool copy_file(QString &destfile, Ext2File *srcfile);

protected:
    void run();

public:
    Ext2CopyProcess(Ext2File *parent, QString &dest);
    ~Ext2CopyProcess();

public slots:
    void slot_cancelprocess();

signals:
    void sig_updateui(QString &file, QString &from, QString &to, int copied, int total);
    void sig_copydone();
};

class Ext2CopyFile : public QDialog
{
    Q_OBJECT

    Ui::ProgressDialog *progress;
    QString filename;
    Ext2File *file;
    bool cancelOperation;
    Ext2CopyProcess *proc;

public:
    Ext2CopyFile(Ext2File *parent, QString &dest);
    ~Ext2CopyFile();

    void start_copy();

private:
    bool showMessageBox();

private slots:
    void on_buttonBox_clicked(QAbstractButton* button);

public slots:
    void slot_updateui(QString &file, QString &from, QString &to, int copied, int total);
    void slot_copydone();

signals:
    void signal_cancelprocess();
};

#endif // EXT2COPYFILE_H
