#pragma once

#include "Primitives.h"
#include "ContainersAllocationPolicy.h"
#include "HAL/Allocator/DefaultAllocatorPolicies.h"
#include "Traits/IsVoidType.h"

template<typename Allocator > class TBitArray;

// Functions for manipulating bit sets.
struct FBitSet
{
	static constexpr uint32 BitsPerWord = NumBitsPerDWORD;
};


// Forward declaration.
template<typename Allocator = FDefaultBitArrayAllocator>
class TBitArray;

template<typename Allocator = FDefaultBitArrayAllocator>
class TConstSetBitIterator;

template<typename Allocator = FDefaultBitArrayAllocator,typename OtherAllocator = FDefaultBitArrayAllocator, bool Both=true>
class TConstDualSetBitIterator;

template <typename AllocatorType, typename InDerivedType = void>
class TScriptBitArray;

/** Flag enumeration for control bitwise operator functionality */
enum class EBitwiseOperatorFlags
{
	/** Specifies that the result should be sized Max(A.Num(), B.Num()) */
	MaxSize = 1 << 0,
	/** Specifies that the result should be sized Min(A.Num(), B.Num()) */
	MinSize = 1 << 1,
	/** Only valid for self-mutating bitwise operators - indicates that the size of the LHS operand should not be changed */
	MaintainSize = 1 << 2,

	/** When MaxSize or MaintainSize is specified and the operands are sized differently, any missing bits in the resulting bit array will be considered as 1, rather than 0 */
	OneFillMissingBits = 1 << 4,
};

/** Used to read/write a bit in the array as a bool. */
class FBitReference
{
public:

	FORCEINLINE FBitReference(uint32& InData,uint32 InMask)
	:	Data(InData)
	,	Mask(InMask)
	{}

	uint32& Data;
	uint32 Mask;
};


/** Used to read a bit in the array as a bool. */
class FConstBitReference
{
public:

	FORCEINLINE FConstBitReference(const uint32& InData,uint32 InMask)
	:	Data(InData)
	,	Mask(InMask)
	{}

	const uint32& Data;
	uint32 Mask;
};


/** Used to reference a bit in an unspecified bit array. */
class FRelativeBitReference
{
public:
	FORCEINLINE explicit FRelativeBitReference(int32 BitIndex)
		: DWORDIndex(BitIndex >> NumBitsPerDWORDLogTwo)
		, Mask(1 << (BitIndex & (NumBitsPerDWORD - 1)))
	{
	}

	int32  DWORDIndex;
	uint32 Mask;
};

class FBitArrayMemory
{
public:

};

/**
 * A dynamically sized bit array.
 * An array of Booleans.  They stored in one bit/Boolean.  There are iterators that efficiently iterate over only set bits.
 */
template<typename Allocator /*= FDefaultBitArrayAllocator*/>
class TBitArray
{
	uint32* AllocatorInstance;
	int32         NumBits;
	int32         MaxBits;
};

