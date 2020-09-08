#ifndef TST_DATABASEWORKER_H
#define TST_DATABASEWORKER_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include "include/databaseworker.h"
#include "include/databaseconnectionconfig.h"

#include <QFile>

constexpr static auto databaseName {"testQSQLITE.db"};
constexpr static auto driver {"QSQLITE"};
const DatabaseConnectionConfig conf {databaseName, driver};

constexpr static auto connectionName {"TestConnection"};
constexpr static auto tableName {"TestTable"};
constexpr static auto idField {"id"};
constexpr static auto nameField {"name"};

using namespace testing;

class DatabaseWorkerTest : public Test
{
protected:
    void SetUp() override
    {
    }

    void TearDown() override
    {
    }

    DatabaseWorker* dbWorker {new DatabaseWorker(connectionName, conf)};
};

TEST_F(DatabaseWorkerTest, SQLiteDriverAvailable)
{
    ASSERT_TRUE(QSqlDatabase::isDriverAvailable(driver));
}

TEST_F(DatabaseWorkerTest, OpenSQLiteDatabase)
{
    QFile::remove(databaseName);
    EXPECT_FALSE(QFile::exists(databaseName));

    dbWorker->open();
    ASSERT_TRUE(QFile::exists(databaseName));
    ASSERT_TRUE(QSqlDatabase::database(connectionName).isValid());
    ASSERT_TRUE(QSqlDatabase::database(connectionName).isOpen());
}

TEST_F(DatabaseWorkerTest, InsertAndRetrieveElement)
{
    dbWorker->rawExecRequest(QString("CREATE TABLE ") + tableName
                            + '(' + idField + " int PRIMARY KEY, "
                            + nameField + " varchar(255))");
    dbWorker->insert(tableName, {"0", "Paul"});
    dbWorker->insert(tableName, {"1", "Emile"});
    dbWorker->insert(tableName, {"2", "Charlotte"});

    const auto select {dbWorker->select({idField, nameField}, tableName, {{}})};
    ASSERT_EQ(select.size(), 6);
    ASSERT_EQ(select[0], "0");
    ASSERT_EQ(select[1], "Paul");
    ASSERT_EQ(select[2], "1");
    ASSERT_EQ(select[3], "Emile");
    ASSERT_EQ(select[4], "2");
    ASSERT_EQ(select[5], "Charlotte");
}

TEST_F(DatabaseWorkerTest, SelectWhereEqual)
{
    // Comparison for Where uses Operator::eq by default
    const auto select {dbWorker->select({nameField}, tableName, {{"id", "1"}})};
    ASSERT_EQ(select.size(), 1);
    ASSERT_EQ(select.first(), "Emile");
}

TEST_F(DatabaseWorkerTest, SelectWhereLessThan)
{
    const auto select {dbWorker->select({idField}, tableName, {{idField, "2", Operator::lt}})};
    ASSERT_EQ(select.size(), 2);
    ASSERT_TRUE(select.contains("1"));
    ASSERT_TRUE(select.contains("0"));
}

TEST_F(DatabaseWorkerTest, SelectWhereLesserEqual)
{
    const auto select {dbWorker->select({idField}, tableName, {{idField, "1", Operator::le}})};
    ASSERT_EQ(select.size(), 2);
    ASSERT_TRUE(select.contains("1"));
    ASSERT_TRUE(select.contains("0"));
}

TEST_F(DatabaseWorkerTest, SelectWhereNotEqual)
{
    const auto select {dbWorker->select({idField}, tableName, {{idField, "2", Operator::ne}})};
    ASSERT_EQ(select.size(), 2);
    ASSERT_TRUE(select.contains("1"));
    ASSERT_TRUE(select.contains("0"));
}

TEST_F(DatabaseWorkerTest, SelectWhereGreaterThan)
{
    const auto select {dbWorker->select({idField}, tableName, {{idField, "0", Operator::gt}})};
    ASSERT_EQ(select.size(), 2);
    ASSERT_TRUE(select.contains("1"));
    ASSERT_TRUE(select.contains("2"));
}

TEST_F(DatabaseWorkerTest, SelectWhereGreaterEqual)
{
    const auto select {dbWorker->select({idField}, tableName, {{idField, "1", Operator::ge}})};
    ASSERT_EQ(select.size(), 2);
    ASSERT_TRUE(select.contains("1"));
    ASSERT_TRUE(select.contains("2"));
}

TEST_F(DatabaseWorkerTest, UpdateElement)
{
    const auto selectBefore {dbWorker->select({nameField}, tableName, {{idField, "0"}})};
    ASSERT_EQ(selectBefore.size(), 1);
    EXPECT_EQ(selectBefore.first(), "Paul");

    dbWorker->update(tableName, {{nameField, "Jules"}}, {{nameField, "Paul"}});

    const auto selectAfter {dbWorker->select({nameField}, tableName, {{idField, "0"}})};
    ASSERT_EQ(selectAfter.size(), 1);
    ASSERT_EQ(selectAfter.first(), "Jules");
}

TEST_F(DatabaseWorkerTest, RemoveElement)
{
    const auto selectBefore {dbWorker->select({"*"}, tableName, {{idField, "0"}})};
    // Size is 2 : id and name
    ASSERT_EQ(selectBefore.size(), 2);

    dbWorker->remove(tableName, {{idField, "0"}});
    const auto selectAfter {dbWorker->select({"*"}, tableName, {{idField, "0"}})};
    ASSERT_EQ(selectAfter.size(), 0);
}

TEST_F(DatabaseWorkerTest, CloseSQLiteDatabase)
{
    dbWorker->close();
    ASSERT_FALSE(QSqlDatabase::database(connectionName, false).isOpen());
}

TEST_F(DatabaseWorkerTest, CleanDeletion)
{
    EXPECT_TRUE(QSqlDatabase::database(connectionName).isValid());
    delete dbWorker;
    ASSERT_FALSE(QSqlDatabase::database(connectionName).isValid());
}

#endif // TST_DATABASEWORKER_H
