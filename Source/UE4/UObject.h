#pragma once

#include "NameTypes.h"
#include "ObjectMacros.h"

namespace UE4
{
	class UObjectBase
	{
	public:
		void *Vfptr;
		/** Flags used to track and report various object states. This needs to be 8 byte aligned on 32-bit
	    platforms to reduce memory waste */
		EObjectFlags ObjectFlags;

		/** Index into GObjectArray...very private. */
		int32 InternalIndex;

		/** Class the object belongs to. */
		class UClass *ClassPrivate;

		/** Name of this object */
		FName NamePrivate;

		/** Object this object resides in. */
		class UObject *OuterPrivate;
	};

	class UObjectBaseUtility : public UObjectBase
	{
	};

	class UObject : public UObjectBaseUtility
	{
	};

	struct FUObjectItem
	{
	public:
		// Pointer to the allocated object
		class UObjectBase *Object;
		// Internal flags
		int32 Flags;
		// UObject Owner Cluster Index
		int32 ClusterRootIndex;
		// Weak Object Pointer Serial number associated with the object
		int32 SerialNumber;

	};

	struct FUObjectItemDev : public FUObjectItem
	{
		/** Stat id of this object, 0 if nobody asked for it yet */
		mutable void *StatID;
	};

	class FChunkedFixedUObjectArray
	{
	public:
		enum
		{
			NumElementsPerChunk = 64 * 1024,
		};

		/** Master table to chunks of pointers **/
		FUObjectItem **Objects;
		/** If requested, a contiguous memory where all objects are allocated **/
		FUObjectItem *PreAllocatedObjects;
		/** Maximum number of elements **/
		int32 MaxElements;
		/** Number of elements we currently have **/
		int32 NumElements;
		/** Maximum number of chunks **/
		int32 MaxChunks;
		/** Number of chunks we currently have **/
		int32 NumChunks;

		FUObjectItem const *GetObjectPtr(int32 Index) const
		{
			const int32 ChunkIndex = Index / NumElementsPerChunk;
			const int32 WithinChunkIndex = Index % NumElementsPerChunk;
			if (Unreal::bIsProd)
			{
				FUObjectItem *Chunk = Objects[ChunkIndex];
				return Chunk + WithinChunkIndex;
			}
			else
			{
				FUObjectItemDev *Chunk = static_cast<FUObjectItemDev*>(Objects[ChunkIndex]);
				return Chunk + WithinChunkIndex;
			}
		}

		FUObjectItem *GetObjectPtr(int32 Index)
		{
			const int32 ChunkIndex = Index / NumElementsPerChunk;
			const int32 WithinChunkIndex = Index % NumElementsPerChunk;
			if (Unreal::bIsProd)
			{
				FUObjectItem *Chunk = Objects[ChunkIndex];
				return Chunk + WithinChunkIndex;
			}
			else
			{
				FUObjectItemDev *Chunk = static_cast<FUObjectItemDev*>(Objects[ChunkIndex]);
				return Chunk + WithinChunkIndex;
			}
		}
	};

	class FUObjectArray
	{
	public:
		enum ESerialNumberConstants
		{
			START_SERIAL_NUMBER = 1000,
		};
		//typedef TStaticIndirectArrayThreadSafeRead<UObjectBase, 8 * 1024 * 1024 /* Max 8M UObjects */, 16384 /* allocated in 64K/128K chunks */ > TUObjectArray;
		typedef FChunkedFixedUObjectArray TUObjectArray;

		// note these variables are left with the Obj prefix so they can be related to the historical GObj versions

		/** First index into objects array taken into account for GC.							*/
		int32 ObjFirstGCIndex;
		/** Index pointing to last object created in range disregarded for GC.					*/
		int32 ObjLastNonGCIndex;
		/** Maximum number of objects in the disregard for GC Pool */
		int32 MaxObjectsNotConsideredByGC;

		/** If true this is the intial load and we should load objects int the disregarded for GC range.	*/
		bool OpenForDisregardForGC;
		/** Array of all live objects.											*/
		TUObjectArray ObjObjects;
		/** Synchronization object for all live objects.											*/
		//FCriticalSection ObjObjectsCritical;
		/** Available object indices.											*/
		//TLockFreePointerListUnordered<int32, PLATFORM_CACHE_LINE_SIZE> ObjAvailableList;
#if UE_GC_TRACK_OBJ_AVAILABLE
		/** Available object index count.										*/
		//FThreadSafeCounter ObjAvailableCount;
#endif
		/**
	 * Array of things to notify when a UObjectBase is created
	 */
		//TArray<FUObjectCreateListener* > UObjectCreateListeners;
		/**
	 * Array of things to notify when a UObjectBase is destroyed
	 */
		//TArray<FUObjectDeleteListener* > UObjectDeleteListeners;
#if THREADSAFE_UOBJECTS
		//FCriticalSection UObjectDeleteListenersCritical;
#endif

		/** Current master serial number **/
		//FThreadSafeCounter	MasterSerialNumber;
	};
}
