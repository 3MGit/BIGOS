#pragma once

#include <chrono>

namespace BIGOS {

	class ScopedTimer
	{
	public:
		ScopedTimer()
		{
			Reset();
		}

		void ScopedTimer::Reset()
		{
			m_Start = std::chrono::high_resolution_clock::now();
		}

		float ScopedTimer::Elapsed()
		{
			return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - m_Start).count() * 0.001f * 0.001f * 0.001f;
		}

		float ScopedTimer::ElapsedMillis()
		{
			return Elapsed() * 1000.0f;
		}

	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
	};

}