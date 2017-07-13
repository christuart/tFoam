#ifndef TIMING_HH
#define TIMING_HH

#ifdef _WIN32
#include <Windows.h>
#else
#include <sys/time.h>
#include <ctime>
#endif

#include <iostream>
#include <sstream>
#include <cstdio>

typedef long long int64; typedef unsigned long long uint64;

// Andreas Bonini's timing method from http://stackoverflow.com/a/1861337
/* Returns the amount of milliseconds elapsed since the UNIX epoch. Works on both
 * windows and linux. */
uint64 GetTimeMs64();

double average_historic_time(double _history_times[], int _history_count);

std::string format_time(double time);
	
#endif
