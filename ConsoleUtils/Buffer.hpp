//
// Created by Host on 08.08.2016.
//

#pragma once

#include <cstdint>
#include <vector>
#include <windows.h>

namespace console
{
    class Buffer
    {
    public:
        Buffer() = delete;
        Buffer(std::size_t width, std::size_t height);
        Buffer(const Buffer& other);
        Buffer(Buffer&& other);
        ~Buffer() = default;

        std::size_t getWidth() const;
        std::size_t getHeight() const;

        void putCharAt(std::size_t x, std::size_t y, std::uint32_t charCode);
        void display();

    private:
        std::size_t width_, height_;
        std::vector<CHAR_INFO> chars_;
    };
}
