#pragma once

#include <cstdint>
#include <filesystem>
#include <vector>

struct XrayImage
{
    static constexpr int width = 512;
    static constexpr int height = 1024;

    std::vector<std::uint32_t> intensities;

    [[nodiscard]] std::vector<std::uint8_t> toGrayscale8() const;
};

class FileParser
{
public:
    [[nodiscard]] static XrayImage read(const std::filesystem::path& path);
};
