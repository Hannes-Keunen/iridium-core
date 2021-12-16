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

    constexpr std::strong_ordering compare(const char* lhs, const char* rhs)
    {
        return to_ordering(std::strcmp(lhs, rhs));
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

        constexpr std::strong_ordering operator<=>(const StringView& rhs) const { return compare(data(), rhs.data()); }
        constexpr bool operator==(const StringView& rhs) const { return compare(data(), rhs.data()) == std::strong_ordering::equal; };
        constexpr bool operator!=(const StringView& rhs) const = default;
        constexpr bool operator<(const StringView& rhs) const = default;
        constexpr bool operator>(const StringView& rhs) const = default;
        constexpr bool operator<=(const StringView& rhs) const = default;
        constexpr bool operator>=(const StringView& rhs) const = default;

        constexpr auto find(char s, char* from) const -> const char*
        {
            for (auto c = begin(); c < end(); ++c)
            {
                if (*c == s)
                {
                    return c;
                }
            }
            return end();
        }

        constexpr auto substr(const char* from, const char* to) -> StringView
        {
            return StringView(from, to);
        }

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
