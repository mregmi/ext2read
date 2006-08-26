TEMPLATE	= app
LANGUAGE	= C++

CONFIG	+= qt warn_on debug

INCLUDEPATH	+= ./ ../ ../../core/ ../../core/platform ../../core/ext2 ../../core/lvm

HEADERS += 	mainui.h viewmodel.h

SOURCES	+=	main.cpp 				\
		mainui.cpp				\
		viewmodel.cpp				\
		../../core/partition.c 			\
		../../core/log.c 			\
		../../core/platform/platform_unix.c 	\
		../../core/platform/platform_win32.c 	\
		../../core/ext2/ext2io.c 		\
		../../core/ext2/inode.c 		\
		../../core/ext2/mount.c 		\
		../../core/ext2/namei.c 		\
		../../core/lvm/lvm.c 			\

FORMS	= mainui.ui properties.ui

RESOURCES = ext2explore.qrc

RC_FILE	 = app.rc

TARGET = ext2explore

unix {
  UI_DIR = user_interface
  MOC_DIR = meta_objects
  OBJECTS_DIR = objects
}

win32 {
  UI_DIR = user_interface
  MOC_DIR = meta_objects
  OBJECTS_DIR = objects
}