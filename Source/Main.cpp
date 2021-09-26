#include "ReverseUnrealKiero.h"
#include "Utils.h"
#include "Renderer.h"
#include "magic_enum.hpp"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "UE4/Class.h"
#include "UE4/UObject.h"
#include "UE4/NameTypes.h"
#include "UE4/String.h"

#include "Unreal.h"
#include "ReverseUnreal.h"

#include <Windows.h>
#include <iostream>
#include <string>

#include <d3d11.h>
#include "map"

using namespace UE4;

HMODULE ThisDllModule = nullptr;
typedef HRESULT(__stdcall *TPresent)(IDXGISwapChain *, UINT, UINT);

WNDPROC OriginalWndProc = nullptr;
bool bImguiInitialised = false;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

static std::string Filter;

LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
        return true;

    return CallWindowProc(OriginalWndProc, hWnd, uMsg, wParam, lParam);
}

void InitialiseImgui(IDXGISwapChain *Swapchain)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &Io = ImGui::GetIO();
    (void)Io;

    OriginalWndProc = (WNDPROC)SetWindowLongPtr(Renderer::RenderingWindow, GWLP_WNDPROC, (LONG_PTR)&WndProc);

    ImGui::StyleColorsDark();
    ImGui_ImplWin32_Init(Renderer::RenderingWindow);
    ImGui_ImplDX11_Init(Renderer::Device, Renderer::Context);

    bImguiInitialised = true;
}

struct FObjectsMap
{
    std::string Name;
    UObjectBase *Object = nullptr;
    bool bSelected = false;
    std::vector<std::string> Properties;
};

struct FPropertiesMap
{
    bool bSelected = false;
    std::string Name = "";
    FProperty* Property = nullptr;
};

HRESULT Present(IDXGISwapChain *Swapchain, UINT SyncInterval, UINT Flags)
{
    if (!Renderer::Swapchain)
    {
        Renderer::Initialise(Swapchain);
    }

    if (!bImguiInitialised)
    {
        InitialiseImgui(Swapchain);
    }

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin("ReverseUnreal", nullptr, 0))
    {
        ImGui::End();
    }
    else
    {
        if (ImGui::CollapsingHeader("Offsets"))
        {
            static char GWorldStr[24] = {0x0};
            if (GWorldStr[0] == 0x0)
                sprintf_s(GWorldStr, "%p", (void *)Unreal::GWorldAddress);
            ImGui::InputText("GWorld", GWorldStr, sizeof(GWorldStr), ImGuiInputTextFlags_ReadOnly);
            static char NamePoolStr[24] = {0x0};
            if (NamePoolStr[0] == 0x0)
                sprintf_s(NamePoolStr, "%p", (void *)Unreal::NamePoolAddress);
            ImGui::InputText("NamePool", NamePoolStr, sizeof(NamePoolStr), ImGuiInputTextFlags_ReadOnly);
            static char GUObjectArrayStr[24] = {0x0};
            if (GUObjectArrayStr[0] == 0x0)
                sprintf_s(GUObjectArrayStr, "%p", (void *)Unreal::GUObjectArrayAddress);
            ImGui::InputText("GUObjectArray", GUObjectArrayStr, sizeof(GUObjectArrayStr), ImGuiInputTextFlags_ReadOnly);
        }
        static int CurrentObjectIndex = 0;

        if (ImGui::CollapsingHeader("Dumper"))
        {
            static ImGuiTextFilter filter = ImGuiTextFilter("default__pawn");
            filter.Draw();
            ImGui::BeginListBox("Objects"); // open a framed scrolling region
            static std::map<int, FObjectsMap> CachedResolvedNames;

            static FObjectsMap *SelectedObject = nullptr;
            static FPropertiesMap* SelectedProperty = nullptr;
            //00007FF6904A1E58
            for (int i = 0; i < Unreal::GUObjectArray->ObjObjects.NumElements; i++)
            {
                std::string *Name = nullptr;
                if (CachedResolvedNames.count(i) != 0)
                {
                    Name = &CachedResolvedNames[i].Name;
                }
                else if (auto Obj = Unreal::GUObjectArray->ObjObjects.GetObjectPtr(i))
                {
                    if (Obj->Object)
                    {
                        const FNameEntry &NameEntry = Obj->Object->NamePrivate.GetEntry();
                        std::string TmpName = std::string(NameEntry.AnsiName, NameEntry.GetNameLength());
                        if (Obj->Object->ClassPrivate)
                        {
                            const FNameEntry &ClassNameEntry = Obj->Object->ClassPrivate->NamePrivate.GetEntry();
                            TmpName = TmpName + " (" + std::string(ClassNameEntry.AnsiName, ClassNameEntry.GetNameLength()) + ")";
                        }
                        CachedResolvedNames.insert({i, {TmpName, Obj->Object, false}});
                        Name = &CachedResolvedNames[i].Name;
                    }
                }

                if (Name != nullptr)
                {
                    if (filter.PassFilter(Name->c_str()))
                    {
                        bool bSelected = CachedResolvedNames[i].bSelected;

                        ImGui::Selectable(Name->c_str(), &bSelected);

                        if (bSelected != CachedResolvedNames[i].bSelected)
                        {
                            if (SelectedObject)
                            {
                                SelectedObject->bSelected = false;
                            }

                            SelectedProperty = nullptr;

                            CachedResolvedNames[i].bSelected = bSelected;
                            SelectedObject = bSelected ? &CachedResolvedNames[i] : nullptr;
                        }
                    }
                }
            }

            ImGui::EndListBox();
            //PropertyLink
            if (SelectedObject && SelectedObject->Object)
            {
                static char ObjectAddressBuff[24] = { 0x0 };
                sprintf_s(ObjectAddressBuff, "%p", (void*)SelectedObject->Object);
                ImGui::InputText("Address", ObjectAddressBuff, sizeof(ObjectAddressBuff), ImGuiInputTextFlags_ReadOnly);
                static std::map<FProperty *, FPropertiesMap> CachedResolvedProperties;
                static ImGuiTextFilter PropertiesFilter;
                PropertiesFilter.Draw("Properties Filter");
                ImGui::BeginListBox("Object Properties");
                if (UClass *Class = SelectedObject->Object->ClassPrivate)
                {
                    FProperty *Property = Class->PropertyLink;
                    while (Property != nullptr)
                    {
                        auto CachedProperty = CachedResolvedProperties.find(Property);
                        if (CachedProperty == CachedResolvedProperties.end())
                        {
                            const auto [It, Success] = CachedResolvedProperties.insert({Property, FPropertiesMap{false, "", Property}});
                            if (Success)
                            {
                                std::string Str = std::string(Property->NamePrivate.GetAnsi(), Property->NamePrivate.GetEntry().GetNameLength());
                                std::string Offset = "(" + std::to_string(Property->Offset_Internal) + ")";
                                Str += " " + Offset;
                                if (Property->ClassPrivate)
                                {
                                    const FNameEntry &ClassNameEntry = Property->ClassPrivate->Name.GetEntry();
                                    Str += " (" + std::string(ClassNameEntry.AnsiName, ClassNameEntry.GetNameLength()) + ")";
                                }
                                It->second.Name = Str;
                                CachedProperty = It;
                            }
                        }

                        if (CachedProperty != CachedResolvedProperties.end() && PropertiesFilter.PassFilter(CachedProperty->second.Name.c_str()))
                        {
                            bool bSelected = CachedProperty->second.bSelected;
                            ImGui::Selectable(CachedProperty->second.Name.c_str(), &bSelected);

                            if (bSelected != CachedProperty->second.bSelected)
                            {
                                if (SelectedProperty)
                                {
                                    SelectedProperty->bSelected = false;
                                }

                                CachedProperty->second.bSelected = bSelected;
                                SelectedProperty = CachedProperty->second.bSelected ? &CachedProperty->second : nullptr;
                            }
                        }
                        Property = Property->PropertyLinkNext;
                    }
                }

                ImGui::EndListBox();

                if (SelectedProperty && SelectedProperty->Property && SelectedProperty->Property->ClassPrivate)
                {
                    const FFieldClass* Class = SelectedProperty->Property->ClassPrivate;
                    const FNameEntry& ClassName = Class->Name.GetEntry();
                    ImGui::InputText("Property Type", (char*)ClassName.AnsiName, ClassName.GetNameLength(), ImGuiInputTextFlags_ReadOnly);
                    const EClassCastFlags ClassCastFlags = (EClassCastFlags) SelectedProperty->Property->ClassPrivate->CastFlags;
                    
                    void* ValuePointer = (void*)((uint64_t)SelectedObject->Object + SelectedProperty->Property->Offset_Internal);
                    
                    static char AddressBuff[24] = { 0x0 };
                    sprintf_s(AddressBuff, "%p", ValuePointer);
                    ImGui::InputText("Address", AddressBuff, sizeof(AddressBuff), ImGuiInputTextFlags_ReadOnly);

                    if (ClassCastFlags & EClassCastFlags::CASTCLASS_FBoolProperty)
                    {
                        FBoolProperty* BoolProp = static_cast<FBoolProperty*>(SelectedProperty->Property);
                        bool ActualValue = *((uint8*)ValuePointer) & BoolProp->ByteMask;
                        ImGui::Checkbox("Value", &ActualValue);

                        if (ActualValue != (bool)(*((uint8*)ValuePointer) & BoolProp->ByteMask))
                        {

                            *((uint8*)ValuePointer) ^= BoolProp->ByteMask;
                        }
                    } 
                    else if (ClassCastFlags & EClassCastFlags::CASTCLASS_FFloatProperty)
                    {
                        ImGui::InputFloat("Value", (float*)ValuePointer);
                    }
                    else if (ClassCastFlags & EClassCastFlags::CASTCLASS_FUInt32Property)
                    {
                        ImGui::InputInt("Value", (int32*)ValuePointer);
                    }
                    else if (ClassCastFlags & EClassCastFlags::CASTCLASS_FByteProperty)
                    {
                        ImGui::InputInt("Value", (int32*)ValuePointer);
                    }
                    else if (ClassCastFlags & EClassCastFlags::CASTCLASS_FStrProperty)
                    {
                        FString* String = (FString*) ValuePointer;
                        if (String->Data.Num() > 0)
                        {
                            ImGui::InputText("Value", (char*)String->Data.GetData(), String->Data.Num(), ImGuiInputTextFlags_ReadOnly);
                        }
                    }
                }
            }
        }
        ImGui::End();
    }

    static bool show_demo_window = false;

    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);

    ImGui::Render();

    Renderer::Present();

    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    return Renderer::OriginalPresentAddress(Swapchain, SyncInterval, Flags);
}

