file(GLOB_RECURSE RA_SRC *.cpp)
file(GLOB_RECURSE RA_H *.h)
list(FILTER RA_SRC EXCLUDE REGEX ThirdParty/*)
list(FILTER RA_SRC EXCLUDE REGEX Main\.cpp)

file(GLOB IMGUI_SRC ThirdParty/imgui/imgui*.cpp)
list(APPEND IMGUI_SRC ThirdParty/imgui/backends/imgui_impl_dx11.cpp ThirdParty/imgui/backends/imgui_impl_win32.cpp)
#list(FILTER IMGUI_SRC EXCLUDE REGEX .+imgui_demo\.cpp)

file(GLOB_RECURSE KIERO_SRC ThirdParty/kiero/minhook/**/*.c)
list(APPEND KIERO_SRC ThirdParty/kiero/kiero.cpp)

#including RA_H to make it easier for VS 
add_library(ReverseUnreal SHARED ${RA_SRC} ${RA_H} ${KIERO_SRC} ${IMGUI_SRC} Main.cpp)

target_link_libraries(ReverseUnreal PRIVATE D3D11.dll)
target_include_directories(ReverseUnreal PRIVATE . ThirdParty/kiero ThirdParty/imgui ThirdParty/imgui/backends)
target_compile_features(ReverseUnreal PRIVATE cxx_std_17)
