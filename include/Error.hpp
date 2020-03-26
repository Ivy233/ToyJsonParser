#ifndef _ERROR_HPP_
#define _ERROR_HPP_
#include "json_t.hpp"
#include <cassert>
#include <iostream>
namespace ToyJson
{
namespace Error
{
// Value
void type_error(json_t _type, json_t _type1, json_t _type2)
{
    printf("type error: expect %s or %s rather than %s\n", _M_type[_type1], _M_type[_type2], _M_type[_type]);
    assert(0);
}
void type_error(json_t _type, json_t _type1)
{
    printf("type error: expect %s rather than %s\n", _M_type[_type1], _M_type[_type]);
    assert(0);
}
void cannot_find_error(const char *_key)
{
    printf("cannot find: %s in Object\n", _key);
    assert(0);
}
void border_error(size_t _pos, size_t _siz)
{
    printf("border error: need index %lld, but size %lld\n", _pos, _siz);
    assert(0);
}
// file
void io_error(const char *_filedir)
{
    printf("io_error: file %s cannot open\n", _filedir);
    assert(0);
}
// Parser
void expect_error(size_t _line, const char *_meet, const char *_expect)
{
    printf("line %lld: expect \"%s\", but meet \"%s\"\n", _line, _expect, _meet);
    assert(0);
}
void slash_error(size_t _line, const char *_meet)
{
    printf("line %lld: cannot parse \"%s\" while parsing a string\n", _line, _meet);
    assert(0);
}
void cannot_parse_error(size_t _line, const char *_meet)
{
    printf("line %lld: encounted chars \"%s\" are not the beginning of a json structure\n", _line, _meet);
    assert(0);
}
} // namespace Error
}; // namespace ToyJson

#endif