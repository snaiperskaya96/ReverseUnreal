// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Containers/Map.h"
#include "UObject.h"

struct FWeakObjectPtr
{
	int32_t ObjectIndex;
	int32_t ObjectSerialNumber;
};

/**
 * FWeakObjectPtr is a weak pointer to a UObject. 
 * It can return nullptr later if the object is garbage collected.
 * It has no impact on if the object is garbage collected or not.
 * It can't be directly used across a network.
 *
 * Most often it is used when you explicitly do NOT want to prevent something from being garbage collected.
 */

template<class T=UObject, class TWeakObjectPtrBase=FWeakObjectPtr>
struct TWeakObjectPtr;

/**
 * TWeakObjectPtr is the templated version of the generic FWeakObjectPtr
 */
template<class T, class TWeakObjectPtrBase>
struct TWeakObjectPtr : private TWeakObjectPtrBase
{
	friend struct FFieldPath;
};

//template<class T> struct TIsPODType<TWeakObjectPtr<T> > { enum { Value = true }; };
//template<class T> struct TIsZeroConstructType<TWeakObjectPtr<T> > { enum { Value = true }; };
//template<class T> struct TIsWeakPointerType<TWeakObjectPtr<T> > { enum { Value = true }; };


/**
 * MapKeyFuncs for TWeakObjectPtrs which allow the key to become stale without invalidating the map.
 */
