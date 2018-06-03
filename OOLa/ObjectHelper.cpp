//
// Created by DN on 25.06.2017.
//

#include <algorithm>
#include <stack>
#include "Member.hpp"
#include "Class.hpp"
#include "ObjectHelper.hpp"

void* ObjectHelper::getRawMember(Object* obj, std::string name)
{
    for (Class* c = obj->cls; c != nullptr; c = c->super_)
    {
        const auto begin = std::begin(c->members_);
        const auto end = std::end(c->members_);

        const auto it = std::find_if(begin, end, [&name](const Member& m) {
            return m.name() == name;
        });

        if (it != end)
        {
            const auto baseOffset = c->memberOffset();
            const auto memberOffset = sizeof(void*) * std::distance(begin, it);
            return obj + baseOffset + memberOffset;
        }
    }

    return nullptr;
}

void ObjectHelper::dumpObject(Object* obj)
{
    // Push all the super classes onto the stack
    // so we could easily list members starting from the root class

    std::stack<Class*> classes;
    for (Class* c = obj->cls; c != nullptr; c = c->super_)
    {
        classes.push(c);
    }

    while (!classes.empty())
    {
        Class* c = classes.top();
        for (const auto& member : c->members_)
        {
            switch (member.type())
            {
            case MemberType::Integer:
                printf(" %s (integer) -> %lld\n", member.name().c_str(), *getMember<std::int64_t>(obj, member.name()));
                break;
            case MemberType::Float:
                printf(" %s (float) -> %lf\n", member.name().c_str(), *getMember<double>(obj, member.name()));
                break;
            default:
                printf(" %s (unknown)\n", member.name().c_str());
            }
        }
        classes.pop();
    }
}

bool ObjectHelper::isObjectInstanceOf(Object* obj, Class* cls)
{
    for (Class* c = obj->cls; c != nullptr; c = c->super_)
        if (c == cls)
            return true;

    return false;
}

void ObjectHelper::callMethod(Object* obj, std::string methodName)
{
    for (Class* c = obj->cls; c != nullptr; c = c->super_)
    {
        const auto begin = std::begin(c->methods_);
        const auto end = std::end(c->methods_);

        const auto it = std::find_if(begin, end, [&methodName](const Member& m) {
            return m.name() == methodName;
        });

        if (it != end)
        {
            it->body()(obj);
            return;
        }
    }
}

bool ObjectHelper::setInteger(Object* obj, std::string name, std::int64_t value)
{
    return setMemberValue(obj, name, value);
}

bool ObjectHelper::setFloat(Object* obj, std::string name, double value)
{
    return setMemberValue(obj, name, value);
}

Object* ObjectHelper::createObject(Class* cls)
{
    const auto numBytes = cls->size();
    auto buffer = new uint8_t[numBytes];
    std::fill_n(buffer, numBytes, 0);

    auto obj = reinterpret_cast<Object*>(buffer);
    obj->cls = cls;
    return obj;
}

void ObjectHelper::destroyObject(Object* obj)
{
    delete[] reinterpret_cast<std::uint8_t*>(obj);
}
