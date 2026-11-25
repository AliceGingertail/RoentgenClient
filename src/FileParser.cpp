#include "FileParser.h"

#include <itkRawImageIO.h>

#include <algorithm>
#include <stdexcept>

XrayImage FileParser::read(const std::filesystem::path& path)
{
    // Проверяем размер кадра по числу пикселей и размеру одного пикселя.
    constexpr auto pixelCount = XrayImage::width * XrayImage::height;
    constexpr auto expectedSize = pixelCount * sizeof(std::uint32_t);
    if (std::filesystem::file_size(path) != expectedSize) {
        throw std::runtime_error("Expected a 512 x 1024 RAW image (2097152 bytes)");
    }

    // Задаём ITK размеры кадра и порядок байтов, поскольку RAW без заголовка.
    auto imageIO = itk::RawImageIO<std::uint32_t, 2>::New();
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

std::vector<std::uint8_t> XrayImage::toGrayscale8() const
{
    std::vector<std::uint8_t> grayscale(intensities.size());
    const auto peak = std::ranges::max_element(intensities);
    // Для пустого или чёрного кадра масштабирование не требуется.
    if (peak == intensities.end() || *peak == 0) {
        return grayscale;
    }

    // Масштабируем от 0 до 255 с округлением и 64-битным умножением.
    const uint64_t maxIntensity = *peak;
    std::ranges::transform(intensities, grayscale.begin(), [maxIntensity](uint32_t value) {
        return static_cast<std::uint8_t>((value * uint64_t{255} + maxIntensity / 2) / maxIntensity);
    });
    return grayscale;
}
