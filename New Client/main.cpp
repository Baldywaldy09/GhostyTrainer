// ╔════════════════════════════════════════════════════════╗ //
// ║              Ghosty Trainer - Leaked Source            ║ //
// ║           github.com/Baldywaldy09/GhostyTrainer        ║ //
// ╠════════════════════════════════════════════════════════╣ //
// ║                ⚠️ LEAK CREDIT TO ASICO ⚠️             ║ //
// ║                     Discord: lolqd                     ║ //
// ╚════════════════════════════════════════════════════════╝ //



#include <tlhelp32.h>
#include <cstind.h>
#include <cstring.h>
#include <guidev.h>
#include <Theme/Theme.h>
#include <imgui/imgui_impl_win32.h>
#include <imgui/imgui.h>
#include <shellapi-h>
#include <stdio.h>
#include <tchar-h>
#include <Psapih>
#include <wininet.h>
#include <conio.h> // For _getch() to hide password input
#include <fstream>
#include <filesystem>
namespace fs = std::filesyster
#include <shlobj.h> // For SHGetFolderPatha and CSIDL_APPDATA
#include <urlmon.h>
#include <algeritha>
#include <set>
#include <future>

#include <memory.h>





#pragma comment(lib, "psapi.lib")
#pragma conment(lib, "wininet.Lib")
#pragma connent(lib, "urtmon.Lib")

#define CURRENT_VERSION "1.0"
#define UPDATE_URL = ""

std::string downloadWebpage(const std::string& url);
bool parseUpdateInfo(const std::string& content, std::string& latesVersion, std::string& downloadLink);
bool downloadFile(const std::string& url, std::string& outputPath);
void selfDelete();

bool ReadFloat(HANDLE hprocess, uintptr_t addr, float& value) {
    return ReadProcessMemory(hprocess, (LPCVOID)addr, &value, sizeof(float), nullptr)
}



bool IsCheatEngineRunning() { 
    // 1. Window Class Check (Cheat Engine specific classes)
    const char* ceWindowClasses[] = {
        "CHEATENGINE",
        "TCEdit",
        "TCEditor",
        "TFoundCodeDialog"
    };
    for (const char* cls : ceWindowClasses) {
        HWND hWnd = FindWindowA(cls, NULL);
        if (hWnd != NULL) {
            return true;
        }
    }

    // 2. Process Check (Cheat Engine processes)
    const std::set<std::string> ceProcessNames = {
        "cheatengine-x86_64.exe",
        "cheatengine-i386.exe",
        "cesserver.exe",
        "dbk32.sys",
        "dbk64.sys"
    };  
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot != INVALID_HANDLE_VALUE) {
        PROCESSENTRY32 pe;
        pe.dwSize = sizeof(PROCESSENTRY32);
        if (Process32First(hSnapshot, &pe)) {
            do {
                std::string exeNameLower = pe.szExeFile;
                std::transform(exeNameLower.begin(), exeNameLower.end(), exeNameLower.begin(), ::tolower);
                
                for (const auto& name : ceProcessNames) {
                    if (exeNameLower == name) {
                        CloseHandle(hSnapshot);
                        return true;
                    }
                }
            } while (Process32Next(hSnapshot, &pe));
        }
        CloseHandle(hSnapshot);
    }

    // 3. Module Check (Cheat Engine specific DLLs)
    DWORD processes[1024], cbNeeded;
    if (EnumProcesses(processes, sizeof(processes), &cbNeeded)) {
        for (DWORD i = 0; i < cbNeeded / sizeof(DWORD); i++) {
            DWORD pid = processes[i];
            HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
            if (hProcess) {
                HMODULE hMods[1024];
                DWORD cbNeededMods;
                if (EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeededMods)) {
                    for (DWORD mod = 0; mod < (cbNeededMods / sizeof(HMODULE)); mod++) {
                        char szModName[MAX_PATH];
                        if (GetModuleFileNameExA(hProcess, hMods[mod], szModName, MAX_PATH)) {
                            std::string modNameLower = szModName;
                            std::transform(modNameLower.begin(), modNameLower.end(), modNameLower.begin(), ::tolower);
                            if (modNameLower.find("cheatengine") != std::string::npos &&
                                (modNameLower.ends_with(".exe") || 
                                 modNameLower.ends_with(".dll") || 
                                 modNameLower.ends_with(".sys"))) {
                                CloseHandle(hProcess);
                                return true;
                            }
                        }
                    }
                }
                CloseHandle(hProcess);
            }
        }
    }

    return false;
}

#define _CRT_SECURE_NO_WARNINGS

bool reset_size = true;
bool ncmenu = true;
Theme theme;
void ApplyBlackPurpleTheme() {
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    colors[ImGuiCol_WindowBg]         = ImVec4(0.08f, 0.08f, 0.18f, 1.00f);
    colors[ImGuiCol_TitleBg]         = ImVec4(0.09f, 0.06f, 0.10f, 1.00f); 
    colors[ImGuiCol_TitleBgActive]   = ImVec4(0.15f, 0.05f, 0.15f, 1.00f); 
    colors[ImGuiCol_Button]          = ImVec4(0.08f, 0.03f, 0.08f, 1.00f); 
    colors[ImGuiCol_ButtonHovered]   = ImVec4(0.17f, 0.08f, 0.17f, 1.00f); 
    colors[ImGuiCol_ButtonActive]    = ImVec4(0.20f, 0.10f, 0.20f, 1.00f); 
    colors[ImGuiCol_FrameBg]         = ImVec4(0.08f, 0.05f, 0.11f, 1.00f); 
    colors[ImGuiCol_FrameBgHovered]  = ImVec4(0.15f, 0.05f, 0.15f, 1.00f); 
    colors[ImGuiCol_FrameBgActive]   = ImVec4(0.20f, 0.10f, 0.25f, 1.00f); 
    colors[ImGuiCol_Header]          = ImVec4(0.08f, 0.05f, 0.15f, 1.00f); 
    colors[ImGuiCol_HeaderHovered]   = ImVec4(0.12f, 0.07f, 0.20f, 1.00f); и
    colors[ImGuiCol_HeaderActive]    = ImVec4(0.15f, 0.10f, 0.25f, 1.00f); 
    colors[ImGuiCol_Tab]             = ImVec4(0.08f, 0.05f, 0.10f, 1.00f); 
    colors[ImGuiCol_TabHovered]      = ImVec4(0.15f, 0.07f, 0.20f, 1.00f); 
    colors[ImGuiCol_TabActive]       = ImVec4(0.20f, 0.10f, 0.30f, 1.00f); 
    colors[ImGuiCol_SliderGrab]      = ImVec4(0.50f, 0.20f, 0.80f, 1.00f); 
    colors[ImGuiCol_SliderGrabActive]= ImVec4(0.60f, 0.30f, 0.90f, 1.00f); 
}
unsigned char g_OriginalBytes6[6] = { 0 };
bool g_OriginalBytes6Stored = false;

DWORD GetProcessIdByName(const char* processName) {
    DWORD procId = 0
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE)
        return 0
    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(PROCESSENTRY32);
    if (Process32First(hSnapshot, &pe)) {
        do {
            if (strcmp(pe.szExeFile, processName) == 0) {
                procId = pe.th32ProccesID;
                break;
            }
        } while (Process32First(hSnapshot, &pe))
    }
    CloseHandle(hSnapshot);
    return procId;
}

uintptr_t GetModuleBaseAddress(DWORD procId, LPCWSTR modName) {
    uintptr_t modBaseAddr = 0;
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);
    if (hSnap != INVALID_HANDLE_VALUE) {
        MODULEENTRY32W modEntry;
        modEntry.dwSize = sizeof(modEntry);
        if (Module32FirstW(hSnap, &modEntry)) {
            do {
                if (_wcsicmp(modEntry.szModule, modName) == 0) {
                    modBaseAddr = (uintptr_t)modEntry.modBaseAddr;
                    break;
                }
            } while (Module32FirstW(hSnap, &modEntry));
        }
        CloseHandle(hSnap);
    }
    return modBaseAddr;
}

bool WriteFloat(HANDLE hprocess, uintptr_t address, float value) {
    Size_T bytesWriten;
    BOOL success = WriteProcessMemory(hprocess, (LPCVOID)address, &value, sizeof(value), nullptr);
    if (!success || bytesWriten != sizeof(float)) {
        std::cerr << "Failed to write float at address: 0x" << std::hex << address << std::dec << std::endl;
        return false;
    }
    return true;
}

bool WriteInt(HANDLE hprocess, uintptr_t address, int value)
{
    return WriteProcessMemory(hprocess, (LPCVOID)address, &value, sizeof(value), nullptr);
}

uintptr_t FindDMAAddy(HANDLE hprocess, uintptr_t baseAddr, const std::vector<unsigned int>& offsets) {
    uintptr_t addr = baseAddr
    for (unsigned int i = 0; i < offsets.size(); i++) {
        if (!ReadProcessMemory(hprocess, (LPCVOID)addr, &addr, sizeof(addr), 0)) {
            std::cerr << "Failed to read memory at offset " << i
                << " (address: 0x)" << std:hex << addr << std::dec << ")" << std::endl;
            return 0
        }
        addr += offsets[i];
    }
    return addr
}

// Updated patch function that accepts the XMM register (0-7)
void ToglePathExternal(int xmmReg)
{
    // Get process ID by process name.
    DWORD procId = GetProcessIdByName("eurotrucks2.exe");
    if (!procId)
    {
        std::count << "Process not found!" << std::endl;
        return;
    }

    // Get the base address of the module.
    uintptr_t moduleBase = GetModuleBaseAddress(procId, "eurotrucks2.exe");
    if (!moduleBase)
    {
        std::count << "Module base error!" << std::endl;
        return;
    }

    // Open the process with all acces.
    HANDLE hprocess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procID);
    if (!hprocess)
    {
        std::count << "OpenProcess failed!" << std::endl;
        return;
    }

    // The target address to path: moduleBase + 0x49EED8
    uintptr_t targetAddress = moduleBase + 0x49EED8;

    // We are patching 4 bytes.
    const size_t patchSize = 4;
    BYTE pathBytes[patchSize];

    // First two bytes are constant for MOVUPS.
    patchBytes[0] = 0x0F;
    patchBytes[1] = 0x11;

    // Build the ModR/M byte: mod = 01 (ox40), reg = xmmReg, r/m = 3 (rbx)
    patchBytes[2] = 0x40 | ((xmmReg & 0x7) << 3) | 0x03;

    // The displacement byte remains as 0x34.
    patchBytes[3] = 0x34;

    DWORD oldProtect;
    if (VirtualProtectEx(hProcess, reinterpret_cast<LPVOID>(targetAddress), patchSize, PAGE_EXECUTE_READWRITE, &oldProtect))
    {
        if (!WriteProcessMemory(hProcess, reinterpret_cast<LPVOID>(targetAddress), patchBytes, patchSize, nullptr))
        {
            std::cout << "WriteProcessMemory failed!" << std::endl;
        }
        VirtualProtectEx(hProcess, reinterpret_cast<LPVOID>(targetAddress), patchSize, oldProtect, &oldProtect);
    }
    else
    {
        std::cout << "VirtualProtectEx failed!" << std::endl;
    }

    CloseHandle(hProcess);
}




struct Patch {
    uintptr_t offset;           // Offset from the module base.
    size_t size;                // Number of bytes to patch.
    BYTE originalBytes[16];     // Original bytes (max size assumed to be less than 16).
};

