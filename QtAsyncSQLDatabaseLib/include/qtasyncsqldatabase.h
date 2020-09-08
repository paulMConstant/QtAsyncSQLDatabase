#ifndef QTASYNCDATABASE_H
#define QTASYNCDATABASE_H

#include <QtCore/qglobal.h>

#if defined(QTASYNCSQLDATABASE_LIBRARY)
#  define QTASYNCSQLDATABASE_EXPORT Q_DECL_EXPORT
#else
#  define QTASYNCSQLDATABASE_EXPORT Q_DECL_IMPORT
#endif

#include <QVector>
#include <QString>
#include <QStringList>

#include "columnvaluecomparison.h"
#include "databaseconnectionconfig.h"
#include "databaseworker.h"
#include "querythread.h"

class QTASYNCSQLDATABASE_EXPORT QtAsyncSQLDatabase
{
  public:
    QtAsyncSQLDatabase() = delete;
    explicit QtAsyncSQLDatabase(const DatabaseConnectionConfig& conf) noexcept;
    ~QtAsyncSQLDatabase() noexcept;

    /*!
     * \brief Opens the synchronous and asynchronous connections to the database.
     */
    void open() noexcept;

    /*!
     * \brief Closes the synchronous and asynchronous connections to the database.
     */
    void close() noexcept;

    /*!
     * \brief Sets the database to read-only : all requests which are not SELECT will be ignored.
     * \param b true for read-only, false for read-write
     */
    void setReadOnly(bool b) noexcept;

    /*!
     * \brief isReadOnly
     * \return true if read only else false
     */
    [[nodiscard]] bool isReadOnly() const noexcept;

    /*!
     * \brief getDatabaseName
     * \return the name of the current database
     */
    [[nodiscard]] QString getDatabaseName() const noexcept;

    /*!
     * \brief Fetches values from the database.
     * \param what Values to fetch
     * \param from Table name / joints
     * \param where Condition
     * \return Values to be fetched as QStringList.
     */
    QStringList select(const QVector<QString>& what,
                       const QString& from,
                       const QVector<ColumnValueComparison>& where) const noexcept;

    /*!
     * \brief Inserts values into the database.
     * \param into Table name
     * \param values Values
     */
    void insert(const QString& into, const QVector<QString>& values) const noexcept;

    /*!
     * \brief Updates data in the database.
     * \param table Table name
     * \param set New value
     * \param where Condition
     */
    void update(const QString& table,
                const QVector<ColumnValueComparison>& set,
                const QVector<ColumnValueComparison>& where) const noexcept;

    /*!
     * \brief Removes data from the database.
     * \param from Table
     * \param where Condition
     */
    void remove(const QString& from, const QVector<ColumnValueComparison>& where) const noexcept;

    /*!
     * \brief Executes a raw SQL query. Not guarded against SQL injection !
     * \param query Raw request as text
     */
    void rawExecRequest(const QString& query) const noexcept;

    /*!
     * \brief Asynchronously inserts data into the database.
     * \param into Table name
     * \param values Values
     */
    void asyncInsert(const QString& into, const QVector<QString>& values) const noexcept;

    /*!
     * \brief Asynchronously updates data in the database.
     * \param table Table name, joints, etc...
     * \param set New values
     * \param where Condition
     */
    void asyncUpdate(const QString& table,
                     const QVector<ColumnValueComparison>& set,
                     const QVector<ColumnValueComparison>& where) const noexcept;

    /*!
     * \brief Asynchronously removes data from the database.
     * \param from Table name
     * \param where Condition
     */
    void asyncRemove(const QString& from, const QVector<ColumnValueComparison>& where) const noexcept;

    /*!
     * \brief Asynchronously executes a raw SQL query. Not guarded against SQL injection !
     * \param query Raw request as text
     */
    void asyncRawExecRequest(const QString& query) const noexcept;

  private:
    const DatabaseConnectionConfig conf ;
    DatabaseWorker syncWorker {"MainThread", conf};
    QueryThread asyncWorker {conf};
    bool readOnly {false};
    bool dbIsOpen {false};
};

#endif // QTASYNCDATABASE_H
