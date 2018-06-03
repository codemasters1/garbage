//
// Created by Host on 08.08.2016.
//

#include "Buffer.hpp"

using console::Buffer;

Buffer::Buffer(std::size_t width, std::size_t height) :
    width_{width},
    height_{height},
    chars_{width * height}
{

}

Buffer::Buffer(const Buffer& other) :
    width_{other.width_},
    height_{other.height_},
    chars_{other.chars_}
{

}

Buffer::Buffer(Buffer&& other) :
    width_{other.width_},
    height_{other.height_},
    chars_{std::move(other.chars_)}
{

}

std::size_t Buffer::getWidth() const
{
    return width_;
}

std::size_t Buffer::getHeight() const
{
    return height_;
}

void Buffer::putCharAt(std::size_t x, std::size_t y, std::uint32_t charCode)
{
    CHAR_INFO& charInfo = chars_.at(y * width_ + x);
    charInfo.Attributes = FOREGROUND_BLUE | BACKGROUND_GREEN;
    charInfo.Char.UnicodeChar = static_cast<WCHAR>(charCode);
}

void Buffer::display()
{
    COORD bufferSize;
    bufferSize.X = static_cast<SHORT>(width_);
    bufferSize.Y = static_cast<SHORT>(height_);

    SMALL_RECT region;
    region.Left = 0;
    region.Top = 0;
    region.Right = bufferSize.X;
    region.Bottom = bufferSize.Y;

    WriteConsoleOutput(GetStdHandle(STD_OUTPUT_HANDLE), chars_.data(), bufferSize, {0, 0}, &region);
}