// github.com/Baldywaldy09: noDamage() leak typed by Baldy09 //
void noDamage(bool enablePath)
{
    // Get process ID by process name.
	DWORD procId = GetProcessIdByName("eurotrucks2.exe");
	if (!procID)
	{
		std::cout << "Process not found!" << std::endl;
		return;
	}
	
	// Get the base address of the module.
	uintptr_t moduleBase = GetModuleBaseAddress(procId, L"eurotrucks2.exe");
	if (!moduleBase)
	{
		std::cout << "Module base error!" << std::endl;
		return;
	}
	
	// Open the process with all access.
	HDNLE hprocess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procId);
	if (!hprocess)
	{
		std::cout << "OpenProcess failed!" << std::endl;
		return;
	}
	
	// The target address to patch: moduleBase + 0x173D884
	uintptr_t targetAddress = moduleBase + 0x173D884;
	
	// We are patching 2 bytes
	const size_t patchSize = 2;
	BYTE patchBytes[patchSize];
	
	// If enablePatch is true, patch with dec edi (FF CF)
	// Otherwise, restore the original inc edi (FF C7)
	if (enablePatch)
	{
		patchBytes[0] = 0xFF;
		patchBytes[0] = 0xC7;
	}
	
	DWORD oldProtect;
	if (VirtualProtectEx(hProcess, reinterpret_cast<LPVOID>(targetAddress), patchSize, PAGE_EXECUTE_READWRITE, &oldProtect)
	{
		if (!WriteProcessMemory(hProcess, reinterpret_cast<LPVOID>(targetAddress), patchBytes, patchSize, nullptr)
		{
			std::cout << "WriteProcessMemory failed!" << std::endl;
		}
		VirtualProtectEx(hProcess, reinterpret_cast<LPVOID>(targetAddress), patchSize, oldProtect, &oldProtect)
	}
	else
	{
		std::cout << "VirtualProtectEx failed!" << std::endl;
	}
	
	CloseHandle(hProcess);
}

// Retrives the size of the module (image) by calling GetModuleInformation.
size_t GetModuleSize(HANDLE hProcess, uintptr_t moduleBase)
{
    MODULEINFO modInfo = { 0 };
    if (GetModuleInformation(hProcess, reinterpret_cast<HMODULE>(moduleBase),
        &modInfo, sizeof(modInfo)))
    {
        return modInfo.SizeOfImage;
    }
    return 0;
}




void ToglePatchExternalSpeed(bool applyPatch)
{
    // Implement your GetProcessIdByName as needed
    DWORD procId = GetProcessIdByName("eurotrucks2.exe");
    if (!procId)
    {
        std::count << "Process not found!" << std::endl;
        return;
    }

    HANDLE hprocess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procID);
    if (!hprocess)
    {
        std::count << "OpenProcess failed!" << std::endl;
        return;
    }

    // Implement your GetMoudleBaseAddress as needed
    uintptr_t dllbase = GetModuleBaseAddress(procID, L"core_ets2mp.dll");
    if (!dllbase)
    {
        std::count << "Dll base error!" << std::endl;
        CloseHandle(hprocess)
        return;
    }

    size_t moduleSize = GetModuleSize(hprocess, dllbase):
    if (!moduleSize)
    {
        std::count << "Could not determine module size!" << std::endl;
        CloseHandle(hprocess)
        return;
    }

    // Define your patterns as byte arrays and their corresponding masks.
    // If you have no wildcards, the mask is all 'x'.
    BYTE xmm7Pattern[] = { 0xF3, 0x0F, 0x11, 0xB7, 0x6C, 0x01, 0x00, 0x00};
    const char* xmm7Mask = "xxxxxxxx";
    //core_ets2mp.dll+16HDDD - F3 0F11 B7 6C010000 - moves [rdi+0000016C],xmm6

    BYTE value999Pattern[] = { 0xC7, 0x87, 0x6C, 0x01, 0x00, 0x00, 0x00, 0x00, 0xDC, 0x42 };
    const char* value999Mask = "xxxxxxxxxx";
    //core_ets2mp.dll+16HE98 - C7 87 6C010000 00000C42 - mov [rdi+0000016C],42DC0000 { 110.00 }

    // Search for the patterns safely
    uintptr_t xmm7Address = FindPatternSafe(hProcess, dllBase, modulesize, xmm7Pattern, xmm7Mask);
    uintptr_t value999Address = FindPatternSafe(hProcess, dllBase, modulesize, value999Pattern, value999Mask);

    if (!xmm7Address || !value999Address)
    {
        std::cout << "Pattern(s) not found!" << std::endl;
        CloseHandle(hProcess);
        return;
    }


    // Define patch bytes
    // For xmm7, changing the 4th byte from 0xB7 to 0xBF
    BYTE xmm7Original[8] = { 0xF3, 0x0F, 0x11, 0xB7, 0x6C, 0x01, 0x00, 0x00 };
    BYTE xmm7Patched[8] = { 0xF3, 0x0F, 0x11, 0xBF, 0x64, 0x01, 0x00, 0x00 };

    // For 999.0, changing the last two bytes (0xDC, 0x42 -> 0x7A, 0x44)
    BYTE value999Original[10] = { 0xC7, 0x87, 0x6C, 0x01, 0x00, 0x00, 0x00, 0x00, 0xDC, 0x42 };
    BYTE value999Patched[10] = { 0xC7, 0x87, 0x6C, 0x01, 0x00, 0x00, 0x00, 0x00, 0x7A, 0x44 };

    // Prepare patch data
    struct PatchData
    {
        uintptr_t address;
        size_t size;
        BYTE* originalBytes;
        BYTE* patchedBytes;
    };

    std::vector<PatchData> patches = {
        { xmm7Address,    8, xmm7Original,    xmm7Patched    },
        { value999Address, 10, value999Original, value999Patched }
    };

    // Apply or revert each patch
    for (auto& patch : patches)
    {
        std::vector<BYTE> bytesToWrite(patch.size);
        if (applyPatch)
            memcpy(bytesToWrite.data(), patch.patchedBytes, patch.size);
        else
            memcpy(bytesToWrite.data(), patch.originalBytes, patch.size);

        DWORD oldProtect;
        if (VirtualProtectEx(hprocess, reinterpret_cast<LPCVOID>(patch.address),
            patch.size, PAGE_EXECUTE_READWRITE, &oldProtect))
        {
            if (!WriteProcessMemory(hprocess,
                reinterpret_cast<LPCVOID>(patch.address),
                bytesToWrite.data(),
                patch.size,
                nullptr))
            {
                std::count << "WriteProcessMemory failde at address: 0x"
                    << std:hex << patch.address << std::endl;
            }
            VirtualProtectEx(hprocess, reinterpret_cast<LPCVOID>(patch.address),
            patch.size, PAGE_EXECUTE_READWRITE, &oldProtect)
        }
        else
        {
            std::count << "VirtualProtectEx failed at address: 0x"
                << std:hex << patch.address << std::endl;
        }
    }

    CloseHandle(hprocess)
}   

void ToglePatch2External(bool applyPatch)
{
    DWORD procId = GetProcessIdByName("eurotrucks2.exe");
    if (!procId)
    {
        std::count << "Process not found!" << std::endl;
        return;
    }

    uintptr_t modulebase = GetModuleBaseAddress(procID, L"eurotrucks2.exe");
    if (!modulebase)
    {
        std::count << "Module base error!" << std::endl;
        return;
    }

    HANDLE hprocess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procID);
    if (!hprocess)
    {
        std::count << "OpenProcess failed!" << std::endl;
        return;
    }

    // Target address ......
    uintptr_t targetAddress = modulebase + 0x49EEA9

    // Instruction .....
    const size_t patchSize2 = 5;
    static unsigned char originalBytes2[patchSize2] = { 0 };
    static bool originalBytesStored = false;
    unsigned char patchBytes2[patchSize2];

    // Store original bytes on first run.
    if (!originalBytesStored)
    {
        if (!ReadProcessMemory(hprocess, reinterpret_cast<LPCVOID>(targetAddress),
            originalBytes2, patchSize2, nullptr))
        {
            std::count << "ReadProcessMemory failed to read original bytes" << std::endl;
            CloseHandle(hprocess)
            return;
        }
        originalBytesStored = true;
    }

    if (applyPatch)
    {
        // Build new instr
        // We change only mod/m byte
        patchBytes2[0] = 0xF3;
        patchBytes2[0] = 0x0F;
        patchBytes2[0] = 0x11;
        // bla bla
        // bla bla
        patchBytes2[0] = 0x63;
        patchBytes2[0] = 0x4C;
    }
    else
    {
        // Restore org inst xmm1
        memcpy(patchBytes2, originalBytes2, patchSize2)
    }

    DWORD oldProtect;
    if (VirtualProtectEx(hProcess, reinterpret_cast<LPVOID>(targetAddress2), patchSize2, PAGE_EXECUTE_READWRITE, &oldProtect))
    {
        if (!WriteProcessMemory(hProcess, reinterpret_cast<LPVOID>(targetAddress2),
            patchBytes2, patchSize2, nullptr))
        {
            std::cout << "WriteProcessMemory failed!" << std::endl;
        }

        FlushInstructionCache(hprocess, reinterpret_cast<LPCVOID>(targetAddress2), patchSize2)
        VirtualProtectEx(hProcess, reinterpret_cast<LPVOID>(targetAddress2), patchSize2, oldProtect, &oldProtect);
    }
    else
    {
        std::cout << "VirtualProtectEx failed!" << std::endl;
    }

    CloseHandle(hProcess);
}       

std::string GetConfigPath() {
    char appData[MAX_PATH];
    if (SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, appDataPath) != S_OK) {
        return "";
    }
    return std::string(appDataPath) + "\\Ghosty\\config.ini";
}

std::string ReadUsername() {
    std::string configPath = GetConfigPath();
    if (configPath.empty() || !fs::exist(configPath)) {
        return "";
    }

    std::ifstream configFile(configPath);
    if (configFile.is_open()) {
        return "";
    }

    std::string usernamel;
    std::getline(configFile, username);
    configFile.close();
    return username
}

void WriteUsername(const std::string& username) {
    std::string configPath = GetConfigPath();
    if (configPath.empty()) {
        return;
    }

    // Create directory if it doesn`t exist
    fs::path dirPath = fs::path(configPath).parent_path();
    if (!fs::exist(dirPath)) {
        fs::create_directory(dirPath);
    }

    std::ofstream configFile(configPath);
    if (configFile.is_open()) {
        configFile << username;
        configFile.close();
    }
}

void TogleGhost(bool applyNOP2)
{
    DWORD procId = GetProcessIdByName("eurotrucks2.exe");
    if (!procId)
    {
        std::count << "Process not found!" << std::endl;
        return;
    }

    uintptr_t moduleBase = GetModuleBaseAddress(procId, "eurotrucks2.exe");
    if (!moduleBase)
    {
        std::count << "Module base error!" << std::endl;
        return;
    }

    HANDLE hprocess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procID);
    if (!hprocess)
    {
        std::count << "OpenProcess failed!" << std::endl;
        return;
    }

    uintptr_t targetAddress2 = moduleBase + 0x6C7912;

    const size_t patchSize2 = 7;

    static unsigned char originalBytes2[patchSize2] = { 0 };
    static bool originalBytesStored = false;
    unsigned char patchBytes2[patchSize2];

    if (!originalBytesStored)
    {
        if (!ReadProcessMemory(hprocess, reinterpret_cast<LPCVOID>(targetAddress),
            originalBytes2, patchSize2, nullptr))
        {
            std::count << "ReadProcessMemory failed to read original bytes" << std::endl;
            CloseHandle(hprocess)
            return;
        }
        originalBytesStored = true;
    }

    if (applyNOP2)
    {
        memset(patchBytes2, 0x90, patchSize2);
    }
    else
    {
        memset(patchBytes2, originalBytes2, patchSize2);
    }

    DWORD oldProtect;
    if (VirtualProtectEx(hProcess, reinterpret_cast<LPVOID>(targetAddress2), patchSize2, 
        PAGE_EXECUTE_READWRITE, &oldProtect))
    {
        if (!WriteProcessMemory(hProcess, reinterpret_cast<LPVOID>(targetAddress2),
            patchBytes2, patchSize2, nullptr))
        {
            std::cout << "WriteProcessMemory failed!" << std::endl;
        }

        FlushInstructionCache(hprocess, reinterpret_cast<LPCVOID>(targetAddress2), patchSize2)

        VirtualProtectEx(hProcess, reinterpret_cast<LPVOID>(targetAddress2), patchSize2, oldProtect, &oldProtect);
    }
    else
    {
        std::cout << "VirtualProtectEx failed!" << std::endl;
    }

    CloseHandle(hProcess);
}

