#include "ColorPicker.h"
#include <imgui.h>
#include <iostream>
#include <string>
#include "../../PublicVariable/Variable.h"

ImVec4 ColorPicker::GetColorFromPos(POINT pt) {
    HDC hdc = GetDC(nullptr);
    COLORREF color = GetPixel(hdc, pt.x, pt.y);
    ReleaseDC(nullptr, hdc);

    return ImVec4(
        GetRValue(color) / 255.0f,
        GetGValue(color) / 255.0f,
        GetBValue(color) / 255.0f,
        1.0f
    );
}

void ColorPicker::DrawPickerColor(POINT pos, ImVec4 color, bool showblock, bool showRGBn, float blockwl, ImVec4 TextCol) {
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

	ImGui::Begin("PickerColorbkWindow", nullptr, OverlayWindowflags);

	ImDrawList* draw_list = ImGui::GetWindowDrawList();//不然无法在整个显示器置顶绘制
	if(showblock)
		draw_list->AddRectFilled({ (float)pos.x,(float)pos.y }, { (float)pos.x + blockwl,(float)pos.y + blockwl }, ImGui::ColorConvertFloat4ToU32(color));
    if (showRGBn) {
        // 转换为 0-255 的整数
        int r = (int)(color.x * 255.0f);
        int g = (int)(color.y * 255.0f);
        int b = (int)(color.z * 255.0f);

        // 使用 static 字符串避免临时对象问题
        static char show_str[64];
        sprintf_s(show_str, sizeof(show_str), "R:%d G:%d B:%d", r, g, b);

        // 绘制文字（在颜色块右侧）
        float x = (float)pos.x + blockwl + 8.0f;
        float y = (float)pos.y;

        ImGui::PushFont(g_MainFont);

        draw_list->AddText(
            ImVec2(x, y),
            ImGui::ColorConvertFloat4ToU32(TextCol),
            show_str
        );

        ImGui::PopFont();
    }

	ImGui::End();
}