#include "iridium/core/file.hpp"

#include "iridium/core/log.hpp"

#include <fstream>
#include <vector>

constexpr const char* TAG = __FILE__;

namespace ir
{
    auto read_file_as_string(const StringView& filename) -> std::optional<String>
    {
        std::ifstream file(filename.c_str(), std::ios::ate);
        if (!file.is_open())
        {
            log::e(TAG, fmt::format("Failed to open file {:s}", filename));
            return std::nullopt;
        }

        auto size = file.tellg();
        std::vector<char> buffer(size);
        file.seekg(0);
        file.read(buffer.data(), size);
        return String(buffer.data(), buffer.size());
    }

} // namespace ir