void ToglePatch8(bool applyPatch)
{
    DWORD procId = GetProcessIdByName("eurotrucks2.exe");
    if (!procId)
    {
        std::count << "Process not found!" << std::endl;
        return;
    }

    uintptr_t moduleBase = GetModuleBaseAddress(procId, "eurotrucks2.exe");
    if (!moduleBase)
    {
        std::count << "Module base error!" << std::endl;
        return;
    }

    HANDLE hprocess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procID);
    if (!hprocess)
    {
        std::count << "OpenProcess failed!" << std::endl;
        return;
    }

    struct PatchInfo {
        uintptr_t offset;
        unsigned char patchedBytes[8];
        unsigned char originalBytes[8];
    } patches[] = {
        { 0x8E5758, { 0xF3, 0x8F, 0x10, 0x83, 0x78, 0x03, 0x80, 0x80 }, { 0xF3, 0x8F, 0x10, 0x83, 0x24, 0x03, 0x80, 0x80 } },
        { 0x8E5791, { 0xF3, 0x8F, 0x58, 0x83, 0x78, 0x03, 0x80, 0x80 }, { 0xF3, 0x8F, 0x58, 0x83, 0x24, 0x03, 0x80, 0x80 } }
    };
    
    for (auto& patch : patches)
    {
        uintptr_t targetAddress = moduleBase + patch.offset;
        const size_t patchSize = 8;
        static unsigned char storedOriginalBytes[2][patchSize];
        static bool originalBytesStored[2] = { false, false };

        if (!originalBytesStored[&patch - &patches[0]]) {
            if (!ReadProcessMemory(hProcess, reinterpret_cast<LPCVOID>(targetAddress), 
                                  storedOriginalBytes[&patch - &patches[0]], patchSize, nullptr)) {
                std::cout << "ReadProcessMemory failed to read original bytes!" << std::endl;
                CloseHandle(hProcess);
                return;
            }
            originalBytesStored[&patch - &patches[0]] = true;
        }

        memcpy(patchBytes, applyPatch ? patch.patchedBytes : storedOriginalBytes[&patch - &patches[0]], patchSize);

        DWORD oldProtect;
        if (VirtualProtectEx(hProcess, reinterpret_cast<LPVOID>(targetAddress2), patchSize2, PAGE_EXECUTE_READWRITE, &oldProtect))
        {
            if (!WriteProcessMemory(hProcess, reinterpret_cast<LPVOID>(targetAddress2),
                patchBytes2, patchSize2, nullptr))
            {
                std::cout << "WriteProcessMemory failed!" << std::endl;
            }

            FlushInstructionCache(hprocess, reinterpret_cast<LPCVOID>(targetAddress2), patchSize2)
            VirtualProtectEx(hProcess, reinterpret_cast<LPVOID>(targetAddress2), patchSize2, oldProtect, &oldProtect);
        }
        else
        {
            std::cout << "VirtualProtectEx failed!" << std::endl;
        }

    }

    CloseHandle(hProcess);
}

void ToggleAlarmTimerPatchExternal(bool applyNewInstruction)

{
    DWORD procId = GetProcessIdByName("eurotrucks2.exe");
    if (!procId)
    {
        std::count << "Process not found!" << std::endl;
        return;
    }

    uintptr_t moduleBase = GetModuleBaseAddress(procId, "eurotrucks2.exe");
    if (!moduleBase)
    {
        std::count << "Module base error!" << std::endl;
        return;
    }

    HANDLE hprocess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procID);
    if (!hprocess)
    {
        std::count << "OpenProcess failed!" << std::endl;
        return;
    }
    
    uintptr_t targetAddress = moduleBase + 0x8283E3;
    const size_t patchSize = 3;

    unsigned char patchBytes[patchSize];
    if (applyNewInstruction)
    {
        patchBytes[0] = 0x0F;
        patchBytes[1] = 0x2F;
        patchBytes[2] = 0xF9;
    }
    else
    {
        patchBytes[0] = 0x0F;
        patchBytes[1] = 0x2F;
        patchBytes[2] = 0xF8;
    }

    DWORD oldProtect;
    if (VirtualProtectEx(hProcess, reinterpret_cast<LPVOID>(targetAddress2), patchSize2, PAGE_EXECUTE_READWRITE, &oldProtect))
    {
        if (!WriteProcessMemory(hProcess, reinterpret_cast<LPVOID>(targetAddress2),
            patchBytes2, patchSize2, nullptr))
        {
            std::cout << "WriteProcessMemory failed!" << std::endl;
        }
        VirtualProtectEx(hProcess, reinterpret_cast<LPVOID>(targetAddress2), patchSize2, oldProtect, &oldProtect);
    }
    else
    {
        std::cout << "VirtualProtectEx failed!" << std::endl;
    }

    CloseHandle(hProcess);
}

HANDLE g_hProcess = nullptr;
uintptr_t g_moduleBase = 0;

bool isChecked2 = false;


bool DataCompare_New(const BYTE* data, const BYTE* pattern, const char* mask)
{
    for (; *mask; ++mask, ++data, ++pattern)
    {
        if (*mask == 'x' && *data != *pattern)
            return false;
    }
    return (*mask) == '\0'
}


// Prototype declarations for process/module helper functions (implement these as needed)
DWORD GetProcessIdbyName(const char* processName);
uintptr_t GetModuleBaseAddress(DWORD procId, const wchar_t* moduleName);
size_t GetModuleSize(HANDLE hProcess, uintptr_t moduleBase);

// Global storage for caching scanned addresses (new pattern) and original bytes.
std::vector<uintptr_t> cachedAddresses_NewPattern;
bool g_OriginalBytes25Stored_New = false;
unsigned char g_OriginalBytes25_New[25] = { 0 };

// -------------------------------------------------
// Generic Pattern Scanner (Safe Memory Scanning) - New Version
// -------------------------------------------------

// New helper that scans an entire region at once.
std::vector<uintptr_t> FindAllPatternsInRegion(HANDLE hProcess, uintptr_t regionStart, uintptr_t regionEnd,
    const BYTE* pattern,     const char* mask)
{
    std::vector<uintptr_t> results;
    size_t regionSize = regionEnd - regionStart;
    // Allocate a buffer to hold the entire memory region.
    std::vector<BYTE> buffer(regionSize);

    SIZE_T bytesRead = 0;
    if (!ReadProcessMemory(hProcess, reinterpret_cast<LPCVOID>(regionStart), buffer.data(), regionSize, &bytesRead))
    {
        return results; // Read failed; skip this region.
    }

    size_t patternLen = strlen(mask);
    // Scan the buffer instead of doing a remote memory read each time.
    for (size_t i = 0; i <= bytesRead - patternLen; ++i)
    {
        if (DataCompare(&buffer[i], pattern, mask))
        {
            // Convert the local buffer index to the actual memory address.
            results.push_back(regionStart + i);
        }
    }
    return results;
}


std::vector<uintptr_t> FindAllPatternsSafe_MemCH(HANDLE hProcess, uintptr_t start, size_t length,
    const BYTE* pattern, const char* mask)
{
    std::vector<uintptr_t> results;
    const size_t patternLen = strlen(mask);
    const uintptr_t end = start + length;
    uintptr_t current = start;

    while (current < end)
    {
        MEMORY_BASIC_INFORMATION mbi;
        if (!VirtualQueryEx(hProcess, reinterpret_cast<LPCVOID>(current), &mbi, sizeof(mbi)))
        {
            current += 0x1000;
            continue;
        }

        uintptr_t regionStart = reinterpret_cast<uintptr_t>(mbi.BaseAddress);
        uintptr_t regionEnd = regionStart + mbi.RegionSize;

        if (regionStart < start) regionStart = start;
        if (regionEnd > end) regionEnd = end;

        bool isReadable = (mbi.State == MEM_COMMIT) &&
            ((mbi.Protect & PAGE_GUARD) == 0) &&
            (mbi.Protect != PAGE_NOACCESS);
        if (isReadable)
        {
            std::vector<uintptr_t> regionResults = FindAllPatternsInRegion(hProcess, regionStart, regionEnd, pattern, mask);
            // Append found addresses.
            results.insert(results.end(), regionResults.begin(), regionResults.end());
        }
        current = regionEnd;
    }
    return results;
}
// ------------------------------------------------------------------------------
//  Scan Once and Cashe Result for the new 25-byte pattern - New Version
// ------------------------------------------------------------------------------
void ScanAndCacheAddresses_NewPattern(HANDLE hProcess, uintptr_t moduleBase, size_t moduleSize)
{
    if (!cachedAddresses_NewPattern.empty()) return; // already cached
    
    // 75 1C 48 88 85 86 90 00 0C 89 88 A0 00 00 00
    BYTE pattern[] =
    {
        0x75, 0x1C, 0x48, 0x88, 0x85, 0x88, 0x00, 0x00,
        0x00, 0x4C, 0x89, 0x80, 0xA0, 0x00, 0x00, 0x00
    };

    const char* mask = "xxxxxxxxxxxxxxxx";
    
    cachedAddresses_NewPattern = FindAllPatternsSafe_New(hProcess, moduleBase, moduleSize, pattern, mask);
}


void TogglePatch_NewPattern(bool applyJmp)
{
    // Get the target process and module
    DWORD procId = GetProcessIdByName("eurotrucks2.exe");
    if (!procId)
    {
        std::cout << "Process not found!" << std::endl;
        return;
    }

    uintptr_t moduleBase = GetModuleBaseAddress(procId, L"eurotrucks2.exe");
    if (!moduleBase)
    {
        std::cout << "Module base error!" << std::endl;
        return;
    }

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procId);
    if (!hProcess)
    {
        std::cout << "OpenProcess failed!" << std::endl;
        return;
    }

    size_t moduleSize = GetModuleSize(hProcess, moduleBase);
    if (!moduleSize)
    {
        std::cout << "Could not determine module size!" << std::endl;
        CloseHandle(hProcess);
        return;
    }

    // Scan once and cache addresses where the pattern is found
    ScanAndCacheAddresses_NewPattern(hProcess, moduleBase, moduleSize);
    if (cachedAddresses_NewPattern.empty())
    {
        std::cout << "Pattern not found!" << std::endl;
        CloseHandle(hProcess);
        return;
    }

    // For one occurrence, store the original 25 bytes if not stored already
    // Assumption: all occurrences are identical
    if (!g_OriginalBytes25Stored_New)
    {
        if (!ReadProcessMemory(hProcess, reinterpret_cast<LPCVOID>(cachedAddresses_NewPattern[0]), 
                             g_OriginalBytes25_New, sizeof(g_OriginalBytes25_New), nullptr))
        {
            std::cout << "ReadProcessMemory failed!" << std::endl;
            CloseHandle(hProcess);
            return;
        }
        g_OriginalBytes25Stored_New = true;
    }

    // Prepare patch bytes - copy the original, then modify the first byte conditionally
    unsigned char patchBytes[25];
    memcpy(patchBytes, g_OriginalBytes25_New, sizeof(patchBytes));
    if (applyJmp)
    {
        patchBytes[0] = 0x74; // Change from JNE (0x75) to JE (0x74)
    }
    else
    {
        // If not applying the patch, restore the original (0x75 expected)
        patchBytes[0] = g_OriginalBytes25_New[0];
    }

    // Apply the patch to each cached address
    for (uintptr_t address : cachedAddresses_NewPattern)
    {
        DWORD oldProtect;
        if (VirtualProtectEx(hProcess, reinterpret_cast<LPVOID>(address), 
                           sizeof(patchBytes), PAGE_EXECUTE_READWRITE, &oldProtect))
        {
            if (!WriteProcessMemory(hProcess, reinterpret_cast<LPVOID>(address), 
                                  patchBytes, sizeof(patchBytes), nullptr))
            {
                std::cout << "WriteProcessMemory failed at address: 0x" 
                          << std::hex << address << std::endl;
            }
            VirtualProtectEx(hProcess, reinterpret_cast<LPVOID>(address), 
                           sizeof(patchBytes), oldProtect, &oldProtect);
        }
        else
        {
            std::cout << "VirtualProtectEx failed at address: 0x" 
                      << std::hex << address << std::endl;
        }
    }

    CloseHandle(hProcess);
}

