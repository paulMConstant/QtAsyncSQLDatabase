#include "include/qtasyncsqldatabase.h"

QtAsyncSQLDatabase::QtAsyncSQLDatabase(const DatabaseConnectionConfig& conf) noexcept:
    conf(conf)
{
    Q_ASSERT_X(conf.databaseName.isNull() == false, "QtAsyncSQLDatabase::QtAsyncSQLDatabase",
               "dbName in configuration must be initialized");
}

QtAsyncSQLDatabase::~QtAsyncSQLDatabase() noexcept
{
    close();
}

void QtAsyncSQLDatabase::open() noexcept
{
    if (dbIsOpen)
    {
        return;
    }
    dbIsOpen = true;
    syncWorker.open();
    asyncWorker.start();
}

void QtAsyncSQLDatabase::close() noexcept
{
    if (dbIsOpen == false)
    {
        return;
    }
    dbIsOpen = false;
    syncWorker.close();
    asyncWorker.stop();
    asyncWorker.wait();
}

void QtAsyncSQLDatabase::setReadOnly(bool b) noexcept
{
    readOnly = b;
}

bool QtAsyncSQLDatabase::isReadOnly() const noexcept
{
    return readOnly == false;
}

QString QtAsyncSQLDatabase::getDatabaseName() const noexcept
{
    return conf.databaseName;
}

QStringList QtAsyncSQLDatabase::select(const QVector<QString>& what,
                                       const QString& from,
                                       const QVector<ColumnValueComparison>& where) const noexcept
{
    return syncWorker.select(what, from, where);
}

void QtAsyncSQLDatabase::insert(const QString& into, const QVector<QString>& values) const noexcept
{
    if (readOnly)
    {
        return;
    }
    syncWorker.insert(into, values);
}

void QtAsyncSQLDatabase::update(const QString& table,
                                const QVector<ColumnValueComparison>& set,
                                const QVector<ColumnValueComparison>& where) const noexcept
{
    if (readOnly)
    {
        return;
    }
    syncWorker.update(table, set, where);
}

void QtAsyncSQLDatabase::remove(const QString& from,
                                const QVector<ColumnValueComparison>& where) const noexcept
{
    if (readOnly)
    {
        return;
    }
    syncWorker.remove(from, where);
}

void QtAsyncSQLDatabase::rawExecRequest(const QString& query) const noexcept
{
    if (readOnly)
    {
        return;
    }
    syncWorker.rawExecRequest(query);
}

void QtAsyncSQLDatabase::asyncInsert(const QString& into,
                                     const QVector<QString>& values) const noexcept
{
    if (readOnly)
    {
        return;
    }
    asyncWorker.insert(into, values);
}

void QtAsyncSQLDatabase::asyncUpdate(const QString& table,
                                     const QVector<ColumnValueComparison>& set,
                                     const QVector<ColumnValueComparison>& where) const noexcept
{
    if (readOnly)
    {
        return;
    }
    asyncWorker.update(table, set, where);
}

void QtAsyncSQLDatabase::asyncRemove(const QString& from,
                                     const QVector<ColumnValueComparison>& where) const noexcept
{
    if (readOnly)
    {
        return;
    }
    asyncWorker.remove(from, where);
}

void QtAsyncSQLDatabase::asyncRawExecRequest(const QString& query) const noexcept
{
    if (readOnly)
    {
        return;
    }
    asyncWorker.rawExecRequest(query);
}
