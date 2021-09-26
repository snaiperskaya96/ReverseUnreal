#pragma once

#include "Containers/Array.h"

namespace UE4
{

	struct FGCReferenceTokenStream
	{
		/** Initialization value to ensure that we have the right skip index index */
		enum EGCArrayInfoPlaceholder
		{
			E_GCSkipIndexPlaceholder = 0xDEADBABE
		};

		/** Constructor */

		/** Token array */
		TArray<uint32> Tokens;
	};
}
