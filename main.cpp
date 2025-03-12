#include "includes.h"
#include "font.h"
#include "mem.h"
#include "offsets.h"
#include <Windows.h>
#include <cstdint>
#include <chrono>
#include <vector>
#include <thread>
#include <atomic>
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

Present oPresent;
HWND window = NULL;
WNDPROC oWndProc;
ID3D11Device* pDevice = NULL;
ID3D11DeviceContext* pContext = NULL;
ID3D11RenderTargetView* mainRenderTargetView;

//----------------------------------------
static bool toggleLoopCheckbox = false;
static bool toggleLoopCheckbox2 = false;
// Bound to our new ImGui checkbox.
static std::atomic<bool> toggleLoopThreadActive(false);
static std::atomic<bool> toggleLoopThreadActive2(false);    
//----------------------------------------
//eurotrucks2.exe+31BF310 - E8 00000000           - call eurotrucks2.exe+31BF315
// eurotrucks2.exe+1706D3A - 75 34                 - jne eurotrucks2.exe+1706D70 noclip





void TogglePatch(bool applyNOP)
{
	uintptr_t moduleBase = reinterpret_cast<uintptr_t>(GetModuleHandle(nullptr));
	if (moduleBase == 0) {
		return;
	}

	uintptr_t targetAddress = moduleBase + 0x4B97B3;

	const size_t patchSize = 5;
	unsigned char patchBytes[patchSize];

	if (applyNOP)
	{
		memset(patchBytes, 0x90, patchSize);
	}
	else
	{
		unsigned char originalBytes[patchSize] = { 0xF3, 0x0F, 0x11, 0x4A, 0x08 }; //eurotrucks2.exe+4B97B3 - F3 0F11 4A 08         - movss [rdx+08],xmm1
		memcpy(patchBytes, originalBytes, patchSize);
	}

	DWORD oldProtect;
	if (VirtualProtect(reinterpret_cast<LPVOID>(targetAddress), patchSize, PAGE_EXECUTE_READWRITE, &oldProtect))
	{
		memcpy(reinterpret_cast<void*>(targetAddress), patchBytes, patchSize);

		VirtualProtect(reinterpret_cast<LPVOID>(targetAddress), patchSize, oldProtect, &oldProtect);
	}
	else
	{
	}
}


void TogglePatch2(bool applyNOP2)
{
	uintptr_t moduleBase = reinterpret_cast<uintptr_t>(GetModuleHandle(nullptr));
	if (moduleBase == 0) {
		return;
	}

	uintptr_t targetAddress2 = moduleBase + 0x54D013;


	const size_t patchSize2 = 8;

	static unsigned char originalBytes2[patchSize2] = { 0 };
	static bool originalBytesStored = false;

	unsigned char patchBytes2[patchSize2];

	if (!originalBytesStored)
	{
		memcpy(originalBytes2, reinterpret_cast<void*>(targetAddress2), patchSize2);
		originalBytesStored = true;
	}

	if (applyNOP2)
	{
		memset(patchBytes2, 0x90, patchSize2);
	}
	else
	{
		memcpy(patchBytes2, originalBytes2, patchSize2);
	}

	DWORD oldProtect;
	if (VirtualProtect(reinterpret_cast<LPVOID>(targetAddress2), patchSize2, PAGE_EXECUTE_READWRITE, &oldProtect))
	{
		memcpy(reinterpret_cast<void*>(targetAddress2), patchBytes2, patchSize2);
		FlushInstructionCache(GetCurrentProcess(), reinterpret_cast<LPCVOID>(targetAddress2), patchSize2);
		VirtualProtect(reinterpret_cast<LPVOID>(targetAddress2), patchSize2, oldProtect, &oldProtect);
	}
}


