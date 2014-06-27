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
    NewResourceDialog.cpp \
    CodeEditor/addRemoveText.cpp \
    CodeEditor/backtraceWidget.cpp \
    CodeEditor/bookMark.cpp \
    CodeEditor/codeEditor.cpp \
    CodeEditor/highlighter.cpp \
    CodeEditor/locaterWidget.cpp \
    CodeEditor/miniMap.cpp \
    CodeEditor/searchWidget.cpp

HEADERS  += MainWindow.h \
    NewResourceDialog.h \
    CodeEditor/addRemoveText.h \
    CodeEditor/backtraceWidget.h \
    CodeEditor/bookMark.h \
    CodeEditor/codeEditor.h \
    CodeEditor/highlighter.h \
    CodeEditor/locaterWidget.h \
    CodeEditor/miniMap.h \
    CodeEditor/searchWidget.h

FORMS    += MainWindow.ui \
    NewResourceDialog.ui

RESOURCES += \
    res.qrc

macx {
    TARGET = UrhoEdit
    QMAKE_INFO_PLIST = Resources/Info.plist
    ICON = Resources/Images/icon.icns
}
