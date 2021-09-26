#pragma once

#include "DefaultAllocatorPolicies.h"
#include "Primitives.h"

/** A type which is used to represent a script type that is unknown at compile time. */
struct FScriptContainerElement
{
};

/** Allocator that allocates memory using standard library functions. */
class FAnsiAllocator
{
public:
	using SizeType = int32;

	enum { NeedsElementType = false };
	enum { RequireRangeCheck = true };

	typedef FAnsiAllocator ElementAllocator;
	typedef FAnsiAllocator BitArrayAllocator;

	class ForAnyElementType
	{
		/** A pointer to the container's elements. */
		FScriptContainerElement* Data;
	};

	template<typename ElementType>
	class ForElementType : public ForAnyElementType
	{
	};
};


/** ANSI allocator that can be used with a TSet. */
//class FAnsiSetAllocator : public TSetAllocator<FAnsiAllocator, TInlineAllocator<1, FAnsiAllocator>>
//{	
//};

