#pragma once
#include "bgspch.h"

namespace BIGOS {

	namespace Utils {

		struct Members;

		class Timer
		{
		private:
			byte m_Reserved[32];
			Members* m_Members;
		public:
			// Creates and starts timer
			Timer();
			// Resets and restarts timer
			virtual void Reset();
			// Returns time in seconds
			virtual float Elapsed();
			// Returns time in milliseconds
			virtual float ElapsedMillis();
		};

	}
}