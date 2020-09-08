include(gtest_dependency.pri)

TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG += thread
CONFIG += qt

QT += core sql

HEADERS += \
    tst_databaseworker.h \
    "../QtAsyncSQLDatabaseLib/include/databaseworker.h" \
    "../QtAsyncSQLDatabaseLib/include/querythread.h" \
    "../QtAsyncSQLDatabaseLib/include/columnvaluecomparison.h"

SOURCES += \
        main.cpp \
        ../QtAsyncSQLDatabaseLib/src/databaseworker.cpp \
        ../QtAsyncSQLDatabaseLib/src/querythread.cpp \
        ../QtAsyncSQLDatabaseLib/src/columnvaluecomparison.cpp

INCLUDEPATH += \
        ../QtAsyncSQLDatabaseLib/
