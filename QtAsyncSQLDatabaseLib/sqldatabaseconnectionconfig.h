#ifndef SQLDATABASECONNECTIONCONFIG_H
#define SQLDATABASECONNECTIONCONFIG_H

#include <QString>

/*!
 * \brief Holds the configuration data to initialize a QSqlDatabase.
 * See QSqlDatabase documentation.
 */
struct SqlDatabaseConnectionConfig
{
    SqlDatabaseConnectionConfig() noexcept = default;
    SqlDatabaseConnectionConfig(const QString& databaseName,
                             const QString& driver = "QSQLITE",
                             const QString& userName = "",
                             const QString& password = "",
                             const QString& hostName = "",
                             const QString& connectOptions = "",
                             int port = 3000) noexcept:
        databaseName(databaseName),
        driver(driver),
        userName(userName),
        password(password),
        hostName(hostName),
        connectOptions(connectOptions),
        port(port)
    {
    }

    QString databaseName;
    QString driver;
    QString userName;
    QString password;
    QString hostName;
    QString connectOptions;
    int port;
};

#endif // SQLDATABASECONNECTIONCONFIG_H
