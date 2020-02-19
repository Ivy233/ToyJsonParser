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
using std::string;
class Value
{
private:
    json_t _M_type;
    union backup_value {
        backup_value() : _M_integer(0) {}
        backup_value(double _decimal) : _M_decimal(_decimal) {}
        backup_value(long long _integer) : _M_integer(_integer) {}
        backup_value(bool _bool) : _M_bool(_bool) {}
        backup_value(string _string) : _M_string(new string(_string)) {}
        bool _M_bool;
        long long _M_integer;
        double _M_decimal;
        string *_M_string;
    } _M_val;

private:
    string escape(const string &str)
    {
        string ret;
        ret.reserve(str.size());
        for (size_t i = 0; i < str.size(); i++)
            switch (str[i])
            {
            case '\"':
                ret.append("\\\"");
                break;
            case '\\':
                ret.append("\\\\");
                break;
            case '\b':
                ret.append("\\b");
                break;
            case '\f':
                ret.append("\\f");
                break;
            case '\n':
                ret.append("\\n");
                break;
            case '\r':
                ret.append("\\r");
                break;
            case '\t':
                ret.append("\\t");
                break;
            case '/':
                ret.append("\\/");
                break;
            default:
                ret += str[i];
            }
        return std::move(ret);
    }

public:
    Value() : _M_type(_T_Null) {}
    Value(const Value &rhs) : _M_type(rhs._M_type)
    {
        switch (rhs._M_type)
        {
        case _T_String:
            _M_val._M_string = new string(*rhs._M_val._M_string);
            break;
        default:
            _M_val = rhs._M_val;
        }
    }
    Value &operator=(const Value &rhs)
    {
        clear();
        _M_type = rhs._M_type;
        switch (rhs._M_type)
        {
        case _T_String:
            _M_val._M_string = new string(*rhs._M_val._M_string);
            break;
        default:
            _M_val = rhs._M_val;
        }
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
    explicit Value(string _string) : _M_type(_T_String), _M_val(escape(_string)) {}

public:
    json_t type() const { return _M_type; }
    bool bool_value() const
    {
        if (_M_type != _T_Boolean)
            Error::type_error(_M_type, _T_Boolean);
        return _M_val._M_bool;
    }
    long long integer_value() const
    {
        if (_M_type != _T_Integer)
            Error::type_error(_M_type, _T_Integer);
        return _M_val._M_integer;
    }
    double decimal_value() const
    {
        if (_M_type != _T_Decimal)
            Error::type_error(_M_type, _T_Decimal);
        return _M_val._M_decimal;
    }
    string string_value() const
    {
        if (_M_type != _T_String)
            Error::type_error(_M_type, _T_String);
        return std::move(*_M_val._M_string);
    }

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
        break;
    case _T_Decimal:
        os << v.decimal_value();
        break;
    case _T_String:
        os << v.string_value();
        break;
    case _T_Object:
        break;
    case _T_Array:
        break;
    }
    return os;
}
}; // namespace ToyJson
#endif