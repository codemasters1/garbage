#include <windows.h>
#include <cstdint>
#include <cstdio>
#include "routine.hpp"

void f()
{
    const int id = gt::getCurrentId() + 1;

    for (int i = 0; i < id * 3; i++)
    {
        printf("@%d %d\n", id, i);
        //gt::dumpRoutines();
        gt::yield();
    }

    //while (gt::yield());
}

void aa()
{
    printf("siema");
}

int main()
{
    for (int i = 0; i < 3; ++i)
        gt::createTask(f);

    gt::dumpRoutines();
    gt::startScheduler();

//    auto section = (uint8_t*)VirtualAlloc(nullptr, 4096, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
//    memcpy(&section[0], "\x50\x48\xB8\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xFF\xD0\x58\xCC", 14);
//    memcpy(&section[3], (uint8_t*)aa, 8);
//
//    asm("int3");
//
//    for (int i = 0; i < 16; ++i)
//        printf("%02x", section[i]);
//
//    ((void(*)())(section))();
}