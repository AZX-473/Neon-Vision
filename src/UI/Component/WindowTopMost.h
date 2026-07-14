#pragma once
#include <imgui.h>
#include <Windows.h>
#include <string>
#include <vector>

// 窗口信息结构体
struct WindowInfo {
	HWND hwnd;
	std::string title;
	bool isTopMost;
	int id;
};

class WindowTopMost {
public:
	static std::vector<WindowInfo> GetAllWindow();

	static bool IsWindowTopMost(HWND hwnd);

	static std::vector<WindowInfo> AllWindow;

	static void ToggleTopMost(HWND hwnd);
};