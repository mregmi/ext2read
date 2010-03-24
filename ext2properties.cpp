#include "ext2properties.h"

Ext2Properties::Ext2Properties(QWidget *parent)
            : QDialog(parent)
{
    properties = new Ui::PropertiesDialog();
    properties->setupUi(this);
}
