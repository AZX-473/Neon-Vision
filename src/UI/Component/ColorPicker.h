#pragma once

#include <iostream>
#include <Windows.h>
#include <imgui.h>

class ColorPicker {
public:
	static ImVec4 GetColorFromPos(POINT pos);

	static void DrawPickerColor(POINT pos, ImVec4 color, bool showblock, bool showRGBn, float blockwl, ImVec4 TextCol);
};