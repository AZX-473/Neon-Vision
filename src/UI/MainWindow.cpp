#include "MainWindow.h"
#include <imgui.h>
#include <cstdlib>
#include <ctime>
#include "../PublicVariable/Variable.h"
#include <time.h>
#include "../Utils/Input.h"
#include "../Utils/VirtualKeyCodes.h"
#include "imgui_stdlib.h"

MainWindow::MainWindow()
    : m_counter(0)
    , m_sliderValue(0.5f)
    , m_color(0.2f, 0.6f, 0.9f, 1.0f)
    , m_selected(0)
    , m_checkbox(false)
    , m_statusMessage("Ready") {
    m_inputText[0] = '\0';
    srand(static_cast<unsigned>(time(nullptr)));
    for (int i = 0; i < 100; i++) {
        m_values.push_back(rand() % 100 / 100.0f);
    }
}

void MainWindow::Draw(ImVec2 WindowSize) {
    float base = fmodf(ImGui::GetTime() * RainbowSpeed, 1.0f);
    ImGui::ColorConvertHSVtoRGB(base, 1.0f, 1.0f, rb_R, rb_G, rb_B);

    ImGui::SetNextWindowSize(WindowSize);

    ImGuiWindowFlags window_flags =
        ImGuiWindowFlags_NoTitleBar |
        /*ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoMove |*/
        ImGuiWindowFlags_NoResize ;
    if (MainWindowShow || KEY_DOWN(VK_RCONTROL)) {
        if (ImGui::Begin("NeonVision Main", nullptr, window_flags)) {
            ImGui::PushFont(g_MainFont);

            DrawMENU();
            switch (Tab) {
            case Tab_Home:
                DrawTab_HomeMain();
                break;
            case Tab_Vision:
                DrawTab_VisionMain();
                break;
            case Tab_Mode:
                DrawTab_ModeMain();
                break;
            case Tab_Surveillance:
                DrawTab_SurveillanceMain();
                break;
            case Tab_SC_PointerLine:
                DrawTab_SC_PointerLineMain();
                break;
            case Tab_SC_WindowName:
                DrawTab_SC_WindowNameMain();
                break;
            case Tab_SC_WindowRECT:
                DrawTab_SC_WindowRECTMain();
                break;
            }
            ImGui::PopFont();
        }
        ImGui::End();
    }
}

