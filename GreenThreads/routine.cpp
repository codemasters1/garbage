//
// Created by DN on 19.01.2017.
//
#include <array>
#include <functional>
#include <algorithm>
#include <cstring>
#include <cassert>
#include "routine.hpp"

namespace gt
{

/**
 * Slots for routines.
 */
static std::array<Routine, 4> routines;

/**
 * Pointer to the currently running routine.
 */
static Routine* currentRoutine;

/**
 * Pretty-prints the information about slots.
 */
void dumpRoutines()
{
    printf("Routines (%zu):\n", routines.size());

    for (int i = 0; i < routines.size(); ++i)
    {
        const Routine& routine = routines[i];

        std::string state;
        switch (routine.state)
        {
        case RoutineState::Ready:
            state = "ready";
            break;
        case RoutineState::Running:
            state = "running";
            break;
        case RoutineState::Unused:
            state = "unused";
            break;
        case RoutineState::Terminated:
            state = "terminated";
            break;
        }

        printf(
            "  Routine #%d\n"
            "    State:         %s\n"
            "    Stack Address: 0x%p\n"
            "    Stack Size:    0x%08x\n",
            i, state.c_str(), routine.stack, routine.stackSize
        );
    }
}

extern "C" __attribute__((sysv_abi))
void switchContext_impl(RoutineContext& old_ctx, const RoutineContext& new_ctx);

/**
 * Stores current content of the registers in oldContext and loads new from newContext.
 * @param oldContext context of currently running routine
 * @param newContext context of the routine that will replace current one
 */
void switchContext(RoutineContext& oldContext, const RoutineContext& newContext)
{
    assert(&oldContext != nullptr);
    assert(&newContext != nullptr);

    switchContext_impl(oldContext, newContext);
}

static Routine* findFirstRoutine()
{
    // We need to find routine that is ready to be executed...
    auto it = std::find_if(std::begin(routines), std::end(routines), [](const Routine& routine) {
        return routine.state == RoutineState::Ready;
    });

    return it == std::end(routines) ? nullptr : it;
}

static Routine* findNextRoutine()
{
    for (Routine* p = currentRoutine; ; )
    {
        if (++p == routines.end())
            p = &routines[0];

        if (p == currentRoutine)
            // There are no ready routines
            return nullptr;

        if (p->state == RoutineState::Ready)
            return p;
    }
}

bool yield()
{
    if (currentRoutine == nullptr)
    {
        // We have no running routines so we need to load any that is ready.

        Routine* nextRoutine = findFirstRoutine();
        if (nextRoutine == nullptr)
            return false;

        RoutineContext oldContext{};
        RoutineContext& newContext = nextRoutine->context;

        currentRoutine = nextRoutine;
        currentRoutine->state = RoutineState::Running;

        switchContext(oldContext, newContext);
    }
    else
    {
        // Some routine is already running so we have to suspend current one
        // and run another which is ready to be executed.

        Routine* nextRoutine = findNextRoutine();
        if (nextRoutine == nullptr)
            return false;

        if (currentRoutine->state == RoutineState::Running)
            currentRoutine->state = RoutineState::Ready;

        RoutineContext& oldContext = currentRoutine->context;
        RoutineContext& newContext = nextRoutine->context;

        currentRoutine = nextRoutine;
        currentRoutine->state = RoutineState::Running;

        switchContext(oldContext, newContext);
    }

    return true;
}

void terminate()
{
    currentRoutine->state = RoutineState::Terminated;
    while (gt::yield())
        ;
}

int getCurrentId()
{
    auto it = std::find_if(std::begin(routines), std::end(routines), [](const Routine& routine) {
        return routine.state == RoutineState::Running;
    });

    if (it == std::end(routines))
        return -1;

    return (int) std::distance(std::begin(routines), it);
}

void rtReturn(int ret)
{
    while (gt::yield())
        ;

    exit(ret);
}

static void routineTerminationHandler()
{
    currentRoutine->state = RoutineState::Terminated;
    yield();
}

static Routine* retrieveUnusedSlot()
{
    auto it = std::find_if(std::begin(routines), std::end(routines), [](const Routine& routine) {
        return routine.state == RoutineState::Unused;
    });

    return it == routines.end() ? nullptr : it;
}

int createTask(RoutineBody body)
{
    Routine* slot = retrieveUnusedSlot();
    if (slot == nullptr)
        return -1;

    const uint32_t stackSize = 0x10000;
    auto stack = new(std::nothrow) char[stackSize];
    if (stack == nullptr)
        return -1;

    *reinterpret_cast<uint64_t*>(&stack[stackSize - 8]) = reinterpret_cast<uint64_t>(routineTerminationHandler);
    *reinterpret_cast<uint64_t*>(&stack[stackSize - 16]) = reinterpret_cast<uint64_t>(body);

    Routine& rt = *slot;
    rt.stack = (uint64_t*)stack;
    rt.stackSize = stackSize;
    rt.context.rsp = (uint64_t)&stack[stackSize - 16];
    rt.state = RoutineState::Ready;

    return 0;
}

void startScheduler()
{
    rtReturn(0);
}

}