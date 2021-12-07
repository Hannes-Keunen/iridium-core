#pragma once

#include <fmt/core.h>
#include <fmt/chrono.h>

#include <chrono>

namespace ir::log
{
    namespace
    {
        void print(char level, const char* tag, const std::string_view& msg)
        {
            auto now = std::chrono::system_clock::now();
            fmt::print("[{:%H:%M:%S}][{:c}|{:s}]: {:s}\n", now, level, tag, msg);
        }
    }

    inline void v(const char* tag, const std::string_view& msg)
    {
        print('V', tag, msg);
    }

    inline void d(const char* tag, const std::string_view& msg)
    {
        print('D', tag, msg);
    }

    inline void i(const char* tag, const std::string_view& msg)
    {
        print('I', tag, msg);
    }

    inline void w(const char* tag, const std::string_view& msg)
    {
        print('W', tag, msg);
    }

    inline void e(const char* tag, const std::string_view& msg)
    {
        print('E', tag, msg);
    }

    inline void f(const char* tag, const std::string_view& msg)
    {
        print('F', tag, msg);
    }

} // namespace log
