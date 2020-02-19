#ifndef _VALUE_HPP_
#define _VALUE_HPP_
#include "Error.hpp"
#include <string>
namespace ToyJson
{
using std::enable_if;
using std::is_floating_point;
using std::is_integral;
using std::is_same;
class Value
{
private:
    json_t _M_type;
    union backup_value {
        backup_value(double _decimal) : _M_decimal(_decimal) {}
        backup_value(long long _integer) : _M_integer(_integer) {}
        backup_value(bool _bool) : _M_bool(_bool) {}
        backup_value() : _M_integer(0) {}
        bool _M_bool;
        long long _M_integer;
        double _M_decimal;
    } _M_val;

public:
    Value() : _M_type(_T_Null) {}
    Value(const Value &rhs)
    {
        _M_type = rhs._M_type;
        _M_val = rhs._M_val;
    }
    Value &operator=(const Value &rhs)
    {
        clear();
        _M_type = rhs._M_type;
        _M_val = rhs._M_val;
        return *this;
    }
    Value(Value &&rhs) : _M_type(rhs._M_type), _M_val(rhs._M_val)
    {
        rhs._M_type = _T_Null;
        rhs._M_val._M_integer = 0;
    }
    Value &operator=(Value &&rhs)
    {
        clear();
        _M_type = rhs._M_type;
        _M_val = rhs._M_val;
        rhs._M_type = _T_Null;
        rhs._M_val._M_integer = 0;
        return *this;
    }
    template <typename T>
    explicit Value(T _bool, typename enable_if<is_same<T, bool>::value>::type * = 0)
        : _M_type(_T_Boolean), _M_val((bool)_bool) {}
    template <typename T>
    explicit Value(T _integer, typename enable_if<is_integral<T>::value && !is_same<T, bool>::value>::type * = 0)
        : _M_type(_T_Integer), _M_val((long long)_integer) {}
    template <typename T>
    explicit Value(T _decimal, typename enable_if<is_floating_point<T>::value>::type * = 0)
        : _M_type(_T_Decimal), _M_val((double)_decimal) {}

public:
    json_t type() const { return _M_type; }
    bool bool_value() const { return _M_val._M_bool; }
    long long integer_value() const { return _M_val._M_integer; }
    double decimal_value() const { return _M_val._M_decimal; }

public:
    void clear()
    {
        _M_type = _T_Null;
        _M_val._M_integer = 0;
    }
};
std::ostream &operator<<(std::ostream &os, const Value &v)
{
    switch (v.type())
    {
    case _T_Null:
        os << "null";
        break;
    case _T_Boolean:
        os << (v.bool_value() ? "true" : "false");
        break;
    case _T_Integer:
        os << v.integer_value();
    case _T_Decimal:
        os << v.decimal_value();
    default:
        break;
    }
    return os;
}
}; // namespace ToyJson
#endif