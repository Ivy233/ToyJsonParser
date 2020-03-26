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
    string buffer;
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
            Error::expect_error(line, escape(buffer.substr(pos, 8)).c_str(), _expect);
        pos += strlen(_expect);
    }
    void skip_ws()
    {
        while (pos < buffer.size())
            if (buffer[pos] == 0xA)
                pos++, line++;
            else if (buffer[pos] == 0x9 || buffer[pos] == 0xD || buffer[pos] == 0x20)
                pos++;
            else
                break;
    }

private:
    string get_string()
    {
        expect("\"");
        string ret;
        ret.reserve(128);
        while (pos < buffer.size() && buffer[pos] != '\"')
        {
            if (buffer[pos] != '\\')
                ret += buffer[pos];
            else
                switch (buffer[++pos])
                {
                case '\\':
                    ret += '\\';
                    break;
                case '\"':
                    ret += '\"';
                    break;
                case '\'':
                    ret += '\'';
                    break;
                case 'n':
                    ret += '\n';
                    break;
                case 'b':
                    ret += '\b';
                    break;
                case 'f':
                    ret += '\f';
                    break;
                case 'r':
                    ret += '\r';
                    break;
                case 't':
                    ret += '\t';
                    break;
                default:
                    ret += '\\';
                }
            pos++;
        }
        expect("\"");
        return ret;
    }
    Value get_number()
    {
        double _d_val;
        size_t pos_n;
        string tmp = buffer.substr(pos, buffer.find('\n', pos) - pos);
        _d_val = std::stod(tmp, &pos_n);
        pos += pos_n;
        for (size_t i = 0; i < pos_n; i++)
            if (tmp[i] == '.' || tmp[i] == 'e' || tmp[i] == 'E')
                return Value(_d_val);
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
        return move(ret);
    }
    Value get_object()
    {
        Value ret;
        ret.make(_T_Object);
        expect("{");
        if (next_token() != '}')
            while (1)
            {
                string _key = get_string();
                expect(":");
                Value _val = get_element();
                ret[_key] = _val;
                if (next_token() == '}')
                    break;
                expect(",");
            }
        expect("}");
        return move(ret);
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
            return Value(get_string());
        case '[':
            return move(get_array());
        case '{':
            return move(get_object());
        default:
            if (nexttoken == '-' || isdigit(nexttoken))
                return std::move(get_number());
        }
        Error::cannot_parse_error(line, escape(buffer.substr(pos, 8)).c_str());
        skip_ws();
        return Value();
    }

public:
    Value parse_string(const char *_string)
    {
        buffer = _string;
        line = 1, pos = 0;
        return move(get_element());
    }
    Value parse_string(const string &_string)
    {
        buffer = _string;
        line = 1, pos = 0;
        return move(get_element());
    }
    Value parse_file(const char *_filedir)
    {
        std::ifstream ifs;
        ifs.open(_filedir);
        if (ifs.good())
        {
            std::stringstream ss;
            ss << ifs.rdbuf();
            buffer = ss.str();
            line = 1, pos = 0;
        }
        else
            Error::io_error(_filedir);
        return move(get_element());
    }
};
}; // namespace ToyJson

#endif