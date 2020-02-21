#ifndef _PARSER_HPP_
#define _PARSER_HPP_
#include "Value.hpp"
#include <cstring>
#include <fstream>
#include <sstream>
namespace ToyJson
{
class Parser
{
private:
    string buffer, thisline;
    size_t line, pos;

private:
    char next_token()
    {
        skip_ws();
        return buffer[pos];
    }
    void expect(const char *_expect)
    {
        skip_ws();
        string check_str = buffer.substr(pos, strlen(_expect));
        if (check_str != _expect)
            Error::expect_error(line, thisline.c_str(), check_str.c_str(), _expect);
        pos += strlen(_expect);
    }
    void skip_ws()
    {
        while (pos < buffer.size())
        {
            if (buffer[pos] == 0xA)
            {
                pos++, line++;

                size_t pos_n = buffer.find('\n', pos);
                if (pos_n == string::npos)
                    pos_n = buffer.size();
                thisline = buffer.substr(pos, pos_n - pos);
            }
            else if (buffer[pos] == 0x9 || buffer[pos] == 0xD || buffer[pos] == 0x20)
                pos++;
            else
                break;
        }
    }

private:
    Value get_string()
    {
        expect("\"");
        string val;
        val.reserve(1024);
        for (char c = buffer[pos]; c != '\"'; c = buffer[++pos])
        {
            val += c;
            if (c == '\\')
            {
                size_t pos_n = pos + 1;
                c = buffer[pos_n];
                if (c == '\"' || c == '\\' || c == '/' || c == 'b' || c == 'f' || c == 'n' || c == 'r' || c == 't')
                    val += c;
                else if (c == 'u')
                {
                    val += 'u';
                    for (size_t i = 0; i < 4; i++)
                    {
                        c = buffer[pos_n + i];
                        if (isdigit(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'))
                            val += c;
                        else
                            Error::slash_error(line, thisline.c_str(), buffer.substr(pos, pos_n - pos).c_str());
                    }
                    pos_n += 4;
                }
                else
                    Error::slash_error(line, thisline.c_str(), buffer.substr(pos, pos_n - pos).c_str());
                pos = pos_n;
            }
        }
        pos++;
        return Value(val);
    }
    Value get_number()
    {
        double _d_val;
        size_t pos_n, line_end = buffer.find('\n', pos);
        string tmp = buffer.substr(pos, line_end - pos);
        _d_val = std::stod(tmp, &pos_n);
        pos += pos_n;
        for (size_t i = 0; i < pos_n; i++)
        {
            if (tmp[i] == '.' || tmp[i] == 'e' || tmp[i] == 'E')
                return Value(_d_val);
        }
        return Value((long long)_d_val);
    }
    Value get_array()
    {
        Value ret;
        ret.make(_T_Array);
        size_t tmp = 0;
        expect("[");
        if (next_token() != ']')
            while (1)
            {
                Value _val = get_element();
                ret[tmp++] = _val;
                if (next_token() == ']')
                    break;
                expect(",");
            }
        expect("]");
        return std::move(ret);
    }
    Value get_object()
    {
        Value ret;
        ret.make(_T_Object);
        expect("{");
        if (next_token() != '}')
            while (1)
            {
                Value _key = get_string();
                expect(":");
                Value _val = get_element();
                ret[_key.string_value()] = _val;
                if (next_token() == '}')
                    break;
                expect(",");
            }
        expect("}");
        return std::move(ret);
    }
    Value get_element()
    {
        char nexttoken = next_token();
        switch (nexttoken)
        {
        case 't':
            expect("true");
            return Value(true);
        case 'f':
            expect("false");
            return Value(false);
        case 'n':
            expect("null");
            return Value();
        case '\"':
            return std::move(get_string());
        case '[':
            return std::move(get_array());
        case '{':
            return std::move(get_object());
        default:
            if (nexttoken == '-' || isdigit(nexttoken))
                return std::move(get_number());
        }
        Error::cannot_parse_error(line, thisline.c_str(), buffer.substr(pos, 3).c_str());
        skip_ws();
        return Value();
    }

public:
    Parser(const char *filedir) { open(filedir); }
    void open(const char *filedir)
    {
        std::ifstream ifs;
        ifs.open(filedir);
        if (ifs.good())
        {
            std::stringstream ss;
            ss << ifs.rdbuf();
            buffer = ss.str();
            line = 1, pos = 0;
            thisline = buffer.substr(pos, buffer.find('\n', pos));
        }
        else
            Error::io_error(filedir);
    }
    Value parse() { return std::move(get_element()); }
};
}; // namespace ToyJson

#endif