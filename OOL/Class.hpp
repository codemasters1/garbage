//
// Created by DN on 20.02.2017.
//

#pragma once

#include <algorithm>
#include <functional>
#include <string>
#include <vector>
#include "Member.hpp"
#include "Object.hpp"

class Class
{
private:
    std::string name_;
    std::vector<Member> members_;
    std::vector<Member> methods_;
    Class* super_;

public:
    Class(std::string name) :
        name_{name},
        super_{nullptr} {};

    Class(std::string name, Class* super) :
        name_{name},
        super_{super} {};

    std::string name() const {return name_;}
    std::size_t memberOffset();
    std::size_t size();

    void addMember(std::string, MemberType type);
    void addMethod(std::string, MethodBody body);

    friend class ObjectHelper;
};