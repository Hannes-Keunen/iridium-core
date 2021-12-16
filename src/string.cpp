#include "iridium/core/string.hpp"

namespace ir
{
    String::String(const char* str)
    {
        _init(str, std::strlen(str));
    }

    String::String(const char* begin, const char* end)
    {
        _init(begin, end - begin);
    }

    String::String(const char* str, std::size_t len)
    {
        _init(str, len);
    }

    String::String(const String& other)
    {
        _init(other._begin, other._end - other._begin);
    }

    String::String(String&& other)
    {
        _begin = other._begin;
        _end = other._end;
        other._begin = nullptr;
        other._end = nullptr;
    }

    String& String::operator=(const String& other)
    {
        if (_begin != nullptr)
        {
            delete[] _begin;
        }

        _init(other._begin, other._end - other._begin);
        return *this;
    }

    String& String::operator=(String&& other)
    {
        if (_begin != nullptr)
        {
            delete[] _begin;
        }

        _begin = other._begin;
        _end = other._end;
        other._begin = nullptr;
        other._end = nullptr;

        return *this;
    }

    String::~String()
    {
        delete[] _begin;
    }

    void String::_init(const char* str, std::size_t len)
    {
        _begin = new char[len+1];
        std::strncpy(_begin, str, len);
        _begin[len] = '\0';
        _end = _begin + len;
    }

} // namespace ir
