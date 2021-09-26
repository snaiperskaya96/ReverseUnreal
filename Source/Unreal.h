#pragma once

#include <cstdint>

namespace UE4 
{
    class FUObjectArray;
    class FNamePool;
    class UWorldProxy;
}

class Unreal
{
public:
    static void FindGWorld(const char* ModuleName, const char* Pattern);
    static void FindNamePool(const char* ModuleName, const char* Pattern);
    static void FindGUObjectArray(const char* ModuleName, const char* Pattern);
public:
    static bool bIsProd;
	static UE4::FUObjectArray* GUObjectArray;
    static UE4::FNamePool* NamePool;
    static UE4::UWorldProxy* GWorld;
    static uint64_t GWorldAddress;
    static uint64_t NamePoolAddress;
    static uint64_t GUObjectArrayAddress; 
};