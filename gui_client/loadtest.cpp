#include "loadtest.h"
loadTest::loadTest()
{

}

void loadTest::run()
{
    while(*pause);
    Network & network = Network::getInstance();
    network.sendQuery(GET_EXAM_LIST,r);
    qDebug() << "*";
}
