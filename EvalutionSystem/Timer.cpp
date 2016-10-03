#include "Timer.h"

Timer::Timer()
{
	timer_start = std::chrono::system_clock::now();
}

Timer::~Timer()
{
}

float Timer::get() const
{
	return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - timer_start).count();
}

float Timer::get_nanoseconds() const
{
	return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now() - timer_start).count();
}

float Timer::get_miliseconds() const
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - timer_start).count();
}

float Timer::get_seconds() const
{
	return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - timer_start).count();
}

void Timer::reset()
{
	timer_start = std::chrono::system_clock::now();
}
