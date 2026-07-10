#pragma once

#include <iostream>
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include <imgui.h>

class PointerLine {
public:
	static void i_ClampedLerp(POINT MousePOS, float l_Step, float Threshold);

	static void i_Bisectio(POINT MousePOS, float Threshold);

	static void i_None(POINT MousePOS);

	static void DrawPointerLine(POINT MousePOS, ImU32 RECTColor, float LineWidth, float Offset);

private:
	static float h_left, h_right, w_top, w_bottom;
};