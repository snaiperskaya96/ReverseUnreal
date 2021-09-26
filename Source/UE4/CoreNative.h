#pragma once

#define RESULT_DECL void*const RESULT_PARAM
typedef void (*FNativeFuncPtr)(class UObject* Context, struct FFrame& TheStack, RESULT_DECL);