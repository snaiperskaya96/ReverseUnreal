#pragma once

namespace Windows
{
   	struct CRITICAL_SECTION { void* Opaque1[1]; long Opaque2[2]; void* Opaque3[3]; };
	typedef void* HANDLE;

    struct SRWLOCK
	{
		void* Ptr;
	};
}