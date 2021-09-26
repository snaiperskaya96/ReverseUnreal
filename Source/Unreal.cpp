#include "Unreal.h"

#include "Utils.h"
#include <iostream>
#include <Windows.h>

#include "UE4/UObject.h"
#include "UE4/NameTypes.h"

bool Unreal::bIsProd = false;

uint64_t Unreal::GWorldAddress = 0x0;
uint64_t Unreal::NamePoolAddress = 0x0;
uint64_t Unreal::GUObjectArrayAddress = 0x0;

UE4::FUObjectArray* Unreal::GUObjectArray = nullptr;
UE4::FNamePool* Unreal::NamePool = nullptr;
UE4::UWorldProxy* Unreal::GWorld = nullptr;

void Unreal::FindGWorld(const char *ModuleName, const char *Pattern)
{
    FModuleInfo Info = Utils::GetModuleInfo(GetModuleHandleA(ModuleName));
    const uint64_t GWorldAddressPattern = (uint64_t)Utils::FindPattern((unsigned char *)Info.Start, (unsigned char *)Info.End, Pattern);
    if (GWorldAddressPattern != 0)
    {
        const uint64_t GWorldAddressOffset = *((uint32_t *)(GWorldAddressPattern + 0x3));
        Unreal::GWorldAddress = GWorldAddressOffset + GWorldAddressPattern + 0x7;
        Unreal::GWorld = (UE4::UWorldProxy*) GWorldAddress;

        std::cout << "GWorldAddressPattern = " << std::hex << GWorldAddressPattern << std::endl;
        std::cout << "GWorldAddressOffset = " << std::hex << GWorldAddressOffset << std::endl;
        std::cout << "GWorldAddress = " << std::hex << Unreal::GWorldAddress << std::endl;
    }
    else
    {
        std::cerr << "Could not find GWorld pattern" << std::endl;
    }
}

void Unreal::FindNamePool(const char *ModuleName, const char *Pattern)
{
    FModuleInfo Info = Utils::GetModuleInfo(GetModuleHandleA(ModuleName));
    const uint64_t NamePoolPattern = (uint64_t)Utils::FindPattern((unsigned char *)Info.Start, (unsigned char *)Info.End, Pattern);
    if (NamePoolPattern != 0)
    {
        const uint64_t NamePoolOffset = *((uint32_t *)(NamePoolPattern + 0x5));
        Unreal::NamePoolAddress = NamePoolPattern + 0x9 + NamePoolOffset;

        std::cout << "NamePoolPattern = " << std::hex << NamePoolPattern << std::endl;
        std::cout << "NamePoolOffset = " << std::hex << NamePoolOffset << std::endl;
        std::cout << "NamePoolAddress = " << std::hex << Unreal::NamePoolAddress << std::endl;
        Unreal::NamePool = (UE4::FNamePool*) Unreal::NamePoolAddress;
    }
    else
    {
        std::cerr << "Could not find NamePool pattern" << std::endl;
    }
}

void Unreal::FindGUObjectArray(const char *ModuleName, const char *Pattern)
{
    FModuleInfo Info = Utils::GetModuleInfo(GetModuleHandleA(ModuleName));

    const uint64_t GUObjectArrayPattern = (uint64_t)Utils::FindPattern((unsigned char *)Info.Start, (unsigned char *)Info.End, Pattern);
    if (GUObjectArrayPattern != 0)
    {
        const uint64_t GUObjectArrayOffset = *((uint32_t *)(GUObjectArrayPattern + 0x8));
        Unreal::GUObjectArrayAddress = GUObjectArrayPattern + 0xc + GUObjectArrayOffset;
        Unreal::GUObjectArray = (UE4::FUObjectArray*)(GUObjectArrayAddress);

        std::cout << "GUObjectArrayPattern = " << std::hex << GUObjectArrayPattern << std::endl;
        std::cout << "GUObjectArrayOffset = " << std::hex << GUObjectArrayOffset << std::endl;
        std::cout << "GUObjectArrayAddress = " << std::hex << Unreal::GUObjectArrayAddress << std::endl;
        std::cout << "GUObjectArray = " << std::hex << (void*)Unreal::GUObjectArray << std::endl;
    }
    else
    {
        std::cerr << "Could not find GUObjectArray pattern" << std::endl;
    }
}