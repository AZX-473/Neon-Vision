#include "MainWindow.h"
#include <imgui.h>
#include <cstdlib>
#include <ctime>
#include "../PublicVariable/Variable.h"
#include <time.h>
#include "../Utils/Input.h"
#include "../Utils/VirtualKeyCodes.h"

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



	ImGui::EndChild();
}