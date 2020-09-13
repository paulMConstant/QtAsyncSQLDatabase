#ifndef COLUMNVALUECOMPARISON_H
#define COLUMNVALUECOMPARISON_H

#include <QString>
#include <QVector>
#include <QMetaType>
#include <QMap>

/*!
 * \brief The Operator enum
 * eq equal
 * ne not equal
 * lt less than
 * le less or equal
 * gt greater than
 * ge greater or equal
 */
enum class Operator {eq, ne, lt, le, gt, ge};

/*!
 * \brief Simple struct holding a column name, a value, and a comparison operator.
 * Can be used to express condition : Column <Operator> Value, e.g. "Name = Paul"
 */
struct ColumnValueComparison
{
    ColumnValueComparison(const QString& col,
                          const QString& value,
                          const Operator& cmp = Operator::eq) noexcept;

    QString col;
    QString value;
    Operator cmp;

    inline QString stringCmp() const noexcept {return cmpAsString[cmp];}

  private:
    static const QMap<Operator, QString> cmpAsString;
};

#endif  // COLUMNVALUECOMPARISON_H
