#pragma once

#include "Windows.h"

/**
 * This is the Windows version of a critical section. It uses an aggregate
 * CRITICAL_SECTION to implement its locking.
 */
class FWindowsCriticalSection
{
public:
	/**
	 * The windows specific critical section
	 */
	Windows::CRITICAL_SECTION CriticalSection;
};

/** System-Wide Critical Section for windows using mutex */
class FWindowsSystemWideCriticalSection
{
public:
	Windows::HANDLE Mutex;
};

/**
 * FWindowsRWLock - Read/Write Mutex
 *	- Provides non-recursive Read/Write (or shared-exclusive) access.
 *	- Windows specific lock structures/calls Ref: https://msdn.microsoft.com/en-us/library/windows/desktop/aa904937(v=vs.85).aspx
 */
class FWindowsRWLock
{
private:
	Windows::SRWLOCK Mutex;
};

typedef FWindowsCriticalSection FCriticalSection;
typedef FWindowsSystemWideCriticalSection FSystemWideCriticalSection;
typedef FWindowsRWLock FRWLock;
