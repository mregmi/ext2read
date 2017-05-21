# Ext2Read

**Ext2Read** is an explorer like utility to explore ext2/ext3/ext4 files.
It runs directly without any installation.
It can be used to view and copy files and folders.
It can recursively copy entire folders.
It can also be used to view and copy disk and file.
It now supports LVM2 and EXT4 extents. 

# Features
- Simple UI designed using Qt4/Qt5
- View/Read ext2/ext3/ext4 partitions
- Linux LVM2 Support
- Ext4 Large File support (untested)
- Recursively Copy the entire folder or even `/`
- Support for external USB disks
- Support for disk and filesystem  images
  For e.g. Wubi users can just open their root.disk file through this program
- LRU based Block cache for faster access
- Unicode support

# Usage

The executables and sources can be downloaded from http://ext2read.sf.net

## Notice
- This program must be run as Administrator in order to directly access partitions. If you are not automatically asked to elevate, right-click the file and select _Run as Administrator_.
- This is not a transparent file system driver, just a user space tool.
- The LVM2 metadata can be complex because of the wide variety of configuration possibilities. All configurations has not been tested. If LVM2 does not work in your system, please file a bug with your LVM2 metadata.

# Contributing

If you find any bugs, have any questions or comments, please let us know.

If any of you are interested in joing this project, you can join the mailing list and discuss. Join from here https://lists.sourceforge.net/lists/listinfo/ext2read-devel

## Building

### Windows

Building for Windows requires _MinGW_ compiler.

**NOTE:** Either copy the dependent Qt DLLs into the same folder as the built `ext2explore.exe` or setup your PATH environment variable correctly!
Common mistakes are other PATH entries which bundle an old version of Qt or the wrong bitness (32/64 Bit).

#### Windows (32bit)

Qt officially supports MinGW 32bit, so just download their official release, run Qt Creator and build.

#### Windows (64bit)

Qt does not officially support MinGW 64bit. Therefor we need to setup the toolchain on our own:
1. Download and extract a compiler: [MinGW-W64 (e.g. posix-seh)](https://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win64/Personal%20Builds/mingw-builds/)
2. Download and extract prebuilt Qt libraries: [Qt64-NG (e.g. posix-seh)](https://sourceforge.net/projects/qt64ng/files/qt/x86-64/)  
   **NOTE:** Unfortunately this project is closed after Qt version 5.5.0. If you need a newer version, [build it yourself](https://wiki.qt.io/Building_Qt_Desktop_for_Windows_with_MinGW#Build_Qt_with_MinGW_for_a_x64_.28x86_64.29_target).
   1. Run `qtbinpatcher.exe` whenever you move your folder.
3. Download and install [Qt-Creator](https://www.qt.io/download-open-source/#section-2)
4. [Setup Qt Creator](http://doc.qt.io/qtcreator/creator-configuring.html#checking-build-and-run-settings)
   in **Tools > Options > Build & Run**
   1. Add Compiler (C: `gcc.exe`, C++: `g++.exe`, 32/64 Bit: select _ABI_)
   2. Add Debugger (`gdb.exe`)
   3. Add Qt version (`qmake.exe`)
   4. Add Kit

Afterwards, you can start the build process.
