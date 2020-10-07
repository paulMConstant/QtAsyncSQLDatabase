#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QVector>
#include <QString>
#include <QSqlDatabase>

#include "QtAsyncSqlDatabase_global.h"
#include "columnvaluecomparison.h"
#include "sqldatabaseconnectionconfig.h"

/*!
 * \brief Builds and executes SQL queries.
 * Owns a database connection.
 */
class QTASYNCSQLDATABASE_EXPORT SqlDatabase : public QObject
{
    Q_OBJECT
  public:
    SqlDatabase() = delete;
    explicit SqlDatabase(const QString& connectionName,
                            const SqlDatabaseConnectionConfig& conf,
                            QObject* parent = nullptr) noexcept;

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

    void answerACK() const noexcept;

  signals:
    void ACK() const;

  private:
    const QString connectionName;
    SqlDatabaseConnectionConfig conf;

    [[nodiscard]] inline QSqlDatabase db() const noexcept
    {return QSqlDatabase::database(connectionName, true);}

    static QString addWhere(QString query, const QVector<ColumnValueComparison>& where) noexcept;
    static QStringList execRequest(QSqlQuery query) noexcept;

};

#endif  // DATABASE_H
