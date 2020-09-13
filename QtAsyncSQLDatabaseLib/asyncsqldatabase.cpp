#include "asyncsqldatabase.h"

#include <QEventLoop>

AsyncSqlDatabase::AsyncSqlDatabase(const QString& connectionName,
                                         const DatabaseConnectionConfig& conf,
                                         QObject* parent) noexcept:
    QObject(parent), connectionName(connectionName), conf(conf)
{
    qRegisterMetaType<QVector<ColumnValueComparison>>("QVector<ColumnValueComparison>");
    qRegisterMetaType<QVector<QString>>("QVector<QString>");
}

void AsyncSqlDatabase::open() noexcept
{
    // Refer to Qt doc : https://doc.qt.io/qt-5/qthread.html
    thread = new QThread;
    worker = new SqlDatabase(connectionName, conf);
    worker->moveToThread(thread);

    connect(thread, &QThread::started, worker, &SqlDatabase::open);

    connect(this, &AsyncSqlDatabase::insertSignal, worker, &SqlDatabase::insert);
    connect(this, &AsyncSqlDatabase::updateSignal, worker, &SqlDatabase::update);
    connect(this, &AsyncSqlDatabase::removeSignal, worker, &SqlDatabase::remove);
    connect(this, &AsyncSqlDatabase::rawExecRequestSignal, worker, &SqlDatabase::rawExecRequest);

    connect(this, &AsyncSqlDatabase::closeSignal, worker, &SqlDatabase::close);
    connect(this, &AsyncSqlDatabase::askForACK, worker, &SqlDatabase::answerACK);

    connect(thread, &QThread::finished, worker, &QObject::deleteLater);
    connect(thread, &QThread::finished, thread, &QObject::deleteLater);
    thread->start();
}

void AsyncSqlDatabase::close() noexcept
{
    emit closeSignal();
    wait();
    thread->quit();
    thread->wait();
}

void AsyncSqlDatabase::insert(const QString &into, const QVector<QString> &values) const noexcept
{
    emit insertSignal(into, values);
}

void AsyncSqlDatabase::update(const QString& table,
            const QVector<ColumnValueComparison>& set,
            const QVector<ColumnValueComparison>& where) const noexcept
{
    emit updateSignal(table, set, where);
}

void AsyncSqlDatabase::remove(const QString& from,
                                 const QVector<ColumnValueComparison>& where) const noexcept
{
    emit removeSignal(from, where);
}

void AsyncSqlDatabase::rawExecRequest(const QString &query) const noexcept
{
    emit rawExecRequestSignal(query);
}

void AsyncSqlDatabase::wait() const noexcept
{
    // When the thread processes the sent signal, all other signals prior to it
    // have been taken care of.
    // https://forum.qt.io/topic/92337/synchronously-waiting-for-a-list-of-signals/
    QEventLoop waitLoop;

    auto connection {connect(worker, &SqlDatabase::ACK,
                             this, [&waitLoop]()
    {
        waitLoop.quit();
    }, Qt::QueuedConnection)};

    emit askForACK();
    waitLoop.exec();
    disconnect(connection);
}
