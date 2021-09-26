#pragma once

#include "Primitives.h"
#include "Platform.h"
#include "Unreal.h"

namespace UE4
{
	struct FNameEntryId
	{
		uint32 Value;
		uint32 ToUnstableInt() const { return Value; }
	};

	/**
 * FNameEntry const* FName::GetEntry(FNameEntryId Id)
{
	return &GetNamePool().Resolve(Id);
}
*/

	struct FNameEntryHeader
	{
		uint16 bIsWide : 1;
#if WITH_CASE_PRESERVING_NAME
		uint16 Len : 15;
#else
		static constexpr uint32 ProbeHashBits = 5;
		uint16 LowercaseProbeHash : ProbeHashBits;
		uint16 Len : 10;
#endif
	};

	enum
	{
		NAME_SIZE = 1024
	};

	struct FNameEntry
	{
	public:
		FNameEntryHeader Header;
		union
		{
			ANSICHAR AnsiName[NAME_SIZE];
			WIDECHAR WideName[NAME_SIZE];
		};

	public:
		/** Returns whether this name entry is represented via WIDECHAR or ANSICHAR. */
		FORCEINLINE bool IsWide() const
		{
			return Header.bIsWide;
		}

		FORCEINLINE int32 GetNameLength() const
		{
			return Header.Len;
		}
	};

	enum
	{
		FNameMaxBlockBits = 13
	}; // Limit block array a bit, still allowing 8k * block size = 1GB - 2G of FName entry data
	enum
	{
		FNameBlockOffsetBits = 16
	};
	enum
	{
		FNameMaxBlocks = 1 << FNameMaxBlockBits
	};
	enum
	{
		FNameBlockOffsets = 1 << FNameBlockOffsetBits
	};

	struct FNameEntryHandle
	{
		uint32 Block = 0;
		uint32 Offset = 0;

		FNameEntryHandle(uint32 InBlock, uint32 InOffset)
			: Block(InBlock), Offset(InOffset)
		{
		}

		FNameEntryHandle(FNameEntryId Id)
			: Block(Id.ToUnstableInt() >> FNameBlockOffsetBits), Offset(Id.ToUnstableInt() & (FNameBlockOffsets - 1))
		{
		}
	};

	class FNameEntryAllocator
	{
	public:
		enum
		{
			Stride = alignof(FNameEntry)
		};
		enum
		{
			BlockSizeBytes = Stride * FNameBlockOffsets
		};

		/** Initializes all member variables. */
		FNameEntry &Resolve(FNameEntryHandle Handle) const
		{
			// Lock not needed
			return *reinterpret_cast<FNameEntry *>(Blocks[Handle.Block] + Stride * Handle.Offset);
		}

		mutable FRWLock Lock;
		uint32 CurrentBlock = 0;
		uint32 CurrentByteCursor = 0;
		uint8 *Blocks[FNameMaxBlocks] = {};
	};

	class FNamePool
	{
	public:
		enum
		{
			MaxENames = 512
		};
		FNameEntryAllocator Entries;
	};

	class FName
	{
	public:
		/** Index into the Names array (used to find String portion of the string/number pair used for comparison) */
		FNameEntryId ComparisonIndex;
		/** Number portion of the string/number pair (stored internally as 1 more than actual, so zero'd memory will be the default, no-instance case) */
		uint32 Number;

		const ANSICHAR *GetAnsi() const
		{
			return GetEntry().AnsiName;
		}

		const FNameEntry &GetEntry() const
		{
			FNameEntry &Entry = Unreal::NamePool->Entries.Resolve(ComparisonIndex);
			return Entry;
		}
	};
}
