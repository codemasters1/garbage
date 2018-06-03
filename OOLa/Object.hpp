//
// Created by DN on 20.02.2017.
//

#pragma once

#include <cstdint>

class Class;

struct Object
{
    Class* cls;
    std::uint8_t reserved[0];
    // .. members ...
};