void TogglePatch3Loop()
{
	HMODULE hModule = GetModuleHandle("core_ets2mp.dll");
	if (!hModule)
		return;

	uintptr_t modulebase2 = reinterpret_cast<uintptr_t>(hModule);
	uintptr_t targetAddress = modulebase2 + 0x163158;


	const size_t patchSize = 10;

	static unsigned char originalBytes[patchSize];
	static bool originalBytesStored = false;
	if (!originalBytesStored)
	{
		memcpy(originalBytes, reinterpret_cast<void*>(targetAddress), patchSize);
		originalBytesStored = true;
	}


	unsigned char patchBytes[patchSize];
	memcpy(patchBytes, originalBytes, patchSize);
	uint32_t newImmediate = 0x447A0000;
	memcpy(&patchBytes[6], &newImmediate, sizeof(newImmediate));

	while (toggleLoopThreadActive2)
	{
		DWORD oldProtect;
		if (VirtualProtect(reinterpret_cast<LPVOID>(targetAddress), patchSize, PAGE_EXECUTE_READWRITE, &oldProtect))
		{
			if (toggleLoopCheckbox2)
			{
				memcpy(reinterpret_cast<void*>(targetAddress), patchBytes, patchSize);
			}
			else
			{
				memcpy(reinterpret_cast<void*>(targetAddress), originalBytes, patchSize);
			}
			FlushInstructionCache(GetCurrentProcess(), reinterpret_cast<LPCVOID>(targetAddress), patchSize);
			VirtualProtect(reinterpret_cast<LPVOID>(targetAddress), patchSize, oldProtect, &oldProtect);
		}
		Sleep(500); 
	}

	DWORD finalOldProtect;
	if (VirtualProtect(reinterpret_cast<LPVOID>(targetAddress), patchSize, PAGE_EXECUTE_READWRITE, &finalOldProtect))
	{
		memcpy(reinterpret_cast<void*>(targetAddress), originalBytes, patchSize);
		FlushInstructionCache(GetCurrentProcess(), reinterpret_cast<LPCVOID>(targetAddress), patchSize);
		VirtualProtect(reinterpret_cast<LPVOID>(targetAddress), patchSize, finalOldProtect, &finalOldProtect);
	}
}

void TogglePatch3(bool applyPatch)
{
	toggleLoopCheckbox2 = applyPatch;

	if (applyPatch && !toggleLoopThreadActive2)
	{
		toggleLoopThreadActive2 = true;
		std::thread(TogglePatch3Loop).detach();
	}
	else if (!applyPatch && toggleLoopThreadActive2)
	{
		toggleLoopThreadActive2 = false;
	}
}

void TogglePatch4(bool applyNOP)
{
	uintptr_t moduleBase = reinterpret_cast<uintptr_t>(GetModuleHandle(nullptr));
	if (moduleBase == 0) {
		return;
	}

	uintptr_t targetAdress4 = moduleBase + 0x4B9735;

	const size_t patchSize4 = 4;
	unsigned char patchBytes4[patchSize4];

	if (applyNOP)
	{
		memset(patchBytes4, 0x90, patchSize4);
	}
	else
	{
		unsigned char originalBytes4[patchSize4] = { 0xF3, 0x0F, 0x11, 0x0A }; //eurotrucks2.exe + 4B9735 - F3 0F11 0A - movss[rdx], xmm1
		memcpy(patchBytes4, originalBytes4, patchSize4);
	}

	DWORD oldProtect;
	if (VirtualProtect(reinterpret_cast<LPVOID>(targetAdress4), patchSize4, PAGE_EXECUTE_READWRITE, &oldProtect))
	{
		memcpy(reinterpret_cast<void*>(targetAdress4), patchBytes4, patchSize4);

		VirtualProtect(reinterpret_cast<LPVOID>(targetAdress4), patchSize4, oldProtect, &oldProtect);
	}
	else
	{

	}
}

void TogglePatch5(bool applyNOP)
{
	uintptr_t moduleBase = reinterpret_cast<uintptr_t>(GetModuleHandle(nullptr));
	if (moduleBase == 0) {
		return;
	}

	uintptr_t targetAddress5 = moduleBase + 0x4B96F4;

	const size_t patchsize5 = 5;
	unsigned char patchBytes5[patchsize5];

	if (applyNOP)
	{
		memset(patchBytes5, 0x90, patchsize5);
	}
	else
	{
		unsigned char originalBytes5[patchsize5] = { 0xF3, 0x0F, 0x11, 0x4A, 0xFC }; //eurotrucks2.exe + 4B96F4 - F3 0F11 4A FC - movss[rdx - 04], xmm1

		memcpy(patchBytes5, originalBytes5, patchsize5);
	}

	DWORD oldProtect;
	if (VirtualProtect(reinterpret_cast<LPVOID>(targetAddress5), patchsize5, PAGE_EXECUTE_READWRITE, &oldProtect))
	{
		memcpy(reinterpret_cast<void*>(targetAddress5), patchBytes5, patchsize5);

		VirtualProtect(reinterpret_cast<LPVOID>(targetAddress5), patchsize5, oldProtect, &oldProtect);
	}
	else
	{
	}
}

unsigned char g_OriginalBytes6[6] = { 0 };
bool g_OriginalBytes6Stored = false;

