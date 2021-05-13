#include "Console.h"

int main()
{
    RCD::TimeRecorder *timeRecorder = new Console;
    timeRecorder->exec();

    return 0;
}