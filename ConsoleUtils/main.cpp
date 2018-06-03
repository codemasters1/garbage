#include <chrono>
#include <iostream>
#include <fstream>
#include <thread>
#include "Buffer.hpp"

using namespace std::chrono_literals;

int main()
{
    /*console::Buffer consoleBuffer{120, 50};

    int i = 0;

    while (1)
    {
        consoleBuffer.putCharAt(i++ % 80, 0, 'a');
        consoleBuffer.display();
        std::this_thread::sleep_for(100ms);
    }*/

    int

    std::ofstream ofs{"test.bin", std::ios::binary};
    ofs.write(macko, sizeof(macko));

    return 0;
}