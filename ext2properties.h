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

private:
    Ui::PropertiesDialog *properties;
};

#endif // EXT2PROPERTIES_H
