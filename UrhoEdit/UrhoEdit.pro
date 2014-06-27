#-------------------------------------------------
#
# Project created by QtCreator 2014-06-20T22:33:53
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = UrhoEdit
TEMPLATE = app


DESTDIR = ../../UrhoEditBin
MOC_DIR = ../../UrhoEditBin/.moc
UI_DIR = ../../UrhoEditBin/.uic
RCC_DIR = ../../UrhoEditBin/.rcc
OBJECTS_DIR = ../../UrhoEditBin/.obj


SOURCES += main.cpp\
        MainWindow.cpp \
    NewResourceDialog.cpp

HEADERS  += MainWindow.h \
    NewResourceDialog.h

FORMS    += MainWindow.ui \
    NewResourceDialog.ui

RESOURCES += \
    res.qrc

macx {
    TARGET = UrhoEdit
    QMAKE_INFO_PLIST = Resources/Info.plist
    ICON = Resources/Images/icon.icns
}
