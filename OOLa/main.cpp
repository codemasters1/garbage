#include "Class.hpp"
#include "ObjectHelper.hpp"

void method(Object* obj)
{
    printf("Hello from %p: %llu\n", obj, *ObjectHelper::getMember<std::uint64_t>(obj, "x"));
}

int main()
{
    Class cls1{"A"};
    cls1.addMember("x", MemberType::Integer);
    cls1.addMember("y", MemberType::Integer);
    cls1.addMethod("__add__", method);

    Class cls2{"B", &cls1};
    cls2.addMember("_x", MemberType::Integer);

    Class cls3{"C", &cls2};
    cls3.addMember("_y", MemberType::Integer);

    Object* o1 = ObjectHelper::createObject(&cls1);
    ObjectHelper::dumpObject(o1);

    Object* o3 = ObjectHelper::createObject(&cls3);
    ObjectHelper::setInteger(o3, "x", 123);
    ObjectHelper::callMethod(o1, "__add__");
    ObjectHelper::callMethod(o3, "__add__");
    ObjectHelper::dumpObject(o3);
    return 0;
}