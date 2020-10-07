cmake_minimum_required(VERSION 3.18)

project(QtAsyncSQLDatabaseLib CXX)

set(CMAKE_AUTOMOC ON)

find_package(Qt5Core REQUIRED)
find_package(Qt5Sql REQUIRED)

include_directories(${PROJECT_SOURCE_DIR})

set(SRCS
    asyncsqldatabase.cpp
    columnvaluecomparison.cpp
    sqldatabase.cpp
    )

set(HEADERS
    asyncsqldatabase.h
    columnvaluecomparison.h
    sqldatabase.h
    sqldatabaseconnectionconfig.h
    )

add_library(QtAsyncSQLDatabaseLib SHARED ${SRCS} ${HEADERS})
target_link_libraries(QtAsyncSQLDatabaseLib Qt5::Core Qt5::Sql)
