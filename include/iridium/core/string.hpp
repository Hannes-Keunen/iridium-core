#pragma once

#include <fmt/format.h>

#include <compare>
#include <cstring>
#include <string>
#include <string_view>

namespace ir
{

    constexpr std::strong_ordering to_ordering(int ordering)
    {
        if (ordering < 0)
            return std::strong_ordering::less;
        else if (ordering == 0)
            return std::strong_ordering::equal;
        else
            return std::strong_ordering::greater;
    }

    constexpr std::strong_ordering compare(const char* lbegin, const char* lend, const char* rbegin, const char* rend)
    {
        while (lbegin != lend && rbegin != rend)
        {
            if (*lbegin < *rbegin)
            {
                return std::strong_ordering::less;
            }
            else if (*lbegin > *rbegin)
            {
                return std::strong_ordering::greater;
            }
            else
            {
                ++lbegin;
                ++rbegin;
            }
        }

        if (lbegin == lend && rbegin == rend)
        {
            return std::strong_ordering::equal;
        }
        else if (lbegin == lend)
        {
            return std::strong_ordering::less;
        }
        else
        {
            return std::strong_ordering::greater;
        }
    }

    class StringView;

    class String
    {
    public:
        String(const char* str);
        String(const char* begin, const char* end);
        String(const char* str, std::size_t len);
        String(const String& other);
        String(String&& other);
        String& operator=(const String& other);
        String& operator=(String&& other);
        ~String();

        constexpr const char* begin() const { return _begin; }
        constexpr const char* end() const { return _end; }
        constexpr const char* data() const { return _begin; }
        constexpr auto size() const -> std::size_t { return _end - _begin; }
    private:
        void _init(const char* str, std::size_t len);

        char* _begin;
        char* _end;
    };

    class StringView
    {
    public:
        constexpr StringView(const char* begin, const char* end)
            : _begin(begin), _end(end) {}

        constexpr StringView(const char* str)
        {
            _begin = str;
            _end = _begin + std::strlen(str);
        };
    
        constexpr StringView(const String& str)
        {
            _begin = str.data();
            _end = _begin + str.size();
        }
    
        constexpr StringView(const std::string& str)
        {
            _begin = str.data();
            _end = _begin + str.size();
        }

        constexpr StringView(const std::string_view& str)
        {
            _begin = str.data();
            _end = _begin + str.size();
        }

        constexpr std::strong_ordering operator<=>(const StringView& rhs) const { return compare(begin(), end(), rhs.begin(), rhs.end()); }
        constexpr bool operator==(const StringView& rhs) const { return compare(begin(), end(), rhs.begin(), rhs.end()) == std::strong_ordering::equal; };
        constexpr bool operator!=(const StringView& rhs) const = default;
        constexpr bool operator<(const StringView& rhs) const = default;
        constexpr bool operator>(const StringView& rhs) const = default;
        constexpr bool operator<=(const StringView& rhs) const = default;
        constexpr bool operator>=(const StringView& rhs) const = default;

        constexpr char operator[](int i) const { return begin()[i]; }

        auto take() -> String
        {
            return String(begin(), end());
        }

        constexpr const char* data() const { return _begin; }
        constexpr const char* begin() const { return _begin; }
        constexpr const char* end() const { return _end; }
        constexpr auto size() const -> std::size_t { return _end - _begin; }
        constexpr const char* c_str() const { return _begin; }
    private:
        const char* _begin;
        const char* _end;
    };

    constexpr std::strong_ordering operator<=>(const String& lhs, const StringView& rhs) { return StringView(lhs) <=> rhs; };
    constexpr bool operator==(const String& lhs, const StringView& rhs) { return StringView(lhs) == rhs; };

    namespace str
    {
        template <typename S>
        constexpr auto find(const S& str, char s, const char* from) -> const char*
        {
            for (auto c = from; c < str.end(); ++c)
            {
                if (*c == s)
                {
                    return c;
                }
            }
            return str.end();
        }

        template <typename S>
        constexpr auto find(const S& str, char s) -> const char*
        {
            return find(str, s, str.begin());
        }

        constexpr auto sub(const char* from, const char* to) -> StringView
        {
            return StringView(from, to);
        }

    } // namespace str

} // namespace ir

template <>
struct fmt::formatter<ir::StringView> : fmt::formatter<std::string_view>
{
    template <typename FormatContext>
    constexpr auto format(const ir::StringView& str, FormatContext& ctx) -> decltype(ctx.out())
    {
        std::string_view s(str.begin(), str.size());
        return fmt::formatter<std::string_view>::format(s, ctx);
    }
};
