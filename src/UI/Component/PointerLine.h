#pragma once

#include <iostream>
#include <Windows.h>
#include <imgui.h>

class PointerLine {
public:
	static void i_ClampedLerp(POINT MousePOS, float l_Step, float Threshold);

	static void i_Bisectio(POINT MousePOS, float Threshold);

	static void i_None(POINT MousePOS);

	static void DrawPointerLine(POINT MousePOS, ImU32 LineColor, float LineWidth, float Offset);

private:
	static float h_left, h_right, w_top, w_bottom;
};