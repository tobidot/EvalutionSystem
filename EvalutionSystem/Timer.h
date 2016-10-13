#pragma once
#include <chrono>

class Timer
{
	std::chrono::time_point<std::chrono::system_clock> timer_start;
public:
	Timer();
	~Timer();
	// returns time past since last call of reset in seconds
	float get() const;
	// returns time past since last call of reset in nanoseconds
	float get_nanoseconds() const;
	// returns time past since last call of reset in microseconds
	float get_microseconds() const;
	// returns time past since last call of reset in miliseconds
	float get_miliseconds() const;
	// returns time past since last call of reset in seconds
	float get_seconds() const;
	// resets the timer to count from now on
	void reset();
};

