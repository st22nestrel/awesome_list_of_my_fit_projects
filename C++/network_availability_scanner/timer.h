/**
 * IPK second project - variant Delta
 * @author Timotej Ponek, xponek00
 * @file timer.h
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>

/**
 * Class that implements timer via alarm signal
 * methods implemented in this Class are based on answer https://stackoverflow.com/a/45443535 
 * from user3206070 (https://stackoverflow.com/users/3206070/user3206070)
 * to question how to stop while-loop execution after 60 seconds (https://stackoverflow.com/q/45442963)
 */
class Timer
{
private:
    static struct itimerval timer;
    static bool hasPassed;
    static void sigTimerStop(int signum);
public:
    static void TimerSet(int waitTime);
    static void TimerStart();
    static void TimerStop();
    static bool HasPassed();
};