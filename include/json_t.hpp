#ifndef _JSON_T_HPP_
#define _JSON_T_HPP_
namespace ToyJson
{
enum json_t
{
    _T_Null,
    _T_Object,
    _T_Array,
    _T_String,
    _T_Decimal,
    _T_Integer,
    _T_Boolean
};
const char *_M_type[] = {"Null", "Object", "Array", "String", "Decimal", "Integer", "Boolean"};
}; // namespace ToyJson

#endif