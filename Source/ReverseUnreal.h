#pragma once

namespace ReverseUnreal
{
    constexpr char *GWorldPattern = "48 8B 05 ? ? ? ? 48 3B C3 48 0F 44 C6 48 89 05 ? ? ? ?";
    constexpr char *NamePoolPattern = "74 09 4C 8D 05 ? ? ? ? EB 16 48 8D 0D ? ? ? ?";
    constexpr char *GUObjectArrayPattern = "8B 52 0C 89 11 48 8D 0D ? ? ? ?";
}
