#pragma once

#include "iridium/core/string.hpp"

#include <optional>
#include <vector>

namespace ir
{
    template<typename T>
    auto read_file(const StringView& filename) -> std::optional<std::vector<T>>
    {
        FILE* file = fopen(filename.c_str(), "r");
        if (file == nullptr)
        {
            return std::nullopt;
        }

        fseek(file, 0, SEEK_END);
        size_t size = ftell(file);
        fseek(file, 0, SEEK_SET);

        size_t count = size / sizeof(T);
        std::vector<T> result(count);
        fread(result.data(), sizeof(T), count, file);
        fclose(file);
        return result;
    }

    auto read_file_as_string(const StringView& filename) -> std::optional<String>;
} // namespace ir
