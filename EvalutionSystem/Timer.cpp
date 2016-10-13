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
	return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now() - timer_start).count() * 0.000000001f;
}

float Timer::get_nanoseconds() const
{
	return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now() - timer_start).count();
}

float Timer::get_microseconds() const
{
	return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now() - timer_start).count()* 0.001f;
}

float Timer::get_miliseconds() const
{
	return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now() - timer_start).count()* 0.000001f;
}

float Timer::get_seconds() const
{
	return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now() - timer_start).count()* 0.000000001f;
}

void Timer::reset()
{
	timer_start = std::chrono::system_clock::now();
}