void TogglePatch6(bool applyJe)
{
	uintptr_t moduleBase = reinterpret_cast<uintptr_t>(GetModuleHandle(nullptr));
	if (!moduleBase)
		return;

	uintptr_t targetAddress = moduleBase + 0x1706D3A;
	const size_t patchSize = 6;

	if (!g_OriginalBytes6Stored)
	{
		memcpy(g_OriginalBytes6, reinterpret_cast<void*>(targetAddress), patchSize);
		g_OriginalBytes6Stored = true;
	}

	unsigned char patchBytes[patchSize];

	if (applyJe)
	{
		memcpy(patchBytes, g_OriginalBytes6, patchSize);
		patchBytes[0] = 0x74;
	}
	else
	{
		memcpy(patchBytes, g_OriginalBytes6, patchSize);
	}

	DWORD oldProtect;
	if (VirtualProtect(reinterpret_cast<LPVOID>(targetAddress), patchSize, PAGE_EXECUTE_READWRITE, &oldProtect))
	{
		memcpy(reinterpret_cast<void*>(targetAddress), patchBytes, patchSize);
		VirtualProtect(reinterpret_cast<LPVOID>(targetAddress), patchSize, oldProtect, &oldProtect);
	}
}




void InitImGui()
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX11_Init(pDevice, pContext);

	ImGuiStyle& style = ImGui::GetStyle();

	style.Alpha = 1.0f;
	style.WindowPadding = ImVec2(8.0f, 8.0f);
	style.WindowRounding = 10.0f;
	style.WindowBorderSize = 0.0f;
	style.WindowMinSize = ImVec2(30.0f, 30.0f);
	style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
	style.WindowMenuButtonPosition = ImGuiDir_Right;
	style.ChildRounding = 5.0f;
	style.ChildBorderSize = 1.0f;
	style.PopupRounding = 10.0f;
	style.PopupBorderSize = 0.0f;
	style.FramePadding = ImVec2(5.0f, 3.5f);
	style.FrameRounding = 5.0f;
	style.FrameBorderSize = 0.0f;
	style.ItemSpacing = ImVec2(5.0f, 4.0f);
	style.ItemInnerSpacing = ImVec2(5.0f, 5.0f);
	style.IndentSpacing = 5.0f;
	style.ColumnsMinSpacing = 5.0f;
	style.ScrollbarSize = 15.0f;
	style.ScrollbarRounding = 9.0f;
	style.GrabMinSize = 15.0f;
	style.GrabRounding = 5.0f;
	style.TabRounding = 5.0f;
	style.TabBorderSize = 0.0f;
	style.ColorButtonPosition = ImGuiDir_Right;
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
	style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

	style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(1.0f, 1.0f, 1.0f, 0.3605149984359741f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.09803921729326248f, 0.09803921729326248f, 0.09803921729326248f, 1.0f);
	style.Colors[ImGuiCol_ChildBg] = ImVec4(1.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.09803921729326248f, 0.09803921729326248f, 0.09803921729326248f, 1.0f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.501960813999176f, 0.3019607961177826f, 1.0f, 0.5490196347236633f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.1568627506494522f, 0.1568627506494522f, 0.1568627506494522f, 1.0f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.3803921639919281f, 0.4235294163227081f, 0.572549045085907f, 0.5490196347236633f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.501960813999176f, 0.3019607961177826f, 1.0f, 0.5490196347236633f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.09803921729326248f, 0.09803921729326248f, 0.09803921729326248f, 1.0f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.09803921729326248f, 0.09803921729326248f, 0.09803921729326248f, 1.0f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.2588235437870026f, 0.2588235437870026f, 0.2588235437870026f, 0.0f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.1568627506494522f, 0.1568627506494522f, 0.1568627506494522f, 0.0f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.1568627506494522f, 0.1568627506494522f, 0.1568627506494522f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.2352941185235977f, 0.2352941185235977f, 0.2352941185235977f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.294117659330368f, 0.294117659330368f, 0.294117659330368f, 1.0f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.501960813999176f, 0.3019607961177826f, 1.0f, 0.5490196347236633f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.501960813999176f, 0.3019607961177826f, 1.0f, 0.5490196347236633f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.501960813999176f, 0.3019607961177826f, 1.0f, 0.5490196347236633f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.501960813999176f, 0.3019607961177826f, 1.0f, 0.5490196347236633f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.501960813999176f, 0.3019607961177826f, 1.0f, 0.5490196347236633f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.501960813999176f, 0.3019607961177826f, 1.0f, 0.5490196347236633f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.501960813999176f, 0.3019607961177826f, 1.0f, 0.5490196347236633f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.501960813999176f, 0.3019607961177826f, 1.0f, 0.5490196347236633f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.501960813999176f, 0.3019607961177826f, 1.0f, 0.5490196347236633f);
	style.Colors[ImGuiCol_Separator] = ImVec4(0.501960813999176f, 0.3019607961177826f, 1.0f, 0.5490196347236633f);
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.501960813999176f, 0.3019607961177826f, 1.0f, 0.5490196347236633f);
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.501960813999176f, 0.3019607961177826f, 1.0f, 0.5490196347236633f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.501960813999176f, 0.3019607961177826f, 1.0f, 0.5490196347236633f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.501960813999176f, 0.3019607961177826f, 1.0f, 0.5490196347236633f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.501960813999176f, 0.3019607961177826f, 1.0f, 0.5490196347236633f);
	style.Colors[ImGuiCol_Tab] = ImVec4(0.501960813999176f, 0.3019607961177826f, 1.0f, 0.5490196347236633f);
	style.Colors[ImGuiCol_TabHovered] = ImVec4(0.501960813999176f, 0.3019607961177826f, 1.0f, 0.5490196347236633f);
	style.Colors[ImGuiCol_TabActive] = ImVec4(0.501960813999176f, 0.3019607961177826f, 1.0f, 0.5490196347236633f);
	style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.0f, 0.4509803950786591f, 1.0f, 0.0f);
	style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.1333333402872086f, 0.2588235437870026f, 0.4235294163227081f, 0.0f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.294117659330368f, 0.294117659330368f, 0.294117659330368f, 1.0f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.501960813999176f, 0.3019607961177826f, 1.0f, 0.5490196347236633f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.501960813999176f, 0.3019607961177826f, 1.0f, 0.5490196347236633f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.7372549176216125f, 0.6941176652908325f, 0.886274516582489f, 0.5490196347236633f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.501960813999176f, 0.3019607961177826f, 1.0f, 0.5490196347236633f);
	style.Colors[ImGuiCol_DragDropTarget] = ImVec4(1.0f, 1.0f, 0.0f, 0.8999999761581421f);
	style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 0.699999988079071f);
	style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.2000000029802322f);
	style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.3499999940395355f);


	ImFontConfig font;
	font.FontDataOwnedByAtlas = false;
	io.Fonts->AddFontFromMemoryTTF((void*)rawData, sizeof(rawData), 18.5f, &font);


}



LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	if (true && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}



bool init = false;
HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	if (!init)
	{
		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice)))
		{
			pDevice->GetImmediateContext(&pContext);
			DXGI_SWAP_CHAIN_DESC sd;
			pSwapChain->GetDesc(&sd);
			window = sd.OutputWindow;
			ID3D11Texture2D* pBackBuffer;
			pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
			pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
			pBackBuffer->Release();
			oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
			InitImGui();
			init = true;
		}

		else
			return oPresent(pSwapChain, SyncInterval, Flags);
	}

	if ((GetAsyncKeyState(VK_CONTROL) & 0x8000) && (GetAsyncKeyState('P') & 1)) {
		static auto lastPressTime = std::chrono::steady_clock::now() - std::chrono::seconds(1);

		auto currentTime = std::chrono::steady_clock::now();
		std::chrono::duration<double> elapsed = currentTime - lastPressTime;

		if (elapsed.count() >= 0.3) {
			globals::show = !globals::show;
			lastPressTime = currentTime;
		}
	}

	static bool fakeLagEnabled = false;

	if (GetAsyncKeyState('A') & 0x8000)
	{
		fakeLagEnabled = true;
	}
	else
	{
		fakeLagEnabled = false;
	}

	globals::fakeLag = fakeLagEnabled ? 1.0f : 0.0f;
	*(float*)mem::FindAdress(moduleBase + 0x020AB388, { 0x8, 0x30, 0x8, 0x80, 0x178, 0x4C8, 0x758 }) = globals::fakeLag;

	static bool checkbox_state2 = false;
	static bool checkbox_state3 = false; 

	static bool patchApplied6 = false;
	static bool nKeyWasDown = false;

	static bool ctrlNWasDown = false;

	if ((GetAsyncKeyState(VK_CONTROL) & 0x8000) && (GetAsyncKeyState('N') & 0x8000))
	{
		if (!ctrlNWasDown) 
		{
			patchApplied6 = !patchApplied6;
			TogglePatch6(patchApplied6);
		}
		ctrlNWasDown = true;
	}
	else
	{
		ctrlNWasDown = false;
	}

	if (globals::show)
	{


		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		auto start = std::chrono::system_clock::now();
		auto end = std::chrono::system_clock::now();
		std::chrono::duration<double> elapsed_seconds = end - start;
		std::time_t end_time = std::chrono::system_clock::to_time_t(end);

		ImGui::SetNextWindowPos({ 15, 15 });
		ImGui::SetNextWindowSize({ 430, 50 });

		ImGui::Begin("Watermark", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove);
		ImGui::Text("youtube.com/@CollideGhost | %s", std::ctime(&end_time));
		ImGui::End();

		ImGuiIO& io = ImGui::GetIO();
		ImVec2 windowSize = ImVec2(900, 450);

		float offsetX = 50.0f;
		ImVec2 centerPos = ImVec2((io.DisplaySize.x - windowSize.x) * 0.5f + offsetX,
			(io.DisplaySize.y - windowSize.y) * 0.5f);

		ImGui::SetNextWindowSize(windowSize);
		ImGui::SetNextWindowPos(centerPos, ImGuiCond_FirstUseEver);


		ImGui::Begin("Ghosty Lite - Online [CTRL+P]", NULL, ImGuiWindowFlags_NoResize);
		       ImGui::GetIO().MouseDrawCursor = true;

		{
			ImGui::SameLine();
			if (ImGui::Button(("Main [LOAD GAME FIRST]"), ImVec2(415, 30)))
			{
				globals::tab = 0;
			}
			ImGui::SameLine();
			if (ImGui::Button(("Misc"), ImVec2(415, 30)))
			{
				globals::tab = 1;
			}
		}

		ImGui::Separator();

		if (globals::tab == 0)
		{
			static int customKey = 'A';
			static bool waitingForKey = false;

			if (ImGui::Button(waitingForKey ? "Press any key..." : "Change Keybind"))
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

			static bool fakeLagEnabled = false;
			if (GetAsyncKeyState(customKey) & 0x8000)
			{
				fakeLagEnabled = true;
			}
			else
			{
				fakeLagEnabled = false;
			}

			// Convert the key code to a string representation
			char keybindText[32];
			sprintf(keybindText, "Fake Lag TMP (Hold %c) SERVER", customKey);

			ImGui::Checkbox(keybindText, &fakeLagEnabled);

			globals::fakeLag = fakeLagEnabled ? 1.0f : 0.0f;
			*(float*)mem::FindAdress(moduleBase + 0x020AB388, { 0x8, 0x30, 0x8, 0x80, 0x178, 0x4C8, 0x758 }) = globals::fakeLag;


			static int selectedSpeed = 0;
			const char* speedOptions[] = { "Default Speed", "Low Speed Hack", "(Best) Speed Hack", "Speed Hack", "Insane Speed Hack" };
			float speedValues[] = { 1.0f, 2.0f, 7.0f, 10.0f, 20.0f };

			if (ImGui::Combo("Speed Mode SERVER", &selectedSpeed, speedOptions, IM_ARRAYSIZE(speedOptions)))
			{
				float selectedValue = speedValues[selectedSpeed];
				*(float*)mem::FindAdress(moduleBase + 0x020AB388, { 0x38, 0x0, 0xC8, 0x30, 0x188, 0x398, 0x554 }) = selectedValue;
			}

			static float frontValue = 1.0f;
			if (ImGui::SliderFloat("Front Suspension SERVER", &frontValue, 0.1f, 10.0f, "%.1f"))
			{
				float* frontSuspensionPtr = (float*)mem::FindAdress(
					moduleBase + 0x026C8DB0,
					{ 0x2A8, 0x10, 0x10, 0x28, 0x8, 0x20, 0x394 }
				);

				if (frontSuspensionPtr)
				{
					*frontSuspensionPtr = frontValue;
				}
				else
				{
					ImGui::TextColored(ImVec4(1, 0, 0, 1), "Failed to update Front Suspension!");
				}
			}

			static float rearValue = 1.0f;
			if (ImGui::SliderFloat("Rear Suspension SERVER", &rearValue, 0.1f, 10.0f, "%.1f"))
			{
				float* rearSuspensionPtr = (float*)mem::FindAdress(
					moduleBase + 0x026777C8,
					{ 0x130, 0x8, 0x0, 0x98, 0x48, 0x8, 0x398 }
				);

				if (rearSuspensionPtr)
				{
					*rearSuspensionPtr = rearValue;
				}
				else
				{
					ImGui::TextColored(ImVec4(1, 0, 0, 1), "Failed to update Rear Suspension!");
				}
			}



			ImGui::Checkbox("Headlight Effect SERVER", &toggleLoopCheckbox);


			if (toggleLoopCheckbox && !toggleLoopThreadActive.load())
			{
				toggleLoopThreadActive.store(true);
				std::thread([](uintptr_t moduleBase, bool& toggleLoopCheckbox, std::atomic<bool>& toggleLoopThreadActive) {
					while (toggleLoopCheckbox)
					{
						float* loopValue = (float*)mem::FindAdress(moduleBase + 0x020AADE0,
							{ 0xF78, 0x140 });
						if (loopValue)
						{
							*loopValue = 0.0f;
						}
						Sleep(200); 

						if (!toggleLoopCheckbox)
							break;


						if (loopValue)
						{
							*loopValue = 1.0f;
						}
						Sleep(500);  
					}
					toggleLoopThreadActive.store(false);
					},
					reinterpret_cast<uintptr_t>(GetModuleHandle(nullptr)),
					std::ref(toggleLoopCheckbox),
					std::ref(toggleLoopThreadActive)
				).detach();
			}

			static bool patchApplied = false;
			if (ImGui::Checkbox("Freeze Reverse Light   |  CLIENT  [Reverse First]", &patchApplied))
			{
				TogglePatch(patchApplied);
			}

			static bool patchApplied2 = false;
			if (ImGui::Checkbox("Super Light Mod   |   CLIENT [Beacon ON First]", &patchApplied2))
			{
				TogglePatch2(patchApplied2);
			}

			static bool patchApplied3 = false;
			if (ImGui::Checkbox("Speed Unlocker   |  SERVER [Out-City TMP]", &patchApplied3))
			{
				TogglePatch3(patchApplied3);
			}

			static bool patchApplied4 = false;
			if (ImGui::Checkbox("Freeze Right Turn Light   |  CLIENT [Right Light ON First]", &patchApplied4))
			{
				TogglePatch4(patchApplied4);
			}

			static bool patchApplied5 = false;
			if (ImGui::Checkbox("Freeze Left Turn Light   |   CLIENT [Left Light ON First]", &patchApplied5))
			{
				TogglePatch5(patchApplied5);
			}

			static bool isChecked = false; 

			if (ImGui::Checkbox("Freeze Hazard light   | SERVER [TMP] ", &isChecked))
			{
				float* valueAddress = (float*)mem::FindAdress(moduleBase2 + 0x00FCDFE0, { 0x30, 0x10, 0x18, 0x98, 0x30, 0x20, 0xB0 });
				if (valueAddress)
				{
					*valueAddress = isChecked ? 1000.0f : 0.3f;
				}
			}



			if (ImGui::Checkbox("Noclip [Ctrl+N] |  SERVER", &patchApplied6))
			{
				TogglePatch6(patchApplied6);
			}



		}


		


		if (globals::tab == 1)
		{
			ImGui::Checkbox("Performance Stats", &checkbox_state2);

			static float moneyValue = 10000.0f; // Default value, change as needed
			ImGui::SliderFloat("Money Value", &moneyValue, 0.0f, 1000000000.0f, "%.0f");

			float* targetValue = (float*)mem::FindAdress(moduleBase + 0x02F2B730, { 0x110 });
			if (targetValue)
			{
				*targetValue = checkbox_state2 ? 1.0f : 0.0f;
			}

			// Modify money value
			float* moneyAddress = (float*)mem::FindAdress(moduleBase + 0x02F57B88, { 0x10, 0x10 });
			if (moneyAddress)
			{
				*moneyAddress = moneyValue;
			}
		}

		ImGui::End();
		ImGui::Render();
		pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}

	return oPresent(pSwapChain, SyncInterval, Flags);

}

DWORD WINAPI MainThread(LPVOID lpReserved)
{
	bool init_hook = false;
	do
	{
		if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success)
		{
			kiero::bind(8, (void**)&oPresent, hkPresent);
			init_hook = true;
		}
	} while (!init_hook);
	return TRUE;
}

BOOL WINAPI DllMain(HMODULE hMod, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hMod);
		CreateThread(nullptr, 0, MainThread, hMod, 0, nullptr);
		break;
	case DLL_PROCESS_DETACH:
		kiero::shutdown();
		break;
	}
	return TRUE;
}