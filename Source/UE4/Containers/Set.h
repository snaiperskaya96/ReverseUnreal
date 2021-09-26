#pragma once

template<typename InElementType>
class TSet
{
	InElementType* Elements;

	mutable int32 Hash;
	mutable int32	 HashSize;

};

//	typedef TSet<ElementType, KeyFuncs, SetAllocator> ElementSetType;
