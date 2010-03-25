#include "ext2about.h"

Ext2About::Ext2About(QWidget *parent)
        : QDialog(parent)
{
    about = new Ui::AboutDialog();
    about->setupUi(this);
}
