#ifndef QUERYTHREAD_H
#define QUERYTHREAD_H

#include <QObject>
#include <QThread>
#include <QVector>

#include "databaseworker.h"
#include "databaseconnectionconfig.h"

// Signal type declaration.
// For each metatype, qRegisterMetaType must be called at runtime.
Q_DECLARE_METATYPE(QVector<ColumnValueComparison>)
Q_DECLARE_METATYPE(QVector<QString>)

class QueryThread : public QThread
{
    Q_OBJECT
  public:
    QueryThread() = delete;
    explicit QueryThread(const DatabaseConnectionConfig& conf, QObject* parent = nullptr) noexcept;

    void stop() noexcept;
    void waitForWorkerToBeDone() const noexcept;

  protected:
    void run() override;

  signals:
    void insert(const QString& into, const QVector<QString>& values) const;
    void update(const QString& table,
                const QVector<ColumnValueComparison>& set,
                const QVector<ColumnValueComparison>& where) const;
    void remove(const QString& from, const QVector<ColumnValueComparison>& where) const;
    void rawExecRequest(const QString& query) const;
    void closeWorker();
    void askForWorkerACK() const;

  private:
    const DatabaseConnectionConfig conf;
    DatabaseWorker* dbWorker {nullptr};

};

#endif  // QUERYTHREAD_H
