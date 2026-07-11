#pragma once

#include <iostream>
#include <Windows.h>
#include <imgui.h>
#include <string>

class WindowName {
public:
	// Set show string template. Use "${WindowName}" in 'fmt' to insert the actual window name.
	static void SetShowStr(const std::string& fmt, const std::string& windowName);

	// Draw the string near MousePOS with offset and packed ImU32 color.
	static void DrawWindowName(const POINT& MousePOS, ImU32 StrColor, float off_X, float off_Y);

	// Convenience: set show string directly
	static void SetShowStrDirect(const std::string& text);

private:
	static std::string showstr;
};