#pragma once

#include <initializer_list>
#include "Primitives.h"
#include "TypeTraits.h"
#include "HAL/Allocator/DefaultAllocatorPolicies.h"

/**
 * Base dynamic array.
 * An untyped data array; mirrors a TArray's members, but doesn't need an exact C++ type for its elements.
 **/
template <typename AllocatorType>
class TScriptArray
	: protected AllocatorType::ForAnyElementType
{
public:

	int32	  ArrayNum;
	int32	  ArrayMax;
};

template<typename AllocatorType> struct TIsZeroConstructType<TScriptArray<AllocatorType>> { enum { Value = true }; };

class FScriptArray : public TScriptArray<FHeapAllocator>
{
	using Super = TScriptArray<FHeapAllocator>;
};

template<> struct TIsZeroConstructType<FScriptArray> { enum { Value = true }; };
