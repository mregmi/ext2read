#include <QtGui/QApplication>
#include "ext2explore.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Q_INIT_RESOURCE(ext2explore);
    log_init();
    Ext2Explore w;
    w.show();
    return a.exec();
}
