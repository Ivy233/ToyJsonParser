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
    std::string buffer;
    size_t line, pos, last_line;

private:
    char next_token()
    {
        skip_ws();
        return buffer[pos];
    }
    void expect(const char *_expect)
    {
        skip_ws();
        if (buffer.substr(pos, strlen(_expect)) != _expect)
            Error::expect_error(buffer.substr(pos, buffer.find('\n', pos) - pos).c_str(),
                                line, pos - last_line, _expect);
        pos += strlen(_expect);
    }
    void skip_ws()
    {
        while (pos < buffer.size())
        {
            if (buffer[pos] == '\n')
                last_line = pos + 1, line++;
            if (buffer[pos] == 0x9 || buffer[pos] == 0xA || buffer[pos] == 0xD || buffer[pos] == 0x20)
                pos++;
            else
                break;
        }
    }

private:
    Value get_string()
    {
        expect("\"");
        std::string val;
        val.reserve(1024);
        for (char c = buffer[pos]; c != '\"'; c = buffer[++pos])
        {
            val += c;
            if (c == '\\')
            {
                c = buffer[++pos];
                if (c == '\"' || c == '\\' || c == '/' || c == 'b' || c == 'f' || c == 'n' || c == 'r' || c == 't')
                    val += c;
                else if (c == 'u')
                {
                    val += 'u';
                    for (size_t i = 0; i < 4; i++)
                    {
                        c = buffer[pos + i];
                        if (isdigit(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'))
                            val += c;
                        else
                            Error::string_error(buffer.substr(pos, buffer.find('\n', pos) - pos).c_str(),
                                                line, pos);
                        pos += 4;
                    }
                }
                else
                    Error::string_error(buffer.substr(pos, buffer.find('\n', pos) - pos).c_str(),
                                        line, pos);
            }
        }
        pos++;
        return Value(val);
    }
    Value get_number()
    {
        double _d_val;
        size_t pos_n, line_end = buffer.find('\n', pos);
        std::string tmp = buffer.substr(pos, line_end - pos);
        _d_val = std::stod(tmp, &pos_n);
        for (size_t i = 0; i < pos_n; i++)
        {
            if (tmp[i] == '.' || tmp[i] == 'e' || tmp[i] == 'E')
                return Value(_d_val);
        }
        return Value((long long)_d_val);
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
        default:
            if (nexttoken == '-' || isdigit(nexttoken))
                return std::move(get_number());
        }
        Error::cannot_parse_error(buffer.substr(pos, buffer.find('\n', pos) - pos).c_str(),
                                  line, pos - last_line);
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
            line = 1, pos = 0, last_line = 0;
        }
        else
            Error::io_error(filedir);
    }
    Value parse() { return std::move(get_element()); }
};
}; // namespace ToyJson

#endif