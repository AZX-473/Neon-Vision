#include <iostream>
#include <Windows.h>
#include <imgui.h>
class WindowRECT {
public:
	static void i_ClampedLerp(RECT NewRECT, LONG l_Step, float Threshold);

	static void i_Bisectio(RECT NewRECT, float Threshold);

	static void i_None(RECT NewRECT);

	static void DrawWindowRECT(ImU32 RECTColor, float LineWidth, float Offset);

private:
	static RECT w_WindowRECT;
};