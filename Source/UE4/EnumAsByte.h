#pragma once

#include "Primitives.h"

template<class TEnum>
class TEnumAsByte
{
public:
	typedef TEnum EnumType;
	uint8 Value;
};