//
// Created by DN on 25.06.2017.
//

#pragma once


#include "Object.hpp"

class ObjectHelper
{
public:
    static void* getRawMember(Object* obj, std::string name);

    template <typename T>
    static T* getMember(Object* obj, std::string name)
    {
        return reinterpret_cast<T*>(getRawMember(obj, name));
    }

    template <typename T>
    static bool setMemberValue(Object* obj, std::string name, T value)
    {
        T* ptr = ObjectHelper::getMember<T>(obj, name);
        if (ptr == nullptr)
            return false;

        *ptr = value;
        return true;
    }

    static Object* createObject(Class* cls);
    static void destroyObject(Object* obj);
    static bool setInteger(Object* obj, std::string name, std::int64_t value);
    static bool setFloat(Object* obj, std::string name, double value);
    static void callMethod(Object* obj, std::string methodName);
    static bool isObjectInstanceOf(Object* obj, Class* cls);
    static void dumpObject(Object* obj);
};


