//
// Created by DN on 20.02.2017.
//

#pragma once

#include <functional>
#include <string>

class Object;

using MethodBody = std::function<void(Object*)>;

enum class MemberType
{
    Integer,
    String,
    Float,
    Object,
    Method
};

class Member
{
private:
    std::string name_;
    MemberType type_;
    MethodBody body_;

public:
    Member(std::string name, MemberType type) : name_{name}, type_{type} {}
    Member(std::string name, MethodBody body) : name_{name}, type_{MemberType::Method}, body_{body} {}
    std::string name() const { return name_; }
    MemberType type() const { return type_; }
    MethodBody body() const { return body_; }
};
