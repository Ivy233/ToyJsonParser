#include "include/Parser.hpp"
#include <bits/stdc++.h>
using namespace ToyJson;
using namespace std;

int main()
{
    // Usage:
    Parser a("ToyJson_parser/test/1-null.json");
    Value tmp;
    {
        tmp = a.parse();
        cout << _M_type[tmp.type()] << endl;
        tmp = Value(true);
        cout << tmp << endl;
    }
    a.open("ToyJson_parser/test/1-true.json");
    {
        tmp = a.parse();
        cout << _M_type[tmp.type()] << ": " << tmp.bool_value() << endl;
        tmp = Value(false);
        cout << tmp << endl;
    }
    a.open("ToyJson_parser/test/1-false.json");
    {
        tmp = a.parse();
        cout << _M_type[tmp.type()] << ": " << tmp.bool_value() << endl;
        tmp = Value();
        cout << tmp << endl;
    }
    a.open("ToyJson_parser/test/1-decimal.json");
    {
        tmp = a.parse();
        cout << _M_type[tmp.type()] << ": " << tmp.decimal_value() << endl;
        tmp = Value("\"huaji\nhuaji\nhuaji\"");
        cout << tmp << endl;
    }
    a.open("ToyJson_parser/test/1-integer.json");
    {
        tmp = a.parse();
        cout << _M_type[tmp.type()] << ": " << tmp.integer_value() << endl;
        tmp = Value(1.23e45);
        cout << tmp << endl;
    }
    a.open("ToyJson_parser/test/1-string.json");
    {
        tmp = a.parse();
        cout << _M_type[tmp.type()] << ": " << tmp.string_value() << endl;
        tmp = Value(12345);
        // cout << tmp.size() << endl;
        cout << tmp << endl;
    }
    a.open("ToyJson_parser/test/2-array.json");
    {
        tmp = a.parse();
        // cout << tmp << endl;
        cout << tmp << endl;
        tmp[2] = Value("\r\n\b\t\f\\");
        cout << tmp << endl;
        // cout << tmp["123"] << endl;
    }
    a.open("ToyJson_parser/test/2-object.json");
    {
        tmp = a.parse();
        cout << tmp << endl;
    }
    Value b = Value("1\t2345");
    cout << b.string_value() << endl;
    return 0;
}