// ------------------------------------------------------------------------------
//  New entry point finction to call the patch togle
// ------------------------------------------------------------------------------
int PatchToggleEntryPoint_New()
{
    //ToglePath_NewPattern to je
    TogglePatch_NewPattern(true);

    //togle
    //togle

    return 0;
}

void TogglePatchXExternal(bool applyPatch)
{
    DWORD procId = GetProcessIdByName("eurotrucks2.exe");
    if (!procId)
    {
        std::cout << "Process not found!" << std::endl;
        return;
    }

    uintptr_t moduleBaseX = GetModuleBaseAddress(procId, L"eurotrucks2.exe");
    if (!moduleBaseX)
    {
        std::cout << "Module base error!" << std::endl;
        return;
    }

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procId);
    if (!hProcess)
    {
        std::cout << "OpenProcess failed!" << std::endl;
        return;
    }

    uintptr_t targetAddressX = moduleBaseX + 0xE68D7; // eurotrucks2.exe+E68D7 - F2 0F11 83 38030600 - movsd [rbx+00000338],xmm0

    const size_t patchSizeX = 8;
    
    unsigned char patchBytesX[patchSizeX];

    if (applyPatch)
    {
        unsigned char modifiedBytesX[patchSizeX] = { 0xF2, 0x0F, 0x11, 0x9B, 0x38, 0x03, 0x00, 0x00 };
        memcpy(patchBytesX, modifiedBytesX, patchSizeX);
    }
    else
    {
        unsigned char originalBytesX[patchSizeX] = { 0xF2, 0x0F, 0x11, 0x83, 0x38, 0x03, 0x00, 0x00 };
        memcpy(patchBytesX, originalBytesX, patchSizeX);
    }

    DWORD oldProtectX;
    if (VirtualProtectEx(hProcess, reinterpret_cast<LPVOID>(targetAddressX), patchSizeX, PAGE_EXECUTE_READWRITE, &oldProtectX))
    {
        if (!WriteProcessMemory(hProcess, reinterpret_cast<LPVOID>(targetAddressX), patchBytesX, patchSizeX, nullptr))
        {
            std::cout << "WriteProcessMemory failed!" << std::endl;
        }
        
        VirtualProtectEx(hProcess, reinterpret_cast<LPVOID>(targetAddressX), patchSizeX, oldProtectX, &oldProtectX);
    }
    else
    {
        std::cout << "VirtualProtectEx failed!" << std::endl;
    }

    CloseHandle(hProcess);
}

bool testEnabled = false;
bool testON = false;
bool testOFF = false;

unsigned char originalBytes[3] = { 0x48, 0x88, 0xC4 };

DWORD GetProcessIdByName(const char* processName);
uintptr_t GetModuleBaseAddress(DWORD procId, const wchar_t* moduleName);

// Store scanned addresses to avoid rescanning
std::vector<uintptr_t> cachedAddresses;

// ------------------------------------------------------------------------------
// Scan Once and Cache Results
// ------------------------------------------------------------------------------
std::vector<uintptr_t> FindAllPatternsSafe(HANDLE hProcess, uintptr_t start, size_t length,
    const BYTE* pattern, const char* mask)
{
    std::vector<uintptr_t> results;
    const size_t patternLen = strlen(mask);
    const uintptr_t end = start + length;
    uintptr_t current = start;

    while (current < end)
    {
        MEMORY_BASIC_INFORMATION mbi;
        if (!VirtualQueryEx(hProcess, reinterpret_cast<LPCVOID>(current), &mbi, sizeof(mbi)))
        {
            current += 0x1000;
            continue;
        }

        uintptr_t regionStart = reinterpret_cast<uintptr_t>(mbi.BaseAddress);
        uintptr_t regionEnd = regionStart + mbi.RegionSize;

        if (regionStart < start) regionStart = start;
        if (regionEnd > end) regionEnd = end;

        bool isReadable = (mbi.State == MEM_COMMIT) &&
                        ((mbi.Protect & PAGE_GUARD) == 0) &&
                        (mbi.Protect != PAGE_NOACCESS);

        if (isReadable)
        {
            for (uintptr_t addr = regionStart; addr + patternLen <= regionEnd; ++addr)
            {
                BYTE localBuf[64]; // Make sure this buffer is large enough for your pattern
                SIZE_T bytesRead = 0;
                if (ReadProcessMemory(hProcess, reinterpret_cast<LPCVOID>(addr),
                                    localBuf, patternLen, &bytesRead) &&
                    bytesRead == patternLen)
                {
                    if (DataCompare(localBuf, pattern, mask))
                    {
                        results.push_back(addr);
                    }
                }
            }
        }
        current = regionEnd;
    }

    return results;
}

void ScanAndCacheAddresses(HANDLE hProcess, uintptr_t moduleBase, size_t moduleSize)
{
    if (!cachedAddresses.empty()) return; // Already scanned, no need to rescan

    BYTE pattern[] =
    {
        0x48, 0x8B, 0xC4, 0x56, 0x57, 0x41, 0x54, 0x41,
        0x56, 0x41, 0x57, 0x48, 0x83, 0xEC, 0x58, 0x48,
        0xC7, 0x40, 0x48
    };
    const char* mask = "xxxxxxxxxxxxxxxxxxx"; // 19 bytes (соответствует размеру pattern)

    cachedAddresses = FindAllPatternsSafe(hProcess, moduleBase, moduleSize, pattern, mask);
}

// ------------------------------------------------------------------------------
// Apply Patch Using Cached Addresses
// ------------------------------------------------------------------------------
void ApplyTestPatch()
{
    DWORD procId = GetProcessIdByName("eurotrucks2.exe");
    if (!procId)
    {
        std::cout << "Process not found!" << std::endl;
        return;
    }

    uintptr_t moduleBase = GetModuleBaseAddress(procId, L"core_ets2mp.dll");
    if (!moduleBase)
    {
        std::cout << "Module base error!" << std::endl;
        return;
    }

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procId);
    if (!hProcess)
    {
        std::cout << "OpenProcess failed!" << std::endl;
        return;
    }

    size_t moduleSize = GetModuleSize(hProcess, moduleBase);
    if (!moduleSize)
    {
        std::cout << "Could not determine module size!" << std::endl;
        CloseHandle(hProcess);
        return;
    }

    // ** Scan once and cache result
    ScanAndCacheAddresses(hProcess, moduleBase, moduleSize);
    if (cachedAddresses.empty())
    {
        std::cout << "Pattern not found!" << std::endl;
        CloseHandle(hProcess);
        return;
    }

    //detemire the patch bytes
    const size_t patchSize = 3;
    unsigned char patchBytes[patchSize];
    
    if (!testEnabled)
    {
        memcpy(patchBytes, originalBytes, patchSize);
    }
    else
    {
        if (testON)
        {
            patchBytes[0] = 0x80;
            patchBytes[1] = 0x00;
            patchBytes[2] = 0xC3;
        }
        else if (testOFF)
        {
            patchBytes[0] = 0x80;
            patchBytes[1] = 0x01;
            patchBytes[2] = 0xC3;
        }
        else
        {
            memcpy(patchBytes, originalBytes, patchSize);
        }
    }

    //apply patch at all cached addresses
    for (uintptr_t address : cachedAddresses)
    {
        DWORD oldProtect;
        if (VirtualProtectEx(hProcess, reinterpret_cast<LPVOID>(address), 
            patchSize, PAGE_EXECUTE_READWRITE, &oldProtect))
        {
            if (!WriteProcessMemory(hProcess, reinterpret_cast<LPVOID>(address), 
                patchBytes, patchSize, nullptr))
            {
                std::cout << "WriteProcessMemory failed at address: 0x" 
                          << std::hex << address << std::endl;
            }
            VirtualProtectEx(hProcess, reinterpret_cast<LPVOID>(address), 
                patchSize, oldProtect, &oldProtect);
        }
        else
        {
            std::cout << "VirtualProtectEx failed at address: 0x"
                      << std::hex << address << std::endl;
        }
    }

    CloseHandle(hProcess);
}

void ColorAdjustedExternal(bool changeToXmm1)
{
    DWORD procId = GetProcessIdByName("eurotrucks2.exe");
    if (!procId)
    {
        std::cout << "Process not found!" << std::endl;
        return;
    }

    uintptr_t moduleBase = GetModuleBaseAddress(procId, L"eurotrucks2.exe");
    if (!moduleBase)
    {
        std::cout << "Module base error!" << std::endl;
        return;
    }

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procId);
    if (!hProcess)
    {
        std::cout << "OpenProcess failed!" << std::endl;
        return;
    }

    uintptr_t targetAddress8 = moduleBase + 0x40A610;
    const size_t patchSize8 = 3;
    unsigned char patchBytes8[patchSize8];

    if (changeToXmm1)
    {
        unsigned char newBytes8[patchSize8] = { 0x0F, 0x11, 0x09 };
        memcpy(patchBytes8, newBytes8, patchSize8);
    }
    else
    {
        unsigned char originalBytes8[patchSize8] = { 0x0F, 0x11, 0x01 };
        memcpy(patchBytes8, originalBytes8, patchSize8);
    }

    DWORD oldProtect;
    if (VirtualProtectEx(hProcess, reinterpret_cast<LPVOID>(targetAddress8), patchSize8, PAGE_EXECUTE_READWRITE, &oldProtect))
    {
        if (!WriteProcessMemory(hProcess, reinterpret_cast<LPVOID>(targetAddress8), patchBytes8, patchSize8, nullptr))
        {
            std::cout << "WriteProcessMemory failed!" << std::endl;
        }

        VirtualProtectEx(hProcess, reinterpret_cast<LPVOID>(targetAddress8), patchSize8, oldProtect, &oldProtect);
    }
    else
    {
        std::cout << "VirtualProtectEx failed!" << std::endl;
    }

    CloseHandle(hProcess);
}

