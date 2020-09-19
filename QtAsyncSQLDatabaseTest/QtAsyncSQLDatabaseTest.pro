include(gtest_dependency.pri)

TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG += thread
CONFIG += qt

QT += core sql

LIBS += -L$$OUT_PWD/../QtAsyncDatabaseLib -lQtAsyncSQLDatabaseLib

HEADERS += \
    tst_asyncsqldatabase.h \
    tst_sqldatabase.h \
    ../QtAsyncSQLDatabaseLib/sqldatabase.h \
    ../QtAsyncSQLDatabaseLib/asyncsqldatabase.h \
    ../QtAsyncSQLDatabaseLib/columnvaluecomparison.h

SOURCES += \
        main.cpp

INCLUDEPATH += \
        ../QtAsyncSQLDatabaseLib/
