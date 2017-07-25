#-------------------------------------------------
#
# Project created by QtCreator 2016-09-09T11:16:42
#
#-------------------------------------------------

QT       += core gui
QT       += svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ogdf-test-2
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    graphprocessor.cpp \
    circuitbuilder.cpp \
    parser.cpp \
    drawingscene.cpp

HEADERS  += mainwindow.h \
    graphprocessor.h \
    circuitbuilder.h \
    parser.h \
    drawingscene.h

FORMS    += mainwindow.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../Downloads/ogdf.v2015.05/OGDF/Win32/release/ -logdf
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../Downloads/ogdf.v2015.05/OGDF/Win32/debug/ -logdf

INCLUDEPATH += $$PWD/../../Downloads/ogdf.v2015.05/OGDF/include
DEPENDPATH += $$PWD/../../Downloads/ogdf.v2015.05/OGDF/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../Downloads/ogdf.v2015.05/OGDF/Win32/release/libogdf.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../Downloads/ogdf.v2015.05/OGDF/Win32/debug/libogdf.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../Downloads/ogdf.v2015.05/OGDF/Win32/release/ogdf.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../Downloads/ogdf.v2015.05/OGDF/Win32/debug/ogdf.lib
