#include <QtGui/QApplication>
#include "ext2explore.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Ext2Explore w;
    w.show();
    return a.exec();
}