void HookDx11()
{
    kiero::Status::Enum Status = kiero::init(kiero::RenderType::D3D11);
    if (Status == kiero::Status::Success)
    {
        kiero::bind(Renderer::Dx11PresentIndex, (void **)&Renderer::OriginalPresentAddress, &Present);
    }
    else
    {
        std::cerr << "There was an error while initialising kiero. " << (int)Status << std::endl;
    }
}

void ReverseUnrealThread()
{
    AllocConsole();
    freopen_s((FILE **)stdin, "CONIN$", "r", stdin);
    freopen_s((FILE **)stderr, "CONOUT$", "w", stderr);
    freopen_s((FILE **)stdout, "CONOUT$", "w", stdout);
    std::cout << "Module: " << Utils::GetModulePath(ThisDllModule) << std::endl;
    Utils::PrintModules();
    Filter.reserve(255);

    std::string CurrentProcessExeModuleName = Utils::GetProcessExeModule();

    if (CurrentProcessExeModuleName.find("Shipping") != std::string::npos)
    {
        Unreal::bIsProd = true;
    }

    std::cout << "Current Process Exe: " << CurrentProcessExeModuleName << std::endl;

    Unreal::FindGUObjectArray(CurrentProcessExeModuleName.c_str(), ReverseUnreal::GUObjectArrayPattern);
    Unreal::FindNamePool(CurrentProcessExeModuleName.c_str(), ReverseUnreal::NamePoolPattern);
    Unreal::FindGWorld(CurrentProcessExeModuleName.c_str(), ReverseUnreal::GWorldPattern);

    HookDx11();
}

void Cleanup()
{
    std::cout << "Cleanup request. Quitting." << std::endl;
    SetWindowLongPtr(Renderer::RenderingWindow, GWLP_WNDPROC, (LONG_PTR)OriginalWndProc);
    kiero::unbind(Renderer::Dx11PresentIndex);
    FreeConsole();
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
{
    ThisDllModule = hinstDLL;
    DisableThreadLibraryCalls(hinstDLL);

    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
    {
        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ReverseUnrealThread, NULL, 0, NULL);
        break;
    }
    case DLL_THREAD_ATTACH:
        // Do thread-specific initialization.
        break;

    case DLL_THREAD_DETACH:
        // Do thread-specific cleanup.
        break;

    case DLL_PROCESS_DETACH:
    {
        Cleanup();
        break;
    }
    }
    return TRUE; // Successful DLL_PROCESS_ATTACH.
}
