#pragma once

#include <cstdint>
#include <filesystem>
#include <vector>

struct XrayImage
{
    static constexpr uint16_t width = 512;
    static constexpr uint16_t height = 1024;

    std::vector<uint32_t> intensities;

    [[nodiscard]] std::vector<std::uint8_t> toGrayscale8(int brightness = 0, int contrast = 0) const;
};

class FileParser
{
public:
    [[nodiscard]] static XrayImage read(const std::filesystem::path& path);
};
