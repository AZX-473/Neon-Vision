#include "WindowRECT.h"
#include <iostream>
#include <imgui.h>
#include <cmath>

RECT WindowRECT::w_WindowRECT = { 0, 0, 0, 0 };

void WindowRECT::i_ClampedLerp(RECT NewRECT, float l_Step, float Threshold)
{
	if (w_WindowRECT.left < NewRECT.left)
	{
		w_WindowRECT.left += l_Step;
		if (w_WindowRECT.left > NewRECT.left || std::abs(w_WindowRECT.left - NewRECT.left) <= Threshold)
			w_WindowRECT.left = NewRECT.left;
	}
	else
	{
		w_WindowRECT.left -= l_Step;
		if (w_WindowRECT.left < NewRECT.left || std::abs(w_WindowRECT.left - NewRECT.left) <= Threshold)
			w_WindowRECT.left = NewRECT.left;
	}// Update left

	if (w_WindowRECT.right < NewRECT.right)
	{
		w_WindowRECT.right += l_Step;
		if (w_WindowRECT.right > NewRECT.right || std::abs(w_WindowRECT.right - NewRECT.right) <= Threshold)
			w_WindowRECT.right = NewRECT.right;
	}
	else
	{
		w_WindowRECT.right -= l_Step;
		if (w_WindowRECT.right < NewRECT.right || std::abs(w_WindowRECT.right - NewRECT.right) <= Threshold)
			w_WindowRECT.right = NewRECT.right;
	}// Update right

	if (w_WindowRECT.top < NewRECT.top)
	{
		w_WindowRECT.top += l_Step;
		if (w_WindowRECT.top > NewRECT.top || std::abs(w_WindowRECT.top - NewRECT.top) <= Threshold)
			w_WindowRECT.top = NewRECT.top;
	}
	else
	{
		w_WindowRECT.top -= l_Step;
		if (w_WindowRECT.top < NewRECT.top || std::abs(w_WindowRECT.top - NewRECT.top) <= Threshold)
			w_WindowRECT.top = NewRECT.top;
	}// Update top

	if (w_WindowRECT.bottom < NewRECT.bottom)
	{
		w_WindowRECT.bottom += l_Step;
		if (w_WindowRECT.bottom > NewRECT.bottom || std::abs(w_WindowRECT.bottom - NewRECT.bottom) <= Threshold)
			w_WindowRECT.bottom = NewRECT.bottom;
	}
	else
	{
		w_WindowRECT.bottom -= l_Step;
		if (w_WindowRECT.bottom < NewRECT.bottom || std::abs(w_WindowRECT.bottom - NewRECT.bottom) <= Threshold)
			w_WindowRECT.bottom = NewRECT.bottom;
	}// Update bottom
}

void WindowRECT::i_Bisectio(RECT NewRECT, float Threshold)
{
	w_WindowRECT.left = (w_WindowRECT.left + NewRECT.left) / 2;
	w_WindowRECT.right = (w_WindowRECT.right + NewRECT.right) / 2;
	w_WindowRECT.top = (w_WindowRECT.top + NewRECT.top) / 2;
	w_WindowRECT.bottom = (w_WindowRECT.bottom + NewRECT.bottom) / 2;
	if (std::abs(w_WindowRECT.left - NewRECT.left) <= Threshold)
		w_WindowRECT.left = NewRECT.left;
	if (std::abs(w_WindowRECT.right - NewRECT.right) <= Threshold)
		w_WindowRECT.right = NewRECT.right;
	if (std::abs(w_WindowRECT.top - NewRECT.top) <= Threshold)
		w_WindowRECT.top = NewRECT.top;
	if (std::abs(w_WindowRECT.bottom - NewRECT.bottom) <= Threshold)
		w_WindowRECT.bottom = NewRECT.bottom;
}

void WindowRECT::i_None(RECT NewRECT)
{
	w_WindowRECT = NewRECT;
}

void WindowRECT::DrawWindowRECT(ImU32 RECTColor, float LineWidth, float Offset)
{
	ImDrawList* draw_list = /*ImGui::GetWindowDrawList()*/ImGui::GetForegroundDrawList();//不然无法在整个显示器置顶绘制
	draw_list->AddLine(ImVec2(w_WindowRECT.left - Offset, w_WindowRECT.top - Offset), ImVec2(w_WindowRECT.right + Offset, w_WindowRECT.top - Offset), RECTColor, LineWidth); // Draw Top
	draw_list->AddLine(ImVec2(w_WindowRECT.left - Offset, w_WindowRECT.top - Offset), ImVec2(w_WindowRECT.left - Offset, w_WindowRECT.bottom + Offset), RECTColor, LineWidth); // Draw Left
	draw_list->AddLine(ImVec2(w_WindowRECT.right + Offset, w_WindowRECT.top - Offset), ImVec2(w_WindowRECT.right + Offset, w_WindowRECT.bottom + Offset), RECTColor, LineWidth); // Draw Right
	draw_list->AddLine(ImVec2(w_WindowRECT.left - Offset, w_WindowRECT.bottom + Offset), ImVec2(w_WindowRECT.right + Offset, w_WindowRECT.bottom + Offset), RECTColor, LineWidth); // Draw Bottom
}