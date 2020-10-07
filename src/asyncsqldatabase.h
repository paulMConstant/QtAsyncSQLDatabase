#ifndef ASYNCSQLDATABASE_H
#define ASYNCSQLDATABASE_H

#include <QThread>

#include "sqldatabase.h"

Q_DECLARE_METATYPE(QVector<ColumnValueComparison>);
Q_DECLARE_METATYPE(QVector<QString>);

/*!
 * \brief Asynchronous database worker.
 * Manages a query thread which uses the base functions of a SqlDatabase.
 */
class QTASYNCSQLDATABASE_EXPORT AsyncSqlDatabase : public QObject
{
    Q_OBJECT
public:
    AsyncSqlDatabase() = delete;
    explicit AsyncSqlDatabase(const QString& connectionName,
                                 const SqlDatabaseConnectionConfig& conf,
                                 QObject* parent = nullptr) noexcept;

    void open() noexcept;
    void close() noexcept;
    void insert(const QString& into, const QVector<QString>& values) const noexcept;
    void update(const QString& table,
                const QVector<ColumnValueComparison>& set,
                const QVector<ColumnValueComparison>& where) const noexcept;
    void remove(const QString& from, const QVector<ColumnValueComparison>& where) const noexcept;
    void rawExecRequest(const QString& query) const noexcept;

    void wait() const noexcept;

signals:
    void insertSignal(const QString& into, const QVector<QString>& values) const ;
    void updateSignal(const QString& table,
                const QVector<ColumnValueComparison>& set,
                const QVector<ColumnValueComparison>& where) const ;
    void removeSignal(const QString& from, const QVector<ColumnValueComparison>& where) const ;
    void rawExecRequestSignal(const QString& query) const ;
    void askForACK() const;

    void closeSignal();

private:
    QString connectionName;
    SqlDatabaseConnectionConfig conf;

    QThread* thread;
    SqlDatabase* worker;
};

#endif // ASYNCSQLDATABASE_H
