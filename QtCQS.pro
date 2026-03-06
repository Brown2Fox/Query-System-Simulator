#-------------------------------------------------
#
# Project created by QtCreator 2015-11-04T00:42:24
#
#-------------------------------------------------

QT       += core gui

CONFIG += c++23

greaterThan(QT_MAJOR_VERSION, 5): QT += widgets

TARGET = QtCQS
TEMPLATE = app

INCLUDEPATH += include \

SOURCES += main.cpp \
            QsClientScope.cpp \
            QsEvent.cpp \
            QsEventScope.cpp \
            QsQuery.cpp \
            QsQueueScope.cpp \
            QsRes.cpp \
            QsServerScope.cpp \
            QsTime.cpp \
            QsSettings.cpp \
            QsForm_Main.cpp \
            QsForm_Settings.cpp \
            QsThreadCore.cpp

HEADERS  += \
            QsScope \
            QsClientScope.hpp \
            QsEvent.hpp \
            QsEventScope.hpp \
            QsQuery.hpp \
            QsQueueScope.hpp \
            QsRes.hpp \
            QsServerScope.hpp \
            QsTime.hpp \
            QsForm_Main.hpp \
            QsForm_Settings.hpp \
            QsThreadCore.hpp \
            QsSettings.hpp




FORMS    += \
    QsForm_Main.ui \
    QsForm_Settings.ui


