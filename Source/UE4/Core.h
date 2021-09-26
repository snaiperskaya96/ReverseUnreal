#pragma once

#include "Primitives.h"
#include "NameTypes.h"
#include "Field.h"
#include "Containers/Array.h"
//#include "Containers/Map.h"
#include "Containers/Pair.h"

#ifndef FORCEINLINE
    #define FORCEINLINE inline
#endif

#ifdef _WIN32
#include "Windows/WindowsCriticalSection.h"
#endif

#define checkf(expr, format,  ...) ;
#define check(expr) ;

