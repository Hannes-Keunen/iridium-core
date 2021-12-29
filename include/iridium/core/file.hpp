#pragma once

#include "iridium/core/string.hpp"

#include <optional>

namespace ir
{
    auto read_file_as_string(const StringView& filename) -> std::optional<String>;
} // namespace ir
