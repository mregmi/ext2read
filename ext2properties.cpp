#include "ext2properties.h"

Ext2Properties::Ext2Properties(Ext2File *file):QDialog()
{
    properties = new Ui::PropertiesDialog();
    properties->setupUi(this);
}
