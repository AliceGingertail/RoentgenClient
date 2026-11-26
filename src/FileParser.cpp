#include "FileParser.h"

#include <itkRawImageIO.h>

#include <algorithm>
#include <cmath>
#include <stdexcept>

XrayImage FileParser::read(const std::filesystem::path& path)
{
    // Проверяем размер кадра по числу пикселей и размеру одного пикселя.
    constexpr auto pixelCount = XrayImage::width * XrayImage::height;
    constexpr auto expectedSize = pixelCount * sizeof(uint32_t);
    if (std::filesystem::file_size(path) != expectedSize) {
        throw std::runtime_error("Expected a 512 x 1024 RAW image (2097152 bytes)");
    }

    // Задаём ITK размеры кадра и порядок байтов, поскольку RAW без заголовка.
    auto imageIO = itk::RawImageIO<uint32_t, 2>::New();
    imageIO->SetFileName(path.string());
    imageIO->SetDimensions(0, XrayImage::width);
    imageIO->SetDimensions(1, XrayImage::height);
    imageIO->SetHeaderSize(0);
    imageIO->SetFileTypeToBinary();
    imageIO->SetByteOrderToLittleEndian();

    // Читаем исходные 32-битные интенсивности в буфер.
    XrayImage image{std::vector<std::uint32_t>(pixelCount)};
    imageIO->Read(image.intensities.data());
    return image;
}

std::vector<uint8_t> XrayImage::toGrayscale8(int brightness, int contrast) const
{
    std::vector<uint8_t> grayscale(intensities.size());
    const auto peak = std::ranges::max_element(intensities);
    if (peak == intensities.end())
        return grayscale;

    const auto maxIntensity = std::max(*peak, uint32_t{1});
    const auto offset = std::clamp(brightness, -100, 100) * 255.0 / 100.0;
    const auto gain = std::exp2(std::clamp(contrast, -100, 100) / 50.0);

    // Меняем контраст вокруг середины шкалы и округляем только на выходе
    std::ranges::transform(intensities, grayscale.begin(), [maxIntensity, offset, gain](uint32_t value) {
        const auto normalized = value * 255.0 / maxIntensity;
        const auto adjusted = (normalized - 127.5) * gain + 127.5 + offset;
        return static_cast<uint8_t>(std::lround(std::clamp(adjusted, 0.0, 255.0)));
    });
    return grayscale;
}
