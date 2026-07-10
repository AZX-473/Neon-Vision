#include "WindowName.h"
#include <imgui.h>
#include <iostream>
#include <string>
#include "../../PublicVariable/Variable.h"

// Define static member
std::string WindowName::showstr = "";

void WindowName::SetShowStr(const std::string& fmt, const std::string& windowName) {
	const std::string key = "${WindowName}";
	size_t pos = fmt.find(key);
	if (pos == std::string::npos) {
		// if key not found, treat fmt as literal pattern with trailing space
		showstr = fmt + " " + windowName;
		return;
	}
	std::string tmp;
	tmp.reserve(fmt.size() + windowName.size());
	tmp.append(fmt.begin(), fmt.begin() + pos);
	tmp.append(windowName);
	tmp.append(fmt.begin() + pos + key.size(), fmt.end());
	showstr = std::move(tmp);
}

void WindowName::SetShowStrDirect(const std::string& text) {
	showstr = text;
}

void WindowName::DrawWindowName(const POINT& MousePOS, ImU32 StrColor, float off_X, float off_Y) {
	ImGui::SetNextWindowBgAlpha(0.0f); // 确保窗口背景完全透明
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);

	ImGuiWindowFlags OverlayWindowflags =
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoInputs |      // 【关键】完全禁用此窗口的所有输入
		ImGuiWindowFlags_NoBackground |
		ImGuiWindowFlags_NoNavFocus;

	ImGui::Begin("RECTbkWindow", nullptr, OverlayWindowflags);

	ImDrawList* draw_list = ImGui::GetWindowDrawList(); // use foreground draw list of the overlay window
	if (!showstr.empty()) {
		ImGui::PushFont(g_MainFont);
		draw_list->AddText(ImVec2((float)MousePOS.x + off_X, (float)MousePOS.y + off_Y), StrColor, showstr.c_str());
		ImGui::PopFont();
	}

	ImGui::End();
}