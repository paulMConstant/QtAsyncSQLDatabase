QT -= gui
QT += sql

TEMPLATE = lib
DEFINES += QTASYNCSQLDATABASE_LIBRARY

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/columnvaluecomparison.cpp \
    src/databaseworker.cpp \
    src/qtasyncsqldatabase.cpp \
    src/querythread.cpp

HEADERS += \
    "include/columnvaluecomparison.h" \
    "include/databaseconnectionconfig.h" \
    "include/databaseworker.h" \
    "include/qtasyncsqldatabase.h" \
    "include/querythread.h"

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
