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
