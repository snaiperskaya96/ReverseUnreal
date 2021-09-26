#pragma once

#include "Primitives.h"

class FThreadSafeCounter
{
public:
	typedef int32 IntegerType;
	/** Thread-safe counter */
	volatile int32 Counter;
};
