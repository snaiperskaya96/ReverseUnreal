#pragma once

#include "Models.h"

template<typename T>
struct THasTypeLayout
{
	static const bool Value = TModels<CTypeLayout, T>::Value;
};
