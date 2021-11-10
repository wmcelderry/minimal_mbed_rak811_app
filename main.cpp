#include "events/mbed_events.h"
#include <mbed.h>


#define STARTUP_PRINT 1

int indx=0;

void report()
{
    printf("Still running: %d.\r\n",indx++);
}

int main(int argc,char *argv[])
{
#if defined STARTUP_PRINT
    printf("Started the target app.\r\n");
#endif

    EventQueue eq(100);
    eq.call_every(1s,report);

    eq.dispatch_forever();

    return 0;
}
