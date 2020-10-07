cmake_minimum_required(VERSION 3.18)

project(QtAsyncSQLDatabaseTest CXX)

find_package(Qt5Sql REQUIRED)
find_package(Qt5Core REQUIRED)

include_directories(${PROJECT_SOURCE_DIR} ../src/)
LINK_DIRECTORIES(../src/build)

set(SRCS main.cpp)
set(HEADERS tst_asyncsqldatabase tst_sqldatabase)

add_executable(QtAsyncSQLDatabaseTest ${SRCS})
target_link_libraries(QtAsyncSQLDatabaseTest
    Qt5::Core
    Qt5::Sql
    QtAsyncSQLDatabaseLib
    gtest
    gmock)

add_test(
    NAME QtAsyncSQLDatabaseTest
        COMMAND ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}/QtAsyncSQLDatabaseTest
)