void ColorAdjustedExternalP(bool changeToXmm1)
{
    DWORD procId = GetProcessIdByName("eurotrucks2.exe");
    if (!procId)
    {
        std::cout << "Process not found!" << std::endl;
        return;
    }

    uintptr_t moduleBase = GetModuleBaseAddress(procId, L"eurotrucks2.exe");
    if (!moduleBase)
    {
        std::cout << "Module base error!" << std::endl;
        return;
    }

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procId);
    if (!hProcess)
    {
        std::cout << "OpenProcess failed!" << std::endl;
        return;
    }

    uintptr_t targetAddress8 = moduleBase + 0x40A619;
    const size_t patchSize8 = 3;
    unsigned char patchBytes8[patchSize8];

    if (changeToXmm1)
    {
        unsigned char newBytes8[patchSize8] = { 0x0F, 0x11, 0x31 };
        memcpy(patchBytes8, newBytes8, patchSize8);
    }
    else
    {
        unsigned char originalBytes8[patchSize8] = { 0x0F, 0x11, 0x01 };
        memcpy(patchBytes8, originalBytes8, patchSize8);
    }

    DWORD oldProtect;
    if (VirtualProtectEx(hProcess, reinterpret_cast<LPVOID>(targetAddress8), patchSize8, PAGE_EXECUTE_READWRITE, &oldProtect))
    {
        if (!WriteProcessMemory(hProcess, reinterpret_cast<LPVOID>(targetAddress8), patchBytes8, patchSize8, nullptr))
        {
            std::cout << "WriteProcessMemory failed!" << std::endl;
        }

        VirtualProtectEx(hProcess, reinterpret_cast<LPVOID>(targetAddress8), patchSize8, oldProtect, &oldProtect);
    }
    else
    {
        std::cout << "VirtualProtectEx failed!" << std::endl;
    }

    CloseHandle(hProcess);
}

void ColorAdjustedExternal(bool applyPatch, int brakePowerValue)
{
    // Get process ID
    DWORD procId = GetProcessIdByName("eurotrucks2.exe");
    if (!procId)
    {
        std::cout << "Process not found!" << std::endl;
        return;
    }

    // Get module base address
    uintptr_t moduleBase = GetModuleBaseAddress(procId, L"eurotrucks2.exe");
    if (!moduleBase)
    {
        std::cout << "Module base error!" << std::endl;
        return;
    }

    // Open process with full access
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procId);
    if (!hProcess)
    {
        std::cout << "OpenProcess failed!" << std::endl;
        return;
    }

    uintptr_t targetAddress2 = moduleBase + 0x5D0013;
    const size_t patchSize2 = 8;

    static unsigned char originalBytes2[patchSize2] = { 0 };
    static bool originalBytesStored = false;
    unsigned char patchBytes2[patchSize2];

    // Store original bytes once
    if (!originalBytesStored)
    {
        if (!ReadProcessMemory(hProcess, reinterpret_cast<LPCVOID>(targetAddress2), originalBytes2, patchSize2, nullptr))
        {
            std::cout << "ReadProcessMemory failed to read original bytes!" << std::endl;
            CloseHandle(hProcess);
            return;
        }
        originalBytesStored = true;
    }

    // Start with the original instruction bytes
    memcpy(patchBytes2, originalBytes2, patchSize2);

    if (applyPatch)
    {
        // Convert the slider's integer value to a float
        float newBrakeValue = static_cast<float>(brakePowerValue);
        // The immediate value is stored in the last 4 bytes (offset 4) of the instruction
        memcpy(&patchBytes2[4], &newBrakeValue, sizeof(newBrakeValue));
    }

    // Apply the patch
    DWORD oldProtect;
    if (VirtualProtectEx(hProcess, reinterpret_cast<LPVOID>(targetAddress2), patchSize2,PAGE_EXECUTE_READWRITE, &oldProtect))
    {
        if (!WriteProcessMemory(hProcess, reinterpret_cast<LPVOID>(targetAddress2),patchBytes2, patchSize2, nullptr))
        {
            std::cout << "WriteProcessMemory failed!" << std::endl;
        }

        FlushInstructionCache(hProcess, reinterpret_cast<LPVOID>(targetAddress2), patchSize2);
        VirtualProtectEx(hProcess, reinterpret_cast<LPVOID>(targetAddress2), patchSize2, oldProtect, &oldProtect);
    }
    else
    {
        std::cout << "VirtualProtectEx failed!" << std::endl;
    }

    CloseHandle(hProcess);
}

void colorajustredf(bool changeToXmm1)
{
    DWORD procId = GetProcessIdByName("eurotrucks2.exe");
    if (!procId)
    {
        std::cout << "Process not found!" << std::endl;
        return;
    }

    uintptr_t moduleBase = GetModuleBaseAddress(procId, L"eurotrucks2.exe");
    if (!moduleBase)
    {
        std::cout << "Module base error!" << std::endl;
        return;
    }

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procId);
    if (!hProcess)
    {
        std::cout << "OpenProcess failed!" << std::endl;
        return;
    }

    uintptr_t targetAddress8 = moduleBase + 0x0A61D;
    const size_t patchSize8 = 3;
    unsigned char patchBytes8[patchSize8];

    if (changeToXmm1)
    {
        unsigned char newBytes8[patchSize8] = { 0x0F, 0x11, 0x29 };
        memcpy(patchBytes8, newBytes8, patchSize8);
    }
    else
    {
        unsigned char originalBytes8[patchSize8] = { 0x0F, 0x11, 0x30 };
        memcpy(patchBytes8, originalBytes8, patchSize8);
    }

    DWORD oldProtect;
    if (VirtualProtectEx(hProcess, reinterpret_cast<LPVOID>(targetAddress8), patchSize8, PAGE_EXECUTE_READWRITE, &oldProtect))
    {
        if (!WriteProcessMemory(hProcess, reinterpret_cast<LPVOID>(targetAddress8), patchBytes8, patchSize8, nullptr))
        {
            std::cout << "WriteProcessMemory failed!" << std::endl;
        }
        
        VirtualProtectEx(hProcess, reinterpret_cast<LPVOID>(targetAddress8), patchSize8, oldProtect, &oldProtect);
    }
    else
    {
        std::cout << "VirtualProtectEx failed!" << std::endl;
    }

    CloseHandle(hProcess);
}



// Forward declaration for the window procedure.



