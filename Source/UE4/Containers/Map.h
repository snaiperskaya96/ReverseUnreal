#pragma once 

#include "Pair.h"
#include "HAL/Allocator/DefaultAllocatorPolicies.h"
#include "TypeTraits.h"
#include "Set.h"
#include "ChooseClass.h"

template <typename KeyType, typename ValueType>
class TMap
{
	typedef TPair<KeyType, ValueType> ElementType;
	typedef TSet<ElementType> ElementSetType;

    ElementSetType Pairs;
};
