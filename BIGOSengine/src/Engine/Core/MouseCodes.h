#pragma once
#include "bgspch.h"

namespace BIGOS
{
	using MouseCode = uint32_t;

	namespace Mouse
	{
		enum : MouseCode
		{
			//TODO: Swap to hexadecimal for win 32api
			Button0 = 0,
			Button1 = 1,
			Button2 = 2,
			Button3 = 3,
			Button4 = 4,
			Button5 = 5,
			Button6 = 6,
			Button7 = 7,
			
			ButtonLast = Button7,
			ButtonLeft = 0x01,
			ButtonRight = 0x02,
			ButtonMiddle = 0x04
		};
	}
}