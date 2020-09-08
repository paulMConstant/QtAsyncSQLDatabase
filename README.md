# QtAsyncSQLDatabase
Asynchronous SQL database &amp; SQL builder with Qt/C++.

Configure it with the DatabaseConnectionConfig struct, which is passed to the underlying QSqlDatabase.

Can execute raw queries or securely build select/insert/update/remove queries (protected against SQL injection).
Everything but the SQL-builder select can be run asynchronously.

The asynchronous database is implemented using a QThread running as long as the database is open.
