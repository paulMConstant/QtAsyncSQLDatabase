#ifndef TST_QUERYTHREAD_H
#define TST_QUERYTHREAD_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include <QFile>

#include "asyncsqldatabase.h"

using namespace testing;

class AsyncDatabaseWorkerTest : public Test
{
protected:
    void SetUp() override
    {
    }

    void TearDown() override
    {
        asyncWorker.close();
    }

    constexpr static auto databaseName {"testQSQLITE2.db"};
    constexpr static auto driver {"QSQLITE"};
    const static DatabaseConnectionConfig conf;

    constexpr static auto asyncConnectionName {"TestAsyncConnection"};
    constexpr static auto syncConnectionName {"TestSyncConnection"};
    constexpr static auto tableName {"TestTable"};
    constexpr static auto idField {"id"};
    constexpr static auto nameField {"name"};

    AsyncSqlDatabase asyncWorker {asyncConnectionName, conf};
    // Select is implemented for synchronous access only
    // Therefore use a standard DatabaseWorker for select
    SqlDatabase syncWorker {syncConnectionName, conf};
};

const DatabaseConnectionConfig AsyncDatabaseWorkerTest::conf = DatabaseConnectionConfig(databaseName, driver);

TEST_F(AsyncDatabaseWorkerTest, OpenSQLiteDatabase)
{
    QFile::remove(databaseName);
    EXPECT_FALSE(QFile::exists(databaseName));
    asyncWorker.open();
    asyncWorker.wait();
    ASSERT_TRUE(QFile::exists(databaseName));
}

TEST_F(AsyncDatabaseWorkerTest, InsertElement)
{
    asyncWorker.open();
    asyncWorker.rawExecRequest(QString("CREATE TABLE ") + tableName
                            + '(' + idField + " int PRIMARY KEY, "
                            + nameField + " varchar(255))");
    asyncWorker.insert(tableName, {"0", "Name0"});
    asyncWorker.insert(tableName, {"1", "Name1"});
    asyncWorker.insert(tableName, {"2", "Name2"});
    asyncWorker.wait();

    syncWorker.open();
    const auto select {syncWorker.select({idField, nameField}, tableName, {{}})};
    ASSERT_EQ(select.size(), 6);
    ASSERT_EQ(select[0], "0");
    ASSERT_EQ(select[1], "Name0");
    ASSERT_EQ(select[2], "1");
    ASSERT_EQ(select[3], "Name1");
    ASSERT_EQ(select[4], "2");
    ASSERT_EQ(select[5], "Name2");
}

TEST_F(AsyncDatabaseWorkerTest, UpdateElement)
{
    const auto selectBefore {syncWorker.select({nameField}, tableName, {{idField, "0"}})};
    ASSERT_EQ(selectBefore.size(), 1);
    EXPECT_EQ(selectBefore.first(), "Name0");

    asyncWorker.open();
    asyncWorker.update(tableName, {{nameField, "NewName0"}}, {{nameField, "Name0"}});
    asyncWorker.wait();

    const auto selectAfter {syncWorker.select({nameField}, tableName, {{idField, "0"}})};
    ASSERT_EQ(selectAfter.size(), 1);
    ASSERT_EQ(selectAfter.first(), "NewName0");
}

TEST_F(AsyncDatabaseWorkerTest, RemoveElement)
{
    const auto selectBefore {syncWorker.select({"*"}, tableName, {{idField, "0"}})};
    // Size is 2 : id and name
    ASSERT_EQ(selectBefore.size(), 2);

    asyncWorker.open();
    asyncWorker.remove(tableName, {{idField, "0"}});
    asyncWorker.wait();
    const auto selectAfter {syncWorker.select({"*"}, tableName, {{idField, "0"}})};
    ASSERT_EQ(selectAfter.size(), 0);
}

// If the tests succeeded, the slots are well connected.

#endif // TST_QUERYTHREAD_H
