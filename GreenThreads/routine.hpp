//
// Created by DN on 19.01.2017.
//

#pragma once

#include <cstdint>
#include <functional>

namespace gt
{
struct RoutineContext
{
    uint64_t rsp;
    uint64_t r15;
    uint64_t r14;
    uint64_t r13;
    uint64_t r12;
    uint64_t rbx;
    uint64_t rbp;
};

enum class RoutineState : uint8_t
{
    Unused,
    Running,
    Ready,
    Terminated
};

struct Routine
{
    RoutineContext context;
    RoutineState state;
    uint64_t* stack;
    uint32_t stackSize;
};

using RoutineBody = void (*)();

void startScheduler();
bool yield();
void terminate();
int getCurrentId();
int createTask(RoutineBody body);
void dumpRoutines();
}