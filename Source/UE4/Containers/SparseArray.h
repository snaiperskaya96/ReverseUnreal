#pragma once

#include "Primitives.h"

// Forward declarations.

template<typename InElementType /*= FDefaultSparseArrayAllocator */>
class TSparseArray
{
	using ElementType = InElementType;

	typedef TArray<FElementOrFreeListLink,typename Allocator::ElementAllocator> DataType;
	DataType Data;

	typedef TBitArray<> AllocationBitArrayType;
	AllocationBitArrayType AllocationFlags;

	/** The index of an unallocated element in the array that currently contains the head of the linked list of free elements. */
	int32 FirstFreeIndex;

	/** The number of elements in the free list. */
	int32 NumFreeIndices;
};
