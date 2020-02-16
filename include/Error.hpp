#ifndef _ERROR_HPP_
#define _ERROR_HPP_
#include "json_t.hpp"
#include <cassert>
#include <iostream>
namespace ToyJson
{
namespace Error
{
void type_error(json_t _type1, json_t _type2)
{
    printf("type error: %s != %s\n", _M_type[_type1], _M_type[_type2]);
    // printf("type error: %d != %d\n", _type1, _type2);
    assert(0);
}

void cannot_find_error(const char *_key)
{
    printf("cannot find: %s in Object\n", _key);
    assert(0);
}
void border_error(const size_t &_pos, const size_t &_siz)
{
    printf("border error: pos %lld >= size %lld\n", _pos, _siz);
    assert(0);
}
void io_error(const char *_filedir)
{
    printf("io_error: file %s cannot open\n", _filedir);
    assert(0);
}
void expect_error(const char *_thisline, const size_t &_line, const size_t &_pos, const char *_expect)
{
    printf("line %lld: expect \"%s\" at pos %lld, but meet \"%s\"\n", _line, _expect, _pos, _thisline);
    assert(0);
}
void string_error(const char *_thisline, const size_t &_line, const size_t &_pos)
{
    printf("line %lld: no \" to enclosure a string from pos %lld in \"%s\"\n", _line, _pos, _thisline);
    assert(0);
}
void cannot_parse_error(const char *_thisline, const size_t &_line, const size_t &_pos)
{
    printf("line %lld: meet something at pos %lld in \"%s\"\n", _line, _pos, _thisline);
    assert(0);
}
} // namespace Error
}; // namespace ToyJson

#endif