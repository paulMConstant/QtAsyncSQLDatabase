#include "sqldatabase.h"

#include <QDebug>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>

SqlDatabase::SqlDatabase(const QString& connectionName,
                               const DatabaseConnectionConfig& conf,
                               QObject* parent) noexcept:
    QObject(parent), connectionName(connectionName), conf(conf)
{
}

void SqlDatabase::open() const noexcept
{
    auto database {QSqlDatabase::addDatabase(conf.driver, connectionName)};
    database.setDatabaseName(conf.databaseName);
    database.setUserName(conf.userName);
    database.setPassword(conf.password);
    database.setHostName(conf.hostName);
    database.setPort(conf.port);
    database.setConnectOptions(conf.connectOptions);
    // Create the database file
    database.open();
}

void SqlDatabase::close() const noexcept
{
    QSqlDatabase::removeDatabase(connectionName);
}

QStringList SqlDatabase::select(const QVector<QString>& what,
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

void SqlDatabase::insert(const QString& into,
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

void SqlDatabase::update(const QString& table,
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

void SqlDatabase::remove(const QString& from,
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

QStringList SqlDatabase::rawExecRequest(const QString& query) const noexcept
{
    auto sqlQuery {QSqlQuery(db())};
    sqlQuery.prepare(query);
    return execRequest(sqlQuery);
}

QString SqlDatabase::addWhere(QString query, const QVector<ColumnValueComparison>& where) noexcept
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

QStringList SqlDatabase::execRequest(QSqlQuery query) noexcept
{
    if (query.exec() == false)
    {
        qDebug() << "SqlDatabase Error :" << query.lastError() << "on query" << query.executedQuery();
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

void SqlDatabase::answerACK() const noexcept
{
    emit ACK();
}
