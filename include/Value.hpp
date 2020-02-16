#ifndef _VALUE_HPP_
#define _VALUE_HPP_
#include "Error.hpp"
#include <map>
#include <string>
#include <vector>
namespace ToyJson
{
class Value
{
private:
    using std::map;
    using std::string;
    using std::vector;
    union {
        map<string, Value> *_M_obj;
        vector<Value> *_M_vec;
        string *_M_str;
        double _M_num;
        unsigned long long _M_int;
        bool _M_bool;
    } _M_val;
    json_t _M_type;

private:
    void clear()
    {
        switch (_M_type)
        {
        case _T_Object:
            delete _M_val._M_obj;
            break;
        case _T_Array:
            delete _M_val._M_vec;
            break;
        case _T_String:
            delete _M_val._M_str;
            break;
        default:;
        }
        _M_type = _T_Null;
    }

public:
    Value() { _M_val._M_obj = nullptr; }
    Value(const Value &other)
    {
        switch (other._M_type)
        {
        case _T_Object:
            _M_val._M_obj =
                new map<string, Value>(other._M_val._M_obj->begin(),
                                       other._M_val._M_obj->end());
            break;
        case _T_Array:
            _M_val._M_vec =
                new vector<Value>(other._M_val._M_vec->begin(),
                                  other._M_val._M_vec->end());
            break;
        case _T_String:
            _M_val._M_str =
                new string(*other._M_val._M_str);
        default:
            _M_val = other._M_val;
        }
        _M_type = other._M_type;
    }
    Value(Value &&other) : _M_val(other._M_val), _M_type(other._M_type)
    {
        other._M_type = _T_Null;
        other._M_val._M_obj = nullptr;
    }

    Value &operator=(const Value &rhs)
    {
        clear();
        switch (other._M_type)
        {
        case _T_Object:
            _M_val._M_obj =
                new map<string, Value>(other._M_val._M_obj->begin(),
                                       other._M_val._M_obj->end());
            break;
        case _T_Array:
            _M_val._M_vec =
                new vector<Value>(other._M_val._M_vec->begin(),
                                  other._M_val._M_vec->end());
            break;
        case _T_String:
            _M_val._M_str =
                new string(*rhs._M_val._M_str);
        default:
            _M_val = other._M_val;
        }
        _M_type = other._M_type;
        return *this;
    }
    Value &operator=(Value &&rhs)
    {
        clear();
        _M_val = rhs._M_val;
        _M_type = rhs._M_type;
        rhs._M_val._M_obj = nullptr;
        rhs._M_type = _T_Null;
    }
    ~Value()
    {
        switch (_M_type)
        {
        case _T_Object:
            delete _M_val._M_obj;
            break;
        case _T_Array:
            delete _M_val._M_vec;
            break;
        case _T_String:
            delete _M_val._M_str;
            break;
        default:;
        }
    }

public:
    Value &operator[](const string &_key)
    {
        if (_M_type != _T_Object)
            Error::check_type(_M_type, _T_Object);
        return _M_val._M_obj->operator[](_key);
    }
    Value &operator[](size_t _pos)
    {
        if (_pos >= _M_val._M_vec->size())
            Error::border_error(_pos, _M_val._M_vec->size());
        return _M_val._M_vec->operator[](_pos);
    }
    size_t size() const
    {
        switch (_M_type)
        {
        case _T_Object:
            return _M_val._M_obj->size();
        case _T_Array:
            return _M_val._M_vec->size();
        case _T_String:
            return _M_val._M_str->size();
        default:
            Error::type_error(_M_type, _T_Object);
        }
    }
    json_t type() const { return _M_type; }
    double double_value() const
    {
        if (_M_type != _T_Number)
            Error::type_error(_M_type, _T_Number);
        return _M_val._M_num;
    }
    bool bool_value() const
    {
        if (_M_type != _T_Boolean)
            Error::type_error(_M_type, _T_Boolean);
        return _M_val._M_bool;
    }
    unsigned long long integer_value() const
    {
        if (_M_type != _T_Integer)
            Error::type_error(_M_type, _T_Integer);
        return _M_val._M_int;
    }
    const string &string_value() const
    {
        if (_M_type != _T_String)
            Error::type_error(_M_type, _T_String);
        return _M_val._M_str;
    }
    bool is_null() const { return _M_type == _T_Null; }

public:
    void set(const vector<Value> &_other)
    {
        clear();
        _M_type = _T_Array;
        _M_val._M_vec = new vector<Value>(_other.begin(), _other.end());
    }
};
}; // namespace ToyJson
#endif