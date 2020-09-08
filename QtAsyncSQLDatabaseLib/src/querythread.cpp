#include "include/querythread.h"

#include <QEventLoop>

QueryThread::QueryThread(const DatabaseConnectionConfig& conf, QObject* parent) noexcept :
    QThread(parent), conf(conf)
{
    // Register meta types for signals
    qRegisterMetaType<QVector<ColumnValueComparison>>("QVector<ColumnValueComparison>");
    qRegisterMetaType<QVector<QString>>("QVector<QString>");
}

void QueryThread::stop() noexcept
{
    emit closeWorker();
    waitForWorkerToBeDone();
    if (dbWorker != nullptr)
    {
        dbWorker->deleteLater();
    }
    exit(0);
}

void QueryThread::waitForWorkerToBeDone() const noexcept
{
    // When the dbWorker processes the sent signal, all events prior to it
    // have been taken care of.
    // https://forum.qt.io/topic/92337/synchronously-waiting-for-a-list-of-signals/
    auto signalCount {1};
    QEventLoop waitLoop;

    auto connection {connect(dbWorker, &DatabaseWorker::ACK,
                             this, [&signalCount, &waitLoop]()
    {
        if (--signalCount == 0)
        {
            waitLoop.quit();
        }
    }, Qt::QueuedConnection)};

    emit askForWorkerACK();
    waitLoop.exec();

    disconnect(connection);
}

void QueryThread::run()
{
    // Create dbWorker object within the context of the new thread
    // Memory is freed on stop(). Must be called 'by hand' or there will be a memory leak.
    const auto connectionName = QString::number((qlonglong)(QThread::currentThreadId()));
    dbWorker = new DatabaseWorker(connectionName, conf);
    dbWorker->open();

    connect(this, &QueryThread::closeWorker, dbWorker, &DatabaseWorker::close, Qt::QueuedConnection);
    connect(this, &QueryThread::insert, dbWorker, &DatabaseWorker::insert, Qt::QueuedConnection);
    connect(this, &QueryThread::update, dbWorker, &DatabaseWorker::update, Qt::QueuedConnection);
    connect(this, &QueryThread::remove, dbWorker, &DatabaseWorker::remove, Qt::QueuedConnection);
    connect(this, &QueryThread::rawExecRequest, dbWorker, &DatabaseWorker::rawExecRequest,
            Qt::QueuedConnection);
    connect(this, &QueryThread::askForWorkerACK, dbWorker, &DatabaseWorker::emitACK,
            Qt::QueuedConnection);

    exec();  // our event loop
}
