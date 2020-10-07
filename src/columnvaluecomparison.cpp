#include "columnvaluecomparison.h"

const QMap<Operator, QString> ColumnValueComparison::cmpAsString
{
    QMap<Operator, QString>(
    {
        {Operator::eq, "="}, {Operator::ne, "!="},
        {Operator::lt, "<"}, {Operator::le, "<="},
        {Operator::gt, ">"}, {Operator::ge, ">="}
    })
};


ColumnValueComparison::ColumnValueComparison(const QString& col,
        const QString& value,
        const Operator& cmp) noexcept :
    col(col), value(value), cmp(cmp)
{
}
