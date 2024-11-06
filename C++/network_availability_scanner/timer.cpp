/**
 * IPK second project - variant Delta
 * @author Timotej Ponek, xponek00
 * @file timer.cpp
 */

#include "timer.h"

// forward declarations of static members
itimerval Timer::timer;
bool Timer::hasPassed;

/**
 * @brief Called when timer expires and SIGALARM is raised
 * @param signum
 */
void Timer::sigTimerStop(int signum) {
    hasPassed = true;
    return;
}

/**
 * @brief Set timer that will be used for all SIGALARM signals
 * @param waitTime time to wait in ms
 */
void Timer::TimerSet(int waitTime) {
    if(waitTime >= 1000){
        timer.it_value.tv_sec = waitTime/1000;
        timer.it_value.tv_usec = (waitTime%1000) * 1000;
    }
    else{
        timer.it_value.tv_sec = 0;
        timer.it_value.tv_usec = waitTime * 1000;
    }
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 0;
}

/**
 * @brief Start timer for SIGALARM signal
 */
void Timer::TimerStart(){
    hasPassed = false;
    if (signal(SIGALRM, sigTimerStop) == SIG_ERR) {
        perror("Unable to catch SIGALRM");
        exit(1);
    }
    if (setitimer(ITIMER_REAL, &timer, NULL) == -1) {
        perror("error calling sigsetitimer()");
        exit(1);
    }
}

/**
 * @brief Ends timer for SIGALARM signal
 */
void Timer::TimerStop(){
    if (signal(SIGALRM, sigTimerStop) == SIG_ERR) {
        perror("Unable to catch SIGALRM");
        exit(1);
    }
    if (setitimer(ITIMER_REAL, 0, NULL) == -1) {
        perror("error calling sigsetitimer()");
        exit(1);
    }
}

/**
 * @brief Getter -> answers to whether timer on SIGALARM has passed
 * @return bool
 */
bool Timer::HasPassed(){ return hasPassed; }