#include "include/Parser.hpp"
#include <bits/stdc++.h>
using namespace ToyJson;
using namespace std;

void test_1()
{
    // 准确率测试
    // string->Value->cout
    string tmp = "null";
    Parser test_parser;

    Value test_val = test_parser.parse_string(tmp);
    assert(test_val.type() == _T_Null);
    cout << "try to parse \"null\": " << test_val << endl;

    test_val = test_parser.parse_string("true");
    assert(test_val.type() == _T_Boolean && test_val.bool_value() == true);
    cout << "try to parse \"true\": " << test_val << endl;

    test_val = test_parser.parse_string("false");
    assert(test_val.type() == _T_Boolean && test_val.bool_value() == false);
    cout << "try to parse \"false\": " << test_val << endl;
    cout << endl;

    test_val = test_parser.parse_string("1234");
    assert(test_val.type() == _T_Integer && test_val.integer_value() == 1234);
    cout << "try to parse integer: " << test_val << endl;

    test_val = test_parser.parse_string("1.23e45");
    assert(test_val.type() == _T_Decimal && test_val.decimal_value() == 1.23e45);
    cout << "try to parse decimal: " << test_val << endl;

    test_val = test_parser.parse_string("\"123\t\n\b\\\\\"");
    assert(test_val.type() == _T_String && test_val.string_value() == "123\\t\\n\\b\\\\");
    cout << "try to parse string: " << test_val << endl
         << endl;

    test_val = test_parser.parse_string("[1,true,3,4.56,\"string_value\"]");
    assert(test_val.type() == _T_Array);
    cout << "try to parse an array: " << test_val << endl;

    test_val = test_parser.parse_string(R"({
        "k1" : "v1",
        "k2" : 42,
        "k3" : [
            "test_val",
            123,
            true,
            false,
            null,
            []
        ]
    })");
    assert(test_val.type() == _T_Object);
    cout << "try to parse an object: " << test_val << endl
         << test_val["k3"] << endl
         << test_parser.parse_string("[1,2,3,\"huajihuajihuaji\",true]") << endl
         << endl;
}
void test_2()
{
    // 文件读写测试
    // ifstream->Value->ofstream
    Parser test_parser;
    ofstream fileout;
    fileout.open("test.json");

    Value test_val = test_parser.parse_file("test/1-object.json");
    fileout << test_val << endl;
}
void test_3()
{
    // 其他初始化测试
    // 主要是initializer_list的嵌套
    // 这一块接口还需要花费时间思考以符合习惯，但是大体都是这类
    // 如果这样修改的话，可能会修改目前对于数组、obj的处理，即先make然后按次序添加
    list<int> l1{1, 2, 3};
    vector<int> l2{1, 2, 3};
    set<int> l3{1, 2, 3};
    assert(Value(l1) == Value(l2));
    assert(Value(l2) == Value(l3));

    map<string, string> m1{{"k1", "v1"}, {"k2", "v2"}};
    unordered_map<string, string> m2{{"k1", "v1"}, {"k2", "v2"}};
    assert(Json(m1) == Json(m2));
}
int main()
{
    // Usage:
    test_1();
    test_2();
    test_3();
    return 0;
}