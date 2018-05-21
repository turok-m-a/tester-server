#include "loadtest.h"
loadTest::loadTest()
{

}

void loadTest::run()
{
    while(*pause);
    LARGE_INTEGER frequency;        // ticks per second
    LARGE_INTEGER t1, t2;           // ticks
    double elapsedTime;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&t1);
    Network & network = Network::getInstance();
    network.sendQuery(GET_EXAM_LIST,r);
    QueryPerformanceCounter(&t2);
    time = (t2.QuadPart - t1.QuadPart) * 1000.0 / frequency.QuadPart;
    std::cout << time << std::endl;
}