void ui_main()
{
    try
    {
        WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L,
                          GetModuleHandle(NULL), NULL, NULL, NULL, NULL, _T("Ghosty"), NULL };
        RegisterClassEx(&wc);
        
        HWND hwnd = CreateWindow(wc.lpszClassName, _T("Ghosty"), WS_OVERLAPPEDWINDOW, 
                                0, 0, 850, 750, NULL, NULL, wc.hInstance, NULL);
        
        if (!CreateDeviceD3D(hwnd))
        {
            CleanupDeviceD3D();
            UnregisterClass(wc.lpszClassName, wc.hInstance);
            return;
        }
        
        UpdateWindow(hwnd);
        
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
        io.IniFilename = nullptr;
        io.LogFilename = nullptr;
        
        theme.Register(io);
        ApplyBlackPurpleTheme();
        printf("ImGui Version: %s\n", IMGUI_VERSION);

        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            style.WindowRounding = 4.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }
        style.WindowTitleAlign = ImVec2(0.5f, 0.5f);

        ImGui_ImplWin32_Init(hwnd);
        ImGui_ImplDX9_Init(g_pd3dDevice);

        bool done = false;
        std::string str_window_title = "Ghosty_lite - FREE Updated [1.54]";
        const char* window_title = str_window_title.c_str();
        ImVec2 window_size(958, 700);
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoSavedSettings | 
                                      ImGuiWindowFlags_NoScrollbar | 
                                      ImGuiHoveredFlags_AllowWhenBlockedByActiveItem;

        while (!done)
        {
            MSG msg;
            while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
                if (msg.message == WM_QUIT)
                    done = true;
            }
            if (done)
                break;

            ImGui_ImplDX9_NewFrame();
            ImGui_ImplWin32_NewFrame();
            ImGui::NewFrame();

            bool show_menu = true;
            static bool reset_size = true;
            if (show_menu)
            {
                if (reset_size)
                {
                    ImGui::SetNextWindowSize(window_size);
                    reset_size = false;
                }
                
                ImGui::SetNextWindowBgAlpha(1.0f);
                ImGui::PushFont(theme.font);
                ImGui::Begin(window_title, &show_menu, window_flags);

                static int selectedTab = 0;
                if (ImGui::Button("Main", ImVec2(150, 30))) selectedTab = 0;
                ImGui::SameLine();
                if (ImGui::Button("Cabin and Camera", ImVec2(150, 30))) selectedTab = 1;
                ImGui::SameLine();
                if (ImGui::Button("Info", ImVec2(150, 30))) selectedTab = 2;
                ImGui::Separator();

                DWORD procId = GetProcessIdByName("eurotrucks2.exe");
                if (procId == 0)
                {
                    ImGui::TextColored(ImVec4(1, 0, 0, 1), "Game Status: Unattached");
                }
                else
                {
                    ImGui::TextColored(ImVec4(0, 1, 0, 1), "Game Status: Attached");
                }

                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 1, 1));
                if (ImGui::Button("Start game via Steam"))
                {
                    ShellExecuteA(NULL, "open", "steam://rungameid/227308", NULL, NULL, SW_SHOWNORMAL);
                }

                ImGui::SameLine();
                if (ImGui::Button("Start TruckersMP Launcher"))
                {
                    const char* localAppData = std::getenv("LOCALAPPDATA");
                    if (localAppData)
                    {
                        char truckersmpPath[MAX_PATH];
                        std::sprintf(truckersmpPath, "%s\\TruckersMP\\TruckersMP-Launcher.exe", localAppData);
                        ShellExecuteA(NULL, "open", truckersmpPath, NULL, NULL, SW_SHOWNORMAL);
                    }
                }







                ImGui::PopStyleColor();


                ImGui::SameLine();
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 0, 0, 1));
                if (ImGui::Button("Kill Game Process")) 
                {
                    DWORD ets2ProcId = GetProcessIdByName("eurotrucks2.exe");
                    if (ets2ProcId != 0) 
                    {
                        HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, ets2ProcId);
                        if (hProcess) 
                        {
                            TerminateProcess(hProcess, 0);
                            CloseHandle(hProcess);
                        }
                    }
                }
                ImGui::PopStyleColor();
                ImGui::Separator();

                switch (selectedTab) 
                {
                    case 0:
                    {
                        static int customKey = 'A';
                        static bool waitingForKey = false;
                        static bool fakelagEnabled = false;

                        if (ImGui::Button(waitingForKey ? "Press any key..." : "Change Fake Lag Keybind")) 
                        {
                            waitingForKey = true;
                        }
                        
                        if (waitingForKey) 
                        {
                            for (int key = 0; key < 256; key++) 
                            {
                                if (ImGui::IsKeyPressed(key)) 
                                {
                                    customKey = key;
                                    waitingForKey = false;
                                    break;
                                }
                            }
                        }

                        if (GetAsyncKeyState(customKey) & 0x8000)
                        {
                            fakelagEnabled = true;
                        }
                        else
                        {
                            fakelagEnabled = false;
                        }

                        char keybindText[64];
                        sprintf(keybindText, "Fake Lag Toggle (Hold %c) SERVER", customKey);
                        ImGui::Checkbox(keybindText, &fakelagEnabled);

                        float fakelagValue = fakelagEnabled ? 1.0f : 0.0f;

                        DWORD procId = GetProcessIdByName("eurotrucks2.exe");
                        if (procId != 0)
                        {
                            uintptr_t moduleBase = GetModuleBaseAddress(procId, L"eurotrucks2.exe");
                            if (moduleBase != 0)
                            {
                                HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procId);
                                if (hProcess)
                                {
                                    std::vector<unsigned int> offsets = { 0x38, 0x8, 0xC8, 0x1D0, 0x3A0, 0x758 };
                                    uintptr_t address = FindDMAAddy(hProcess, moduleBase + 0x0214F238, offsets);
                                    WriteFloat(hProcess, address, fakelagValue);
                                    CloseHandle(hProcess);
                                }
                            }
                        }

                        static bool jumpFunctionEnabled = false;
                        static int jumpPower = 1;
                        ImGui::Checkbox("Enable Jump Function", &jumpFunctionEnabled);
                        ImGui::SliderInt("Jump Power", &jumpPower, 1, 10);

                        if (jumpFunctionEnabled && (GetAsyncKeyState(VK_SPACE) & 0x8000))
                        {
                            DWORD procId = GetProcessIdByName("eurotrucks2.exe");
                            if (procId == 0)
                            {
                                ImGui::TextColored(ImVec4(1, 0, 0, 1), "Target process not found!");
                            }
                            else
                            {
                                uintptr_t moduleBase = GetModuleBaseAddress(procId, L"eurotrucks2.exe");
                                if (moduleBase == 0)
                                {
                                    ImGui::TextColored(ImVec4(1, 0, 0, 1), "Failed to get module base address!");
                                }
                                else
                                {
                                    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procId);
                                    if (hProcess != NULL)
                                    {
                                        uintptr_t dynamicBaseAddr = moduleBase + 0x02FCEB80;
                                        std::vector<unsigned int> jumpOffsets = { 0x0, 0x10, 0x8, 0x9, 0x100, 0x10 };
                                        uintptr_t jumpAddr = FindDMAAddy(hProcess, dynamicBaseAddr, jumpOffsets);
                                        if (jumpAddr != 0)
                                        {
                                            float currentJumpValue = 0.0f;
                                            if (ReadProcessMemory(hProcess, (LPCVOID)jumpAddr, &currentJumpValue, sizeof(float), nullptr))
                                            {
                                                currentJumpValue += static_cast<float>(jumpPower);
                                                if (!WriteFloat(hProcess, jumpAddr, currentJumpValue))
                                                {
                                                    ImGui::TextColored(ImVec4(1, 0, 0, 1), "Failed to update jump value!");
                                                }
                                            }
                                        }
                                        CloseHandle(hProcess);
                                    }
                                }
                            }
                        }
















                    // not find this code



































                        static bool isChecked2 = false;
                        static bool lockTo110 = false;
                        static bool lockTo150 = false;
                        static std::atomic<bool> speedUnlockerThreadActive = false;
                        static std::atomic<bool> pointerError(false);

                        // Speed Unlocker Checkbox
                        if (ImGui::Checkbox("Speed Unlocker    |   IMM", &isChecked2))
                        {
                            TogglePatchExternalSpeed(isChecked2);
                        }

                        // Tooltip
                        ImGui::SameLine();
                        ImGui::TextDisabled("(?)");
                        if (ImGui::IsItemHovered())
                        {
                            ImGui::BeginTooltip();
                            ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.8f);
                            ImGui::TextUnformatted("Detectable if you go above default speed limit, lockable at 110 and 150 [May have long loading time]");
                            ImGui::PopTextWrapPos();
                            ImGui::EndTooltip();
                        }

                        // Lock Speed Options
                        if (isChecked2)
                        {
                            ImGui::SameLine();
                            ImGui::Checkbox("Lock to 110", &lockTo110);
                            
                            ImGui::SameLine();
                            ImGui::Checkbox("Lock to 150", &lockTo150);
                        }

                        // Speed Unlocker Thread
                        if (isChecked2 && !speedUnlockerThreadActive.load())
                        {
                            speedUnlockerThreadActive.store(true);
                            std::thread([&] {
                                DWORD procId = GetProcessIdByName("eurotrucks2.exe");
                                if (procId == 0)
                                {
                                    pointerError.store(true);
                                    speedUnlockerThreadActive.store(false);
                                    return;
                                }

                                uintptr_t moduleBase2 = GetModuleBaseAddress(procId, L"core_ets2mp.dll");
                                if (moduleBase2 == 0)
                                {
                                    pointerError.store(true);
                                    speedUnlockerThreadActive.store(false);
                                    return;
                                }

                                HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procId);
                                if (hProcess == NULL)
                                {
                                    pointerError.store(true);
                                    speedUnlockerThreadActive.store(false);
                                    return;
                                }

                                struct PointerChain {
                                    uintptr_t base;
                                    std::vector<unsigned int> offsets;
                                };

                                std::vector<PointerChain> chains = {
                                    { moduleBase2 + 0x01080900, { 0x28, 0x28, 0x10, 0x16C } }
                                };

                                pointerError.store(false);
                                
                                while (isChecked2)
                                {
                                    float speedValue = 0.0f; // Default (no limit)
                                    
                                    if (lockTo110)
                                        speedValue = 116.0f;
                                    else if (lockTo150)
                                        speedValue = 159.0f;

                                    bool pointerFound = false;
                                    for (const auto& chain : chains)
                                    {
                                        uintptr_t speedAddr = FindDMAAddy(hProcess, chain.base, chain.offsets);
                                        if (speedAddr != 0)
                                        {
                                            WriteFloat(hProcess, speedAddr, speedValue);
                                            pointerFound = true;
                                            break;
                                        }
                                    }

                                    pointerError.store(!pointerFound);
                                    Sleep(200);
                                }

                                CloseHandle(hProcess);
                                speedUnlockerThreadActive.store(false);
                            }).detach();
                        }


                        // Global variables
                        static bool rotateTruckEnabled = false;


                        //

                        ImGui::Checkbox("Rotate truck 90 Degree (K key)", &rotateTruckEnabled);
    

                        // Check if rotation is enabled and K is pressed
                        if (rotateTruckEnabled && (GetAsyncKeyState('K') & 0x1))
                        {
                            DWORD procId = GetProcessIdByName("eurotrucks2.exe");
                            if (procId == 0)
                            {
                                ImGui::TextColored(ImVec4(1, 0, 0, 1), "Target process not found!");
                            }


                            else
                            {
                                uintptr_t moduleBase = GetModuleBaseAddress(procId, L"eurotrucks2.exe");
                                HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procId);
                                


                                if (hProcess != NULL)
                                {
                                    // Use the correct offset for truck rotation
                                    uintptr_t dynamicBaseAddr = moduleBase + 0x02FCEEB0;
                                    std::vector<unsigned int> rotateOffsets = { 0x9, 0x159, 0x18, 0x8, 0x190, 0x8, 0x20 };
                                    uintptr_t rotateAddr = FindDMAAddy(hProcess, dynamicBaseAddr, rotateOffsets);
                                    
                                    if (rotateAddr != 0)
                                    {
                                        // Set rotation directly to 90 degrees (1.5708 radians)
                                        if (!WriteFloat(hProcess, rotateAddr, 1.5708f))
                                        {
                                            ImGui::TextColored(ImVec4(1, 0, 0, 1), "Failed to set rotation value!");
                                        }
                                    }
                                    else
                                    {
                                        ImGui::TextColored(ImVec4(1, 0, 0, 1), "Rotation offset not found!");
                                    }
                                    CloseHandle(hProcess);
                                }
                                else
                                {
                                    ImGui::TextColored(ImVec4(1, 0, 0, 1), "Failed to open process!");
                                }
                            }
                        }
                        // Infinite Zoom Checkbox
                        if (ImGui::Checkbox("Infinite Zoom | CLIENT", &patchAppliedB))
                        {
                            TogglePatchB(patchAppliedB);
                        }
                        // Headlight Effect Checkbox
                        /*ImGui::Checkbox("HeadLight Effect SERVER", &toggleLoopCheckbox);
                        
                        if (toggleLoopCheckbox && !toggleLoopThreadActive.load())
                        {
                            toggleLoopThreadActive.store(true);
                            std::thread([] {
                                DWORD procId = GetProcessIdByName("eurotrucks2.exe");
                                if (procId == 0)
                                {
                                    ImGui::TextColored(ImVec4(1, 0, 0, 1), "Target process not found!");
                                    toggleLoopThreadActive.store(false);
                                    return;
                                }

                                uintptr_t moduleBase = GetModuleBaseAddress(procId, L"eurotrucks2.exe");
                                if (moduleBase == 0)
                                {
                                    ImGui::TextColored(ImVec4(1, 0, 0, 1), "Failed to get module base address!");
                                    toggleLoopThreadActive.store(false);
                                    return;
                                }

                                HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procId);
                                if (hProcess == NULL)
                                {
                                    ImGui::TextColored(ImVec4(1, 0, 0, 1), "Failed to open process!");
                                    toggleLoopThreadActive.store(false);
                                    return;
                                }

                                uintptr_t dynamicBaseAddr = moduleBase + 0x020AADE0;
                                std::vector<unsigned int> headLightOffsets = { 0xF78, 0x140 };
                                
                                while (toggleLoopCheckbox)
                                {
                                    uintptr_t loopValueAddr = FindDMAAddy(hProcess, dynamicBaseAddr, headLightOffsets);
                                    if (loopValueAddr != 0)
                                    {
                                        WriteFloat(hProcess, loopValueAddr, 0.0f);
                                    }
                                    Sleep(200);
                                    
                                    if (!toggleLoopCheckbox) break;
                                    
                                    if (loopValueAddr != 0)
                                    {
                                        WriteFloat(hProcess, loopValueAddr, 1.0f);
                                    }
                                    Sleep(500);
                                }
                                
                                CloseHandle(hProcess);
                                toggleLoopThreadActive.store(false);
                            }).detach();
                        }*/

                        static bool patchAppliedF = false;
                        static std::atomic<bool> patchThreadActive(false);

                        // In your ImGui rendering loop:
                        ImGui::Checkbox("Infinite Fuel | CLIENT", &patchAppliedF);

                        if (patchAppliedF && !patchThreadActive.load())
                        {
                            patchThreadActive.store(true);
                            std::thread([] {
                                DWORD procId = GetProcessIdByName("eurotrucks2.exe");
                                if (procId == 0)
                                {
                                    patchThreadActive.store(false);
                                    return;
                                }

                                uintptr_t moduleBase = GetModuleBaseAddress(procId, L"eurotrucks2.exe");
                                if (moduleBase == 0)
                                {
                                    patchThreadActive.store(false);
                                    return;
                                }

                                HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procId);
                                if (hProcess == NULL)
                                {
                                    patchThreadActive.store(false);
                                    return;
                                }

                                uintptr_t dynamicBaseAddr = moduleBase + 0x03095598;
                                std::vector<unsigned int> fuelOffsets = { 0x48, 0x08, 0x18, 0x0, 0x178 };

                                while (patchAppliedF)
                                {
                                    uintptr_t fuelAddr = FindDMAAddy(hProcess, dynamicBaseAddr, fuelOffsets);
                                    if (fuelAddr != 0)
                                    {
                                        WriteFloat(hProcess, fuelAddr, 1000.0f); // Set to high value instead of 1.0 for more natural behavior
                                    }
                                    Sleep(200);
                                }

                                CloseHandle(hProcess);
                                patchThreadActive.store(false);
                            }).detach();
                        }
                        // No Damage Checkbox
                        static bool patchAppliedD = false;
                        if (ImGui::Checkbox("No Damage | CLIENT", &patchAppliedD))
                        {
                            noDamage(patchAppliedD);
                        }

                        if (ImGui::IsItemHovered())
                        {
                            ImGui::BeginTooltip();
                            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.0f);
                            ImGui::Text("Trigger No damage by hitting something at high speed (so you get damage)");
                            ImGui::PopStyleVar();
                            ImGui::EndTooltip();
                        }

                        // No Light Checkbox
                        static bool patchApplied2 = false;
                        if (ImGui::Checkbox("No Light (BYPASS TMP) | CLIENT [Beacon ON First]", &patchApplied2))
                        {
                            TogglePatch2External(patchApplied2);
                        }

                        // Freeze Turn Signals Checkbox
                        static bool isChecked = false;
                        if (ImGui::Checkbox("Freeze Turn Signals | SERVER", &isChecked))
                        {
                            ToggleAlarmTimerPatchExternal(isChecked);
                        }

                        // Noclip Checkbox
                        static bool patchApplied6 = false;
                        static bool keyWasDown = false;

                        if (ImGui::Checkbox("Noclip [Ctrl+M] | SETA", &patchApplied6))
                        {
                            TogglePatch_NewPattern(patchApplied6);
                        }

                        if (ImGui::IsItemHovered())
                        {
                            ImGui::BeginTooltip();
                            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.0f);
                            ImGui::Text("Can sometimes fling you, to avoid that keep your truck straight with some speed.");
                            ImGui::PopStyleVar();
                            ImGui::EndTooltip();
                        }

                        // Noclip Hotkey (Ctrl+M)
                        if ((GetAsyncKeyState(VK_CONTROL) & 0x8000) && (GetAsyncKeyState('M') & 0x8000))
                        {
                            if (!keyWasDown)
                            {
                                patchApplied6 = !patchApplied6;
                                TogglePatch_NewPattern(patchApplied6);
                                keyWasDown = true;
                            }
                        }
                        else
                        {
                            keyWasDown = false;
                        }

                        // Ghost Mode Checkbox
                        static bool testEnabled = false;
                        static bool testON = false;
                        static bool testOFF = false;

                        if (ImGui::Checkbox("Toggle Ghost Mode | TMP SERVER", &testEnabled))
                        {
                            if (!testEnabled)
                            {
                                testON = false;
                                testOFF = false;
                            }
                            ApplyTestPatch();
                        }

                        if (ImGui::IsItemHovered())
                        {
                            ImGui::BeginTooltip();
                            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.0f);
                            ImGui::Text("May have long loading time.");
                            ImGui::PopStyleVar();
                            ImGui::EndTooltip();
                        }

                        if (testEnabled)
                        {
                            ImGui::SameLine();
                            if (ImGui::Checkbox("Enable Ghost Mode", &testON))
                            {
                                if (testON)
                                    testOFF = false;
                                ApplyTestPatch();
                            }
                            ImGui::SameLine();
                            if (ImGui::Checkbox("Disable Ghost Mode", &testOFF))
                            {
                                if (testOFF)
                                    testON = false;
                                ApplyTestPatch();
                            }
                        }
                        static float rearValue = 1.0f;
                        static float frontValue = 1.0f;
                        // Front Suspension Slider
                        if (ImGui::SliderFloat("Front Suspension SERVER", &frontValue, 0.1f, 10.0f, "%.1f"))
                        {
                            DWORD procId = GetProcessIdByName("eurotrucks2.exe");
                            if (procId == 0)
                            {
                                ImGui::TextColored(ImVec4(1, 0, 0, 1), "Target process not found!");
                            }
                            else
                            {
                                uintptr_t moduleBase = GetModuleBaseAddress(procId, L"eurotrucks2.exe");
                                if (moduleBase == 0)
                                {
                                    ImGui::TextColored(ImVec4(1, 0, 0, 1), "Failed to get module base address!");
                                }
                                else
                                {
                                    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procId);
                                    if (hProcess != NULL)
                                    {
                                        uintptr_t dynamicBaseAddr = moduleBase + 0x02FCE0C0;
                                        std::vector<unsigned int> frontOffsets = { 0x0, 0x8, 0x8, 0x39C };
                                        uintptr_t frontSuspensionAddr = FindDMAAddy(hProcess, dynamicBaseAddr, frontOffsets);
                                        
                                        if (frontSuspensionAddr != 0)
                                        {
                                            if (!WriteFloat(hProcess, frontSuspensionAddr, frontValue))
                                            {
                                                ImGui::TextColored(ImVec4(1, 0, 0, 1), "Failed to update Front Suspension!");
                                            }
                                        }
                                        else
                                        {
                                            ImGui::TextColored(ImVec4(1, 0, 0, 1), "Failed to resolve Front Suspension address!");
                                        }
                                        CloseHandle(hProcess);
                                    }
                                    else
                                    {
                                        ImGui::TextColored(ImVec4(1, 0, 0, 1), "Failed to open process!");
                                    }
                                }
                            }
                        }
                        // Rear Suspension Slider
                        if (ImGui::SliderFloat("Rear Suspension SERVER", &rearValue, 0.1f, 10.0f, "%.1f"))
                        {
                            DWORD procId = GetProcessIdByName("eurotrucks2.exe");
                            if (procId == 0)
                            {
                                ImGui::TextColored(ImVec4(1, 0, 0, 1), "Target process not found!");
                            }
                            else
                            {
                                uintptr_t moduleBase = GetModuleBaseAddress(procId, L"eurotrucks2.exe");
                                if (moduleBase == 0)
                                {
                                    ImGui::TextColored(ImVec4(1, 0, 0, 1), "Failed to get module base address!");
                                }
                                else
                                {
                                    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procId);
                                    if (hProcess != NULL)
                                    {
                                        uintptr_t dynamicBaseAddr = moduleBase + 0x030045B8;
                                        std::vector<unsigned int> rearOffsets = { 0x38, 0x48, 0x290, 0x3A0 };
                                        uintptr_t rearSuspensionAddr = FindDMAAddy(hProcess, dynamicBaseAddr, rearOffsets);
                                        
                                        if (rearSuspensionAddr != 0)
                                        {
                                            if (!WriteFloat(hProcess, rearSuspensionAddr, rearValue))
                                            {
                                                ImGui::TextColored(ImVec4(1, 0, 0, 1), "Failed to update Rear Suspension!");
                                            }
                                        }
                                        else
                                        {
                                            ImGui::TextColored(ImVec4(1, 0, 0, 1), "Failed to resolve Rear Suspension address!");
                                        }
                                        CloseHandle(hProcess);
                                    }
                                    else
                                    {
                                        ImGui::TextColored(ImVec4(1, 0, 0, 1), "Failed to open process!");
                                    }
                                }
                            }
                        }
                        static float brakePowerValue = 1.0f; // initial value (float)

                        // Create the slider (range from 1 to 1000, no decimals shown)
                        if (ImGui::SliderFloat("Brake Power", &brakePowerValue, 1.0f, 1000.0f, "%.0f"))
                        {
                            DWORD procId = GetProcessIdByName("eurotrucks2.exe");
                            if (procId == 0)
                            {
                                ImGui::TextColored(ImVec4(1, 0, 0, 1), "Target process not found!");
                            }
                            else
                            {
                                uintptr_t moduleBase = GetModuleBaseAddress(procId, L"eurotrucks2.exe");
                                if (moduleBase == 0)
                                {
                                    ImGui::TextColored(ImVec4(1, 0, 0, 1), "Failed to get module base address!");
                                }
                                else
                                {
                                    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procId);
                                    if (hProcess != NULL)
                                    {
                                        // Base address as specified in the CheatTable XML
                                        uintptr_t dynamicBaseAddr = moduleBase + 0x02705020;
                                        // Offsets from the cheat table, in order
                                        std::vector<unsigned int> brakeOffsets = { 0x0, 0x128, 0x1B8, 0xD4 };
                                        uintptr_t brakePowerAddr = FindDMAAddy(hProcess, dynamicBaseAddr, brakeOffsets);
                                        
                                        if (brakePowerAddr != 0)
                                        {
                                            if (!WriteFloat(hProcess, brakePowerAddr, brakePowerValue))
                                            {
                                                ImGui::TextColored(ImVec4(1, 0, 0, 1), "Failed to update Brake Power!");
                                            }
                                        }
                                        else
                                        {
                                            ImGui::TextColored(ImVec4(1, 0, 0, 1), "Failed to resolve Brake Power address!");
                                        }
                                        CloseHandle(hProcess);
                                    }
                                    else
                                    {
                                        ImGui::TextColored(ImVec4(1, 0, 0, 1), "Failed to open process!");
                                    }
                                }
                            }
                        }
                        // Engine RPM Slider
                        struct PointerChain {
                            uintptr_t baseOffset; // Base offset added to the module base for this chain
                            std::vector<unsigned int> offsets; // Pointer offsets for this chain
                        };

                        static float engineValue = 3500.0f;

                        if (ImGui::SliderFloat("Engine RPM Speedup", &engineValue, 3500.0f, 100000.0f, "%.1f"))
                        {
                            DWORD procId = GetProcessIdByName("eurotrucks2.exe");
                            if (procId == 0)
                            {
                                ImGui::TextColored(ImVec4(1, 0, 0, 1), "Target process not found!");
                            }
                            else
                            {
                                LPCWSTR moduleName = L"eurotrucks2.exe";
                                uintptr_t moduleBase = GetModuleBaseAddress(procId, moduleName);
                                if (moduleBase == 0)
                                {
                                    ImGui::TextColored(ImVec4(1, 0, 0, 1), "Failed to get module base address!");
                                }
                                else
                                {
                                    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procId);
                                    if (hProcess != NULL)
                                    {
                                        // Define pointer chains with their own modifiable base offsets
                                        std::vector<PointerChain> pointerChains = {
                                            { 0x029B1480, {0x3F8, 0x58, 0x10, 0x8, 0xA0, 0x330, 0x10} },
                                            { 0x029B1480, {0x3F8, 0x20, 0x10, 0x18B, 0xA0, 0x330, 0x10} },
                                            { 0x027CEB60, {0x0, 0x10, 0x8, 0x98, 0x100, 0x330, 0x10} },
                                            { 0x03080508, {0x38, 0x20, 0x628, 0x290, 0x100, 0x330, 0x10} },
                                            { 0x029B1480, {0x3F8, 0x4B, 0x10, 0x8, 0xA0, 0x330, 0x10} }
                                        };

                                        bool anyWriteSucceeded = false;
                                        for (const auto& chain : pointerChains)
                                        {
                                            // Compute the dynamic base address for this chain
                                            uintptr_t dynamicBaseAddr = moduleBase + chain.baseOffset;
                                            uintptr_t engineAddr = FindDMAAddy(hProcess, dynamicBaseAddr, chain.offsets);
                                            if (engineAddr != 0)
                                            {
                                                if (WriteFloat(hProcess, engineAddr, engineValue))
                                                {
                                                    anyWriteSucceeded = true;
                                                }
                                            }
                                        }

                                        if (!anyWriteSucceeded)
                                        {
                                            ImGui::TextColored(ImVec4(1, 0, 0, 1), "Failed to update engine value for all pointer chains!");
                                        }
                                        CloseHandle(hProcess);
                                    }
                                    else
                                    {
                                        ImGui::TextColored(ImVec4(1, 0, 0, 1), "Failed to open process!");
                                    }
                                }
                            }
                        }

                        if (engineValue == 3680.0f) ImGui::Text("Default");

                        // Money Input
                        static char inputBuffer[64] = "";
                        static int intValue = 0;

                        ImGui::InputText("Enter Money Value", inputBuffer, IM_ARRAYSIZE(inputBuffer));
                        ImGui::SameLine();

                        if (ImGui::Button("Apply"))
                        {
                            intValue = atoi(inputBuffer);
                            DWORD procId = GetProcessIdByName("eurotrucks2.exe");
                            if (procId == 0)
                            {
                                ImGui::TextColored(ImVec4(1, 0, 0, 1), "Target process not found!");
                            }
                            else
                            {
                                uintptr_t moduleBase = GetModuleBaseAddress(procId, L"eurotrucks2.exe");
                                if (moduleBase == 0)
                                {
                                    ImGui::TextColored(ImVec4(1, 0, 0, 1), "Failed to get module base address!");
                                }
                                else
                                {
                                    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procId);
                                    if (hProcess != NULL)
                                    {
                                        uintptr_t dynamicBaseAddr = moduleBase + 0x03004598;
                                        std::vector<unsigned int> offsets = { 0x10, 0x10 };
                                        uintptr_t targetAddr = FindDMAAddy(hProcess, dynamicBaseAddr, offsets);
                                        if (targetAddr != 0)
                                        {
                                            if (!WriteInt(hProcess, targetAddr, intValue))
                                            {
                                                ImGui::TextColored(ImVec4(1, 0, 0, 1), "Failed to update value!");
                                            }
                                        }
                                        else
                                        {
                                            ImGui::TextColored(ImVec4(1, 0, 0, 1), "Failed to resolve target address!");
                                        }
                                        CloseHandle(hProcess);
                                    }
                                    else
                                    {
                                        ImGui::TextColored(ImVec4(1, 0, 0, 1), "Failed to open process!");
                                    }
                                }
                            }
                        }

                        // Speed Mode Slider
                        static float selectedSpeed = 1.0f;
                        if (ImGui::SliderFloat("Speed Mode SERVER", &selectedSpeed, -1.0f, 10.0f, "%.1f"))
                        {
                            DWORD procId = GetProcessIdByName("eurotrucks2.exe");
                            if (!procId)
                            {
                                std::cout << "Process not found!" << std::endl;
                            }
                            else
                            {
                                uintptr_t moduleBase = GetModuleBaseAddress(procId, L"eurotrucks2.exe");
                                if (!moduleBase)
                                {
                                    std::cout << "Module base error!" << std::endl;
                                }
                                else
                                {
                                    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procId);
                                    if (!hProcess)
                                    {
                                        std::cout << "OpenProcess failed!" << std::endl;
                                    }
                                    else
                                    {
                                        std::vector<unsigned int> offsets = { 0x100, 0x050, 0x00, 0x550 };
                                        uintptr_t valueAddress = FindDMAAddy(hProcess, moduleBase + 0x0214E238, offsets);
                                        if (!valueAddress)
                                        {
                                            std::cout << "Failed to find dynamic address!" << std::endl;
                                        }
                                        else
                                        {
                                            if (!WriteProcessMemory(hProcess, reinterpret_cast<LPVOID>(valueAddress), 
                                                &selectedSpeed, sizeof(selectedSpeed), nullptr))
                                            {
                                                std::cout << "WriteProcessMemory failed!" << std::endl;
                                            }
                                            CloseHandle(hProcess);
                                        }
                                    }
                                }
                            }
                        }

                        // Teleport Feature
                        struct TeleportData {
                            uintptr_t offset;
                            float checkedValue;
                            float uncheckedValue;
                        };

                        static TeleportData teleportData[3] = {
                            { 0x067248, 3.401087432E19f, -0.187432766f },
                            { 0x0672A4, -6.228550473E-21f, 2.910466750E-9f },
                            { 0x0672A8, -1.35947988E-35f, -1.359486767E-35f }
                        };

                        static bool teleportEnabled = false;
                        if (ImGui::Checkbox("Enable Teleport | TMP SERVER", &teleportEnabled))
                        {
                            DWORD procId = GetProcessIdByName("eurotrucks2.exe");
                            if (procId == 0)
                            {
                                ImGui::TextColored(ImVec4(1, 0, 0, 1), "Target process not found!");
                            }
                            else
                            {
                                uintptr_t moduleBase = GetModuleBaseAddress(procId, L"eurotrucks2.exe");
                                if (moduleBase == 0)
                                {
                                    ImGui::TextColored(ImVec4(1, 0, 0, 1), "Failed to get module-base address!");
                                }
                                else
                                {
                                    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procId);
                                    if (hProcess)
                                    {
                                        bool allSuccess = true;
                                        for (int i = 0; i < 3; i++)
                                        {
                                            uintptr_t address = moduleBase + teleportData[i].offset;
                                            float value = teleportEnabled ? teleportData[i].checkedValue : teleportData[i].uncheckedValue;
                                            if (!WriteFloat(hProcess, address, value))
                                            {
                                                allSuccess = false;
                                                ImGui::TextColored(ImVec4(1, 0, 0, 1), "Failed to write at: 0x%llX", address);
                                            }
                                        }
                                        if (allSuccess)
                                        {
                                            ImGui::TextColored(ImVec4(0, 1, 0, 1), "Teleport %s!", teleportEnabled ? "enabled" : "disabled");
                                        }
                                        CloseHandle(hProcess);
                                    }
                                    else
                                    {
                                        ImGui::TextColored(ImVec4(1, 0, 0, 1), "Failed to open process!");
                                    }
                                }
                            }
                        }

                        ImVec4 statusColor = teleportEnabled ? ImVec4(0.0f, 1.0f, 0.0f, 1.0f) : ImVec4(1.0f, 0.0f, 0.0f, 1.0f); 
                        ImGui::TextColored(statusColor, "Teleport Status: %s", teleportEnabled ? "Enabled" : "Disabled");

                        // Dashboard Controls
                        static bool patchAppliedK = false;
                        if (ImGui::Checkbox("Passenger View Mode (Adjust the distance using for settings in-game)", &patchAppliedK))
                        {
                            TogglePatchXExternal(patchAppliedK);
                        }

                        static bool patchApplied7 = false;
                        if (ImGui::Checkbox("[RED] Dashboard Color", &patchApplied7))
                        {
                            ColorAdjustedExternal(patchApplied7);
                        }

                        static bool patchApplied8 = false;
                        if (ImGui::Checkbox("[PURPLE] Dashboard Color", &patchApplied8))
                        {
                            ColorAdjustedExternalD(patchApplied8);
                        }

                        static bool patchApplied9 = false;
                        if (ImGui::Checkbox("[FLASHY RED] Dashboard Color", &patchApplied9))
                        {
                            ColorAdjustRedF(patchApplied9);
                        }

                        // Vertical Seat Position Slider
                        static float highValue = 1.0f;
                        if (ImGui::SliderFloat("Vertical Seat Position", &highValue, -10.0f, 40.0f, "%.1f"))
                        {
                            DWORD procId = GetProcessIdByName("eurotrucks2.exe");
                            if (procId == 0)
                            {
                                ImGui::TextColored(ImVec4(1, 0, 0, 1), "Target process not found!");
                            }
                            else
                            {
                                uintptr_t moduleBase = GetModuleBaseAddress(procId, L"eurotrucks2.exe");
                                if (moduleBase == 0)
                                {
                                    ImGui::TextColored(ImVec4(1, 0, 0, 1), "Failed to get module base address!");
                                }
                                else
                                {
                                    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procId);
                                    if (hProcess == NULL)
                                    {
                                        ImGui::TextColored(ImVec4(1, 0, 0, 1), "Failed to open process!");
                                    }
                                    else
                                    {
                                        uintptr_t dynamicBaseAddr = moduleBase + 0x02777290;
                                        std::vector<unsigned int> offsets = { 0x08, 0x10, 0x0, 0x28, 0x8, 0x35C };
                                        uintptr_t seatPositionAddr = FindDMAAddy(hProcess, dynamicBaseAddr, offsets);
                                        if (seatPositionAddr != 0)
                                        {
                                            if (!WriteFloat(hProcess, seatPositionAddr, highValue))
                                            {
                                                ImGui::TextColored(ImVec4(1, 0, 0, 1), "Failed to update Vertical Seat Position!");
                                            }
                                        }
                                        else
                                        {
                                            ImGui::TextColored(ImVec4(1, 0, 0, 1), "Failed to resolve Seat Position address!");
                                        }
                                        CloseHandle(hProcess);
                                    }
                                }
                            }
                        }

                        ImVec2 windowContentSize = ImGui::GetWindowContentRegionMax();
                        float donationButtonWidth = 150.0f;
                        float donationButtonHeight = 30.0f;

                        ImGui::SetCursorPos(ImVec2(windowContentSize.x - donationButtonWidth, windowContentSize.y - donationButtonHeight));
                        if (ImGui::Button("Buy me a Coffee", ImVec2(donationButtonWidth, donationButtonHeight)))
                        {
                            ShellExecuteA(NULL, "open", "https://ko-fi.com/dizcatoff", NULL, NULL, SW_SHOWNORMAL);
                        }
                        // Distance Seat Position Slider
                        static float distValue = 0.1f;
                        if (ImGui::SliderFloat("Distance Seat Position", &distValue, -10.0f, 40.0f, "%.1f"))
                        {
                            DWORD procId = GetProcessIdByName("eurotrucks2.exe");
                            if (procId == 0)
                            {
                                ImGui::TextColored(ImVec4(1, 0, 0, 1), "Target process not found!");
                            }
                            else
                            {
                                uintptr_t moduleBase = GetModuleBaseAddress(procId, L"eurotrucks2.exe");
                                if (moduleBase == 0)
                                {
                                    ImGui::TextColored(ImVec4(1, 0, 0, 1), "Failed to get module base address!");
                                }
                                else
                                {
                                    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procId);
                                    if (hProcess == NULL)
                                    {
                                        ImGui::TextColored(ImVec4(1, 0, 0, 1), "Failed to open process!");
                                    }
                                    else
                                    {
                                        uintptr_t dynamicBaseAddr = moduleBase + 0x07CC9C0;
                                        std::vector<unsigned int> offsets = { 0x0, 0x8, 0x5B8, 0x1B8, 0x308, 0x378, 0x340 };
                                        uintptr_t distValueAddr = FindDMAAddy(hProcess, dynamicBaseAddr, offsets);
                                        if (distValueAddr != 0)
                                        {
                                            if (!WriteFloat(hProcess, distValueAddr, distValue))
                                            {
                                                ImGui::TextColored(ImVec4(1, 0, 0, 1), "Failed to update Distance Seat Position!");
                                            }
                                        }
                                        else
                                        {
                                            ImGui::TextColored(ImVec4(1, 0, 0, 1), "Failed to resolve Seat Position address!");
                                        }
                                        CloseHandle(hProcess);
                                    }
                                }
                            }
                        }
                    }
                                        // Info Tab (case 2)
                    case 2:
                    {
                        ImGui::Text("Ghosty Lite V2");
                        ImGui::Separator();
                        ImGui::Text("Contact Information:");
                        
                        // Discord Button
                        ImGui::Text("Discord:");
                        ImGui::SameLine();
                        if (ImGui::Button("https://discord.gg/MediaHyMUrv"))
                        {
                            ShellExecuteA(NULL, "open", "https://discord.gg/MediaHyMUrv", NULL, NULL, SW_SHOWNORMAL);
                        }
                        
                        // Website Button
                        ImGui::Text("Website:");
                        ImGui::SameLine();
                        if (ImGui::Button("https://mysticm7.github.io/Ghosty/"))
                        {
                            ShellExecuteA(NULL, "open", "https://mysticm7.github.io/Ghosty/", NULL, NULL, SW_SHOWNORMAL);
                        }
                        
                        // Donation Button
                        if (ImGui::Button("Buy me a Coffee"))
                        {
                            ShellExecuteA(NULL, "open", "https://ko-fi.com/dizcatoff", NULL, NULL, SW_SHOWNORMAL);
                        }
                        
                        // Credits
                        ImGui::Text("Credits:");
                        ImGui::BulletText("Developer: Dizcat");
                        ImGui::BulletText("Special Thanks: Toxix, Abotin, exobyte, Asico, Nameless");
                        break;
                    }

                    ImGui::PopFont();
                    ImGui::End();
                    }
                    else 
                    {
                        exit(0);
                    }

                    ImGui::EndFrame();
                    g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
                    g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
                    g_pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
                    g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, NULL, 1.0f, 0);
                    if (g_pd3dDevice->BeginScene() >= 0)
                    {
                        ImGui::Render();
                        ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
                        g_pd3dDevice->EndScene();
                    }

                    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
                    {
                        ImGui::UpdatePlatformWindows();
                        ImGui::RenderPlatformWindowsDefault();
                    }

                    HRESULT result = g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
                    if (result == D3DERR_DEVICELOST && g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
                        ResetDevice();

                    CleanupDeviceD3D();
                    UnregisterClass(wc.lpszClassName, wc.hInstance);

int main()
{
    if (AllocConsole())
    {
        FILE* pCout;
        FILE* pCin;
        FILE* pCerr;
        freopen_s(&pCout, "CONOUT$", "w", stdout);
        freopen_s(&pCin, "CONIN$", "r", stdin);
        freopen_s(&pCerr, "CONOUT$", "w", stderr);
    }

    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(console, 1);
    ShowWindow(GetConsoleWindow(), SW_HIDE);
}