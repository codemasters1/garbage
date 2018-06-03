//
// Created by DN on 20.02.2017.
//

#include <algorithm>
#include <sstream>
#include <iostream>
#include <array>
#include <queue>
#include <stack>
#include "Class.hpp"

std::size_t Class::memberOffset()
{
    return super_ != nullptr
           ? super_->memberOffset() + members_.size() * sizeof(void*)
           : offsetof(Object, reserved);
}

std::size_t Class::size()
{
    const auto baseSize = super_ != nullptr
                          ? super_->size()
                          : sizeof(Object);
    const auto membersSize = members_.size() * sizeof(void*);
    return baseSize + membersSize;
}

void Class::addMember(std::string name, MemberType type)
{
    members_.emplace_back(name, type);
}

void Class::addMethod(std::string name, MethodBody body)
{
    methods_.emplace_back(name, body);
}