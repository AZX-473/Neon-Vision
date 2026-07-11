#include "PointerLine.h"
#include <iostream>
#include <imgui.h>
#include <Windows.h>

// Define static members declared in PointerLine.h
float PointerLine::h_left = 0;
float PointerLine::h_right = 0;
float PointerLine::w_top = 0;
float PointerLine::w_bottom = 0;

void PointerLine::i_ClampedLerp(POINT MousePOS, float l_Step, float Threshold) {
	if (h_left < MousePOS.y)
	{
		h_left += l_Step;
		if (h_left > MousePOS.y) h_left = MousePOS.y;
	}
	else {
		h_left -= l_Step;
		if (h_left < MousePOS.y) h_left = MousePOS.y;
	}//left

	if (h_right < MousePOS.y)
	{
		h_right += l_Step;
		if (h_left > MousePOS.y) h_left = MousePOS.y;
	}
	else {
		h_right -= l_Step;
		if (h_right < MousePOS.y) h_right = MousePOS.y;
	}//right

	if (w_top < MousePOS.x)
	{
		w_top += l_Step;
		if (w_top > MousePOS.x) w_top = MousePOS.x;
	}
	else {
		w_top -= l_Step;
		if (w_top < MousePOS.x) w_top = MousePOS.x;
	}//top

	if (w_bottom < MousePOS.x)
	{
		w_bottom += l_Step;
		if (w_bottom > MousePOS.x) w_bottom = MousePOS.x;
	}
	else {
		w_bottom -= l_Step;
		if (w_bottom < MousePOS.x) w_bottom = MousePOS.x;
	}//left

}

void PointerLine::i_Bisectio(POINT MousePOS, float Threshold) {
	w_top = (w_top + MousePOS.x) / 2;
	w_bottom = (w_bottom + MousePOS.x) / 2;
	h_left = (h_left + MousePOS.y) / 2;
	h_right = (h_right + MousePOS.y) / 2;
}

void PointerLine::i_None(POINT MousePOS) {
	w_top = MousePOS.x;
	w_bottom = MousePOS.x;
	h_left = MousePOS.y;
	h_right = MousePOS.y;
}

void PointerLine::DrawPointerLine(POINT MousePOS, ImU32 LineColor, float LineWidth, float Offset) {
	ImGui::SetNextWindowBgAlpha(0.0f); 
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);

	ImGuiWindowFlags OverlayWindowflags =
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoInputs |   
		ImGuiWindowFlags_NoBackground |
		ImGuiWindowFlags_NoNavFocus;

	ImGui::Begin("PointerLinebkWindow", nullptr, OverlayWindowflags);

	ImDrawList* draw_list = ImGui::GetForegroundDrawList();
	draw_list->AddLine(ImVec2(w_top, 0), ImVec2(MousePOS.x, MousePOS.y-Offset), LineColor, LineWidth);
	draw_list->AddLine(ImVec2(0, h_left), ImVec2(MousePOS.x - Offset, MousePOS.y), LineColor, LineWidth);
	draw_list->AddLine(ImVec2(w_bottom, GetSystemMetrics(SM_CYFULLSCREEN)), ImVec2(MousePOS.x, MousePOS.y + Offset), LineColor, LineWidth);
	draw_list->AddLine(ImVec2(GetSystemMetrics(SM_CXFULLSCREEN), h_right), ImVec2(MousePOS.x + Offset, MousePOS.y), LineColor, LineWidth);
	
	ImGui::End();
}