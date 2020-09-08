#ifndef DATABASEWORKER_H
#define DATABASEWORKER_H

#include <QObject>
#include <QVector>
#include <QString>
#include <QSqlDatabase>
#include <QMutex>
#include <QMetaType>

#include <atomic>

#include "columnvaluecomparison.h"
#include "databaseconnectionconfig.h"

/*!
 * \brief Builds and executes SQL queries.
 * Owns a database connection.
 */
class DatabaseWorker : public QObject
{
    Q_OBJECT
  public:
    DatabaseWorker() = delete;
    explicit DatabaseWorker(const QString& connectionName,
                            const DatabaseConnectionConfig& conf,
                            QObject* parent = nullptr) noexcept;
    ~DatabaseWorker() noexcept;


    [[nodiscard]] QStringList select(const QVector<QString>& what,
                                     const QString& from,
                                     const QVector<ColumnValueComparison>& where) const noexcept;

  public slots:
    void open() const noexcept;
    void close() const noexcept;
    void insert(const QString& into, const QVector<QString>& values) const noexcept;
    void update(const QString& table,
                const QVector<ColumnValueComparison>& set,
                const QVector<ColumnValueComparison>& where) const noexcept;
    void remove(const QString& from, const QVector<ColumnValueComparison>& where) const noexcept;
    QStringList rawExecRequest(const QString& query) const noexcept;

    void emitACK() const noexcept;

  signals:
    void ACK() const;

  private:
    const QString connectionName;

    inline QSqlDatabase db() const noexcept {return QSqlDatabase::database(connectionName);}
};

#endif  // DATABASEWORKER_H
