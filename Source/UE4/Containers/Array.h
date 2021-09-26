#pragma once

#include "HAL/Allocator/AnsiAllocator.h"
#include "HAL/Allocator/DefaultAllocatorPolicies.h"
#include "ChooseClass.h"
#include <iterator>

namespace UE4
{

    template <typename InElementType>
    class TArray
    {
    public:
        typedef typename int32 SizeType;
        typedef InElementType ElementType;
        //typedef InAllocator   Allocator;
    public:
        typedef ElementType *RangedForIteratorType;
        typedef const ElementType *RangedForConstIteratorType;

        /**
	 * DO NOT USE DIRECTLY
	 * STL-like iterators to enable range-based for loop support.
	 */
        RangedForIteratorType begin() { return GetData(); }
        RangedForConstIteratorType begin() const { return GetData(); }
        RangedForIteratorType end() { return GetData() + Num(); }
        RangedForConstIteratorType end() const { return GetData() + Num(); }

        ElementType *GetData() const { return reinterpret_cast<ElementType *>(AllocatorInstance); }
        int32 Num() const { return ArrayNum; }

    protected:
        void *AllocatorInstance;
        SizeType ArrayNum;
        SizeType ArrayMax;
    };
}
