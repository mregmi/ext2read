#ifndef EXT2ABOUT_H
#define EXT2ABOUT_H

#include <QDialog>
#include "ui_about.h"

class Ext2About : public QDialog
{
    Q_OBJECT
public:
    Ext2About(QWidget *parent = 0);
    Ui::AboutDialog *about;
};

#endif // EXT2ABOUT_H
