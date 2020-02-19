#include "include/Parser.hpp"
#include <bits/stdc++.h>
using namespace ToyJson;
using namespace std;

int main()
{
    // Usage:
    Parser a("ToyJson_parser/test/1-null.json");
    Value tmp;
    cout << _M_type[tmp.type()] << endl;
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
        tmp = Value((unsigned)12345);
        cout << tmp << endl;
    }
    a.open("ToyJson_parser/test/1-integer.json");
    {
        tmp = a.parse();
        cout << _M_type[tmp.type()] << ": " << tmp.decimal_value() << endl;
        tmp = Value(1.45e7);
        cout << tmp << endl;
    }
    return 0;
}