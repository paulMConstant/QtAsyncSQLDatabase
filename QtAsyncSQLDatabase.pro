TEMPLATE = subdirs

SUBDIRS = \
        Lib \
        Test

Lib.subdir = QtAsyncSQLDatabaseLib
Test.subdir = QtAsyncSQLDatabaseTest

app.depends = Lib
