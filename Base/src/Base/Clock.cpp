#include "Base/Common.h"
#include "Base/Clock.h"

#include <chrono>

namespace base
{
	using namespace std::chrono;

	Clock::Clock() :
		m_time(0)
	{
	}

	void Clock::update()
	{
		steady_clock::time_point tp = steady_clock::now();
		steady_clock::duration d = tp.time_since_epoch();
		m_time = duration_cast<milliseconds>(d).count();
	}

	time_t Clock::getTime() const
	{
		return m_time;
	}
}