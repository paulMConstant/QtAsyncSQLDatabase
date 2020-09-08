#include "include/databaseworker.h"

#include <QDebug>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>

namespace  // Non-member utility functions
{
QString addWhere(QString query, const QVector<ColumnValueComparison>& where) noexcept
{
    for (int i = 0; i < where.size(); ++i)
    {
        if (i == 0)
        {
            query += " WHERE ";
        }
        query += QString("%1 %2 ?").arg(where[i].col, where[i].stringCmp());
        if (i != where.size() - 1)
        {
            query += " AND ";
        }
    }
    return query;
}

QStringList execRequest(QSqlQuery query) noexcept
{
    if (query.exec() == false)
    {
        qDebug() << "Database Error :" << query.lastError() << "on query" << query.executedQuery();
        return {};
    }

    if (query.isSelect())
    {
        auto res {QStringList()};
        while (query.next())
        {
            for (int i = 0; i < query.record().count(); ++i)
            {
                res << query.value(i).toString();
            }
        }
        return res;
    }
    else
    {
        return {};
    }
}
}

DatabaseWorker::DatabaseWorker(const QString& connectionName,
                               const DatabaseConnectionConfig& conf,
                               QObject* parent) noexcept:
    QObject(parent), connectionName(connectionName)
{
    auto database {QSqlDatabase::addDatabase(conf.driver, connectionName)};
    database.setDatabaseName(conf.databaseName);
    database.setUserName(conf.userName);
    database.setPassword(conf.password);
    database.setHostName(conf.hostName);
    database.setPort(conf.port);
    database.setConnectOptions(conf.connectOptions);
}

DatabaseWorker::~DatabaseWorker() noexcept
{
    close();
    QSqlDatabase::removeDatabase(connectionName);
}

void DatabaseWorker::open() const noexcept
{
    auto database {db()};
    if (database.isOpen())
    {
        return;
    }
    if (database.open() == false)
    {
        qDebug() << "Cannot open the database. Not writing nor fetching data.\n";
    }
}

void DatabaseWorker::close() const noexcept
{
    auto database {db()};
    if (database.isOpen() == false)
    {
        return;
    }
    db().close();
}

QStringList DatabaseWorker::rawExecRequest(const QString& query) const noexcept
{
    return execRequest(QSqlQuery(query, db()));
}

void DatabaseWorker::emitACK() const noexcept
{
    // When this event is received, all prior events/requests have been processed
    emit ACK();
}

QStringList DatabaseWorker::select(const QVector<QString>& what,
                                   const QString& from,
                                   const QVector<ColumnValueComparison>& where) const noexcept
{
    // Build the query
    auto queryPrepare {QString("SELECT ")};
    for (int i = 0; i < what.size(); ++i)
    {
        queryPrepare += what[i];
        if (i < what.size() - 1)
        {
            queryPrepare += ',';
        }
    }
    queryPrepare += QString(" FROM %1").arg(from);
    queryPrepare = addWhere(queryPrepare, where);
    auto query {QSqlQuery(db())};
    query.prepare(queryPrepare);

    // Bind the values
    for (const auto& w : where)
    {
        query.addBindValue(w.value);
    }
    return execRequest(query);
}

void DatabaseWorker::insert(const QString& into,
                            const QVector<QString>& values) const noexcept
{
    // Build the query
    auto queryPrepare {QString("INSERT INTO %1 VALUES (").arg(into)};
    for (int i = 0; i < values.size(); ++i)
    {
        queryPrepare += '?';
        if (i != values.size() - 1)
        {
            queryPrepare += ',';
        }
    }
    queryPrepare += ')';

    auto query {QSqlQuery(db())};
    query.prepare(queryPrepare);

    // Bind the values
    for (const auto& value : values)
    {
        query.addBindValue(value);
    }
    execRequest(query);
}

void DatabaseWorker::update(const QString& table,
                            const QVector<ColumnValueComparison>& set,
                            const QVector<ColumnValueComparison>& where) const noexcept
{
    // Build the query
    auto queryPrepare {QString("UPDATE %1 SET ").arg(table)};
    for (int i = 0; i < set.size(); ++i)
    {
        queryPrepare += QString("%1 = ?").arg(set[i].col);
        if (i < set.size() - 1)
        {
            queryPrepare += ',';
        }
    }

    queryPrepare = addWhere(queryPrepare, where);
    auto query {QSqlQuery(db())};
    query.prepare(queryPrepare);

    // Bind the values
    for (const auto& s : set)
    {
        query.addBindValue(s.value);
    }
    for (const auto& w : where)
    {
        query.addBindValue(w.value);
    }
    execRequest(query);
}

void DatabaseWorker::remove(const QString& from,
                            const QVector<ColumnValueComparison>& where) const noexcept
{
    // Build the query
    auto queryPrepare {QString("DELETE FROM %1").arg(from)};
    queryPrepare = addWhere(queryPrepare, where);

    auto query {QSqlQuery(db())};
    query.prepare(queryPrepare);

    // Bind the values
    for (const auto& w : where)
    {
        query.addBindValue(w.value);
    }
    execRequest(query);
}