void MainWindow::DrawMENU() {
    ImGui::PushFont(g_TitleFont);
    ImGui::TextColored(c_show(0), u8"【霓虹视觉】\nNeon Vision");
    ImGui::PopFont();

    const float button_width = 140.0f;

    ImGui::PushStyleColor(ImGuiCol_Button, (Tab == Tab_Home) ? c_show(0) : ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
    if (ImGui::Button(u8"[主页]Home", { button_width,0.0f })) Tab = Tab_Home;
    ImGui::PopStyleColor();

    ImGui::PushStyleColor(ImGuiCol_Button, (Tab == Tab_Vision) ? c_show(0) : ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
    if (ImGui::Button(u8"[视觉]Vision", { button_width,0.0f })) Tab = Tab_Vision;
    ImGui::PopStyleColor();

    ImGui::PushStyleColor(ImGuiCol_Button, (Tab == Tab_Mode) ? c_show(0) : ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
    if (ImGui::Button(u8"[工具]Tool", { button_width,0.0f })) Tab = Tab_Mode;
    ImGui::PopStyleColor();

    ImGui::PushStyleColor(ImGuiCol_Button, (Tab == Tab_Surveillance) ? c_show(0) : ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
    if (ImGui::Button(u8"[监视]Surveillance", { button_width,0.0f })) Tab = Tab_Surveillance;
    ImGui::PopStyleColor();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
    if (ImGui::Button(u8"[退出]Exit", { button_width,0.0f })) SendMessage(hWnd, WM_CLOSE, 0, 0);
    ImGui::PopStyleColor();
}

void MainWindow::DrawTab_HomeMain() {
    ImGui::SetCursorPos({ 160.0f,10.0f });
    ImGui::BeginChild(u8"UI", { 470.0f,330.0f }, true);

    ImGui::PushFont(g_TitleFont);
    ImGui::TextColored(c_show(0), u8"【主页】");
    ImGui::PopFont();

    time_t t;
    static char Time[32];
    t = time(0);
    memset(Time, 0, sizeof Time);
    strftime(Time, sizeof(Time), "%Y-%m-%d %H:%M:%S", localtime(&t));
    ImGui::TextColored(ImColor(200, 200, 200), u8"Hello!\nNeon Vision\nTime:%s", Time);

    ImGui::EndChild();
}

void MainWindow::DrawTab_ModeMain() {
    ImGui::SetCursorPos({ 160.0f,10.0f });
    ImGui::BeginChild(u8"UI", { 470.0f,330.0f }, true);

    ImGui::PushFont(g_TitleFont);
    ImGui::TextColored(c_show(0), u8"【功能】");
    ImGui::PopFont();

    bool tmp=false;
    ImGui::Checkbox(u8"鼠标辅助功能", &tmp);
    ImGui::Checkbox(u8"对选中窗口操作", &tmp);
    ImGui::Checkbox(u8"电脑辅助功能", &tmp);
    ImGui::Checkbox(u8"剪切本功能", &tmp);

    ImGui::EndChild();
}

void MainWindow::DrawTab_SurveillanceMain() {
    ImGui::SetCursorPos({ 160.0f,10.0f });
    ImGui::BeginChild(u8"UI", { 470.0f,330.0f }, true);

    ImGui::PushFont(g_TitleFont);
    ImGui::TextColored(c_show(0), u8"【视奸】");
    ImGui::PopFont();

    ImGui::TextColored(ImColor(200, 200, 200), u8"FPS: %.1f", ImGui::GetIO().Framerate);

    ImGui::EndChild();
}

void MainWindow::DrawTab_VisionMain() {
	ImGui::SetCursorPos({ 160.0f,10.0f });
	ImGui::BeginChild(u8"UI", { 470.0f,330.0f }, true);

    ImGui::PushFont(g_TitleFont);
    ImGui::TextColored(c_show(0), u8"【视觉效果】");
    ImGui::PopFont();

    ImGui::Checkbox(u8"UI启用渐变色", &Com_Col_urbc[0]);
    if (!Com_Col_urbc[0]) {
        ImGui::ColorEdit4(u8"颜色", (float*)&Com_Col[0]);
    }

    ImGui::PushStyleColor(ImGuiCol_Button, c_show(0));
    if (ImGui::Button(u8"定位线设置", { 140.0f,0.0f })) Tab = Tab_SC_PointerLine;
    if (ImGui::Button(u8"窗口名称设置", { 140.0f,0.0f })) Tab = Tab_SC_WindowName;
    if (ImGui::Button(u8"窗口边框设置", { 140.0f,0.0f })) Tab = Tab_SC_WindowRECT;
    ImGui::PopStyleColor();

	ImGui::EndChild();
}

void MainWindow::DrawTab_SC_PointerLineMain() {
    ImGui::SetCursorPos({ 160.0f,10.0f });
    ImGui::BeginChild(u8"UI", { 470.0f,330.0f }, true);

    ImGui::PushFont(g_TitleFont);
    ImGui::TextColored(c_show(0), u8"【定位线设置】");
    ImGui::PopFont();

    ImGui::Checkbox(u8"启用渐变色", &Com_Col_urbc[2]);
    if (!Com_Col_urbc[2]) {
        ImGui::ColorEdit4(u8"颜色", (float*)&Com_Col[2]);
    }
    ImGui::InputFloat(u8"线宽", &lw_PointerLine);
    ImGui::InputFloat(u8"向外偏移量", &off_PointerLine);
    static const char* items[] = { u8"二分插值",u8"线性插值",u8"无" };
    ImGui::ListBox(u8"插值类型", &itype_PointerLine, items, 3);
    if (itype_PointerLine == 0) {
        ImGui::InputFloat(u8"阈值", &ibt_PointerLine);
    }
    else if (itype_PointerLine == 1) {
        ImGui::InputFloat(u8"阈值", &ict_PointerLine);
        ImGui::InputFloat(u8"步长", &ics_PointerLine);
    }

    ImGui::EndChild();
}

void MainWindow::DrawTab_SC_WindowNameMain() {
    ImGui::SetCursorPos({ 160.0f,10.0f });
    ImGui::BeginChild(u8"UI", { 470.0f,330.0f }, true);

    ImGui::PushFont(g_TitleFont);
    ImGui::TextColored(c_show(0), u8"【窗口名称设置】");
    ImGui::PopFont();

    ImGui::Checkbox(u8"启用渐变色", &Com_Col_urbc[3]);
    if (!Com_Col_urbc[2]) {
        ImGui::ColorEdit4(u8"颜色", (float*)&Com_Col[3]);
    }
    ImGui::InputFloat(u8"X向外偏移量", &off_wnX);
    ImGui::InputFloat(u8"Y向外偏移量", &off_wnY);
    ImGui::InputText(u8"显示文本", &str_WindowName);
    ImGui::Text(u8"请使用 ${WindowName} 来表示窗口名称");

    ImGui::EndChild();
}

void MainWindow::DrawTab_SC_WindowRECTMain() {
    ImGui::SetCursorPos({ 160.0f,10.0f });
    ImGui::BeginChild(u8"UI", { 470.0f,330.0f }, true);

    ImGui::PushFont(g_TitleFont);
    ImGui::TextColored(c_show(0), u8"【窗口边框设置】");
    ImGui::PopFont();

    ImGui::Checkbox(u8"启用渐变色", &Com_Col_urbc[1]);
    if (!Com_Col_urbc[2]) {
        ImGui::ColorEdit4(u8"颜色", (float*)&Com_Col[1]);
    }
    ImGui::InputFloat(u8"线宽", &lw_WindowRECT);
    ImGui::InputFloat(u8"向外偏移量", &off_WindowRECT);
    static const char* items[] = { u8"二分插值",u8"线性插值",u8"无" };
    ImGui::ListBox(u8"插值类型", &itype_WindowRECT, items, 3);
    if (itype_WindowRECT == 0) {
        ImGui::InputFloat(u8"阈值", &ibt_WindowRECT);
    }
    else if (itype_PointerLine == 1) {
        ImGui::InputFloat(u8"阈值", &ict_WindowRECT);
        ImGui::InputFloat(u8"步长", &ics_WindowRECT);
    }

    ImGui::EndChild();
}