# -------------------------------------------------
# Project created by QtCreator 2010-03-15T21:59:31
# -------------------------------------------------
TARGET = ext2explore
TEMPLATE = app
SOURCES += main.cpp \
    ext2explore.cpp \
    ext2read.cpp \
    platform_win32.c \
    platform_unix.c \
    log.c \
    ext2fs.cpp \
    lvm.cpp
HEADERS += ext2explore.h \
    ../platform.h \
    ../parttypes.h \
    ../lvm.h \
    ../ext2read.h \
    ../ext2fs.h \
    platform.h \
    parttypes.h \
    lvm.h \
    ext2read.h \
    ext2fs.h \
    ext2explore.h \
    partition.h
FORMS += ext2explore.ui
RESOURCES += ext2explore.qrc
