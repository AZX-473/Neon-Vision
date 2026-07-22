#include "MainWindow.h"
#include <imgui.h>
#include <cstdlib>
#include <ctime>
#include "../PublicVariable/Variable.h"
#include "Component/WindowTopMost.h"
#include <time.h>
#include "../Utils/Input.h"
#include "../Utils/WindowUtils.h"
#include "../Utils/VirtualKeyCodes.h"
#include "imgui_stdlib.h"
#include "Component/RainingKey.h"
#include "../Utils/AnsiToUTF8.h"
#include <winsparkle.h>
#include "../Utils/ImGuiImage.h"
#include "../Utils/ImGuiAudio.h"
#include "../Utils/http.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

std::string GetExeDirectory() {
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    std::string exePath(buffer);
    size_t pos = exePath.find_last_of("\\/");
    return exePath.substr(0, pos + 1);
}

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
    // Toggle main UI on edge (press) of Right Control key
    static bool prevRControlDown = false;
    bool curRControlDown = KEY_DOWN(VK_RCONTROL);
    if (curRControlDown && !prevRControlDown) {
        MainWindowShow = !MainWindowShow;
    }
    prevRControlDown = curRControlDown;

    if (MainWindowShow) {
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
            case Tab_SC_RainingKey:
                DrawTab_SC_RainingKeyMain();
                break;
            case Tab_SC_ColorPicker:
                DrawTab_SC_ColorPickerMain();
                break;
            case Tab_SC_WindowTopMost:
                DrawTab_SC_WindowTopMostMain();
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
    ShowImage("icon.png",true,0.1);
    ImGui::PushFont(g_SmallFont);
    ImGui::TextColored(ImColor(200, 200, 200), u8"©WanFoxAZX");
    ImGui::PopFont();

    /*static std::string image = "", url = "";
    static bool isDownloading = false;      // 是否正在下载
    static bool isWaitingForImage = false;  // 是否等待图片加载

    auto& dm = DownloadManager::GetInstance();
    std::string exeDir = GetExeDirectory();
    std::string savePath = exeDir + "resources/image.webp";

    // 【关键】只在按钮点击时设置回调，避免每帧重复设置
    if (ImGui::Button(u8"fresh") && !isDownloading) {
        image = httpget("api.lolicon.app/setu/v2");
        json data = json::parse(image);
        url = data["data"][0]["urls"]["original"];

        // 设置完成回调（只设置一次）
        dm.SetOnAllFinished([&]() {
            ReloadImage("image.webp", true, 0.4);
            isDownloading = false;
            isWaitingForImage = false;
            url = "✅ 图片已更新！";
            });

        dm.AddTask(url, savePath, "下载图片");
        dm.StartAll();
        isDownloading = true;
        isWaitingForImage = true;
    }

    // 显示进度（只有下载时才绘制）
    if (isDownloading) {
        dm.DrawAllProgress(400.0f);
    }

    // 显示图片（只在非下载状态或完成后显示）
    if (!isWaitingForImage || !isDownloading) {
        ShowImage("image.webp", true, 0.1);
    }
    
    测试图片下载和API调用时期的产物，懒得删了，如果有相关需要可以复制
    */

    ImGui::EndChild();
}

void MainWindow::DrawTab_ModeMain() {
    ImGui::SetCursorPos({ 160.0f,10.0f });
    ImGui::BeginChild(u8"UI", { 470.0f,330.0f }, true);

    ImGui::PushFont(g_TitleFont);
    ImGui::TextColored(c_show(0), u8"【功能】");
    ImGui::PopFont();

    ImGui::PushStyleColor(ImGuiCol_Button, (ol_PointerLine) ? ImVec4(0.0f, 0.7f, 0.0f, 1.0f) : ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
    ImGui::Text(u8"鼠标定位线");
    ImGui::SameLine();
    if (ImGui::Button((ol_PointerLine)?u8"启用##0":u8"禁用##0", {140.0f,0.0f})) ol_PointerLine ^= 1;
    ImGui::PopStyleColor();

    ImGui::PushStyleColor(ImGuiCol_Button, (ol_WindowRECT) ? ImVec4(0.0f, 0.7f, 0.0f, 1.0f) : ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
    ImGui::Text(u8"窗口边框");
    ImGui::SameLine();
    if (ImGui::Button((ol_WindowRECT) ? u8"启用##1" : u8"禁用##1", { 140.0f,0.0f })) ol_WindowRECT ^= 1;
    ImGui::PopStyleColor();

    ImGui::PushStyleColor(ImGuiCol_Button, (ol_WindowName) ? ImVec4(0.0f, 0.7f, 0.0f, 1.0f) : ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
    ImGui::Text(u8"窗口名称");
    ImGui::SameLine();
    if (ImGui::Button((ol_WindowName) ? u8"启用##2" : u8"禁用##2", { 140.0f,0.0f })) ol_WindowName ^= 1;
    ImGui::PopStyleColor();

    ImGui::PushStyleColor(ImGuiCol_Button, (ol_RainingKey) ? ImVec4(0.0f, 0.7f, 0.0f, 1.0f) : ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
    ImGui::Text(u8"键雨");
    ImGui::SameLine();
    if (ImGui::Button((ol_RainingKey) ? u8"启用##3" : u8"禁用##3", { 140.0f,0.0f })) ol_RainingKey ^= 1;
    ImGui::PopStyleColor();

    ImGui::PushStyleColor(ImGuiCol_Button, (ol_ColorPicker) ? ImVec4(0.0f, 0.7f, 0.0f, 1.0f) : ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
    ImGui::Text(u8"取色器");
    ImGui::SameLine();
    if (ImGui::Button((ol_ColorPicker) ? u8"启用##4" : u8"禁用##4", { 140.0f,0.0f })) ol_ColorPicker ^= 1;
    ImGui::PopStyleColor();

    ImGui::PushStyleColor(ImGuiCol_Button, (ol_WindowTopMost) ? ImVec4(0.0f, 0.7f, 0.0f, 1.0f) : ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
    ImGui::Text(u8"调节窗口置顶");
    ImGui::SameLine();
    if (ImGui::Button((ol_WindowTopMost) ? u8"启用##5" : u8"禁用##5", { 140.0f,0.0f })) ol_WindowTopMost ^= 1;
    ImGui::PopStyleColor();

    ImGui::EndChild();
}

void MainWindow::DrawTab_SurveillanceMain() {
    ImGui::SetCursorPos({ 160.0f,10.0f });
    ImGui::BeginChild(u8"UI", { 470.0f,330.0f }, true);

    ImGui::PushFont(g_TitleFont);
    ImGui::TextColored(c_show(0), u8"【视奸】");
    ImGui::PopFont();

    ImGui::TextColored(ImColor(200, 200, 200), u8"FPS: %.1f", ImGui::GetIO().Framerate);
    // Mouse position
    POINT p; GetCursorPos(&p);
    ImGui::Text(u8"鼠标位置: %d, %d", p.x, p.y);

    // Window under mouse
    HWND hw = WindowUtils::GetWindowUnderCursor();
    std::string wtitle = hw ? WindowUtils::GetWindowTitle(hw) : std::string("(无)");
    RECT wr = hw ? WindowUtils::GetWindowRect(hw) : RECT{0,0,0,0};
    ImGui::Text(u8"鼠标所在窗口: %s", wtitle.c_str());
    ImGui::Text(u8"窗口位置: %d , %d", wr.left, wr.top);
    ImGui::Text(u8"窗口大小: %d , %d", wr.right - wr.left, wr.bottom - wr.top);

    // 额外常用信息：客户区、进程ID、类名
    int cw = 0, ch = 0; if (hw) WindowUtils::GetClientSize(hw, cw, ch);
    ImGui::Text(u8"客户区: %d x %d", cw, ch);
    DWORD pid = hw ? WindowUtils::GetWindowProcessId(hw) : 0;
    ImGui::Text(u8"进程ID: %u", pid);
    std::string cls = hw ? WindowUtils::GetWindowClassName(hw) : std::string();
    ImGui::Text(u8"类名: %s", cls.c_str());

    /*ImGui::PushStyleColor(ImGuiCol_Button, (IsAudioPlaying()) ? ImVec4(0.0f, 0.7f, 0.0f, 1.0f) : ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
    if (ImGui::Button(u8"测试扬声器")) {
        if (IsAudioPlaying()) StopMP3();
        else PlayMP3("The_Big_Black.mp3");
    }
    ImGui::PopStyleColor();*/

    static std::string weatherInfo = "点击刷新获取天气";
    static json ipdata = json::parse(httpget("openapi.lddgo.net/base/gtool/api/v1/GetIp"));
    static std::string ip = ipdata["data"]["ip"];//不知道从哪搞来的神秘小API，补药告我口牙
    static json citydata = json::parse(httpget("api.toolshu.com/api/ip/"+ip));
    static std::string city = citydata["city"];

    ImGui::Text("你的城市(由您的IP得到,可能不准确):%s", city.c_str());
    ImGui::Text("你的IP:%s", ip.c_str());

    if (ImGui::Button(u8"获取天气")) {
        std::string url = "wttr.in/" + city + "?format=%C+%t";
        std::string response = httpget("wttr.in/" + city + "?format=%C+%t");
        weatherInfo = "Weather: " + response;
    }
    ImGui::SameLine();

    ImGui::Text("%s", weatherInfo.c_str());

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

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
    if (ol_PointerLine) if (ImGui::Button(u8"定位线设置", { 140.0f,0.0f })) Tab = Tab_SC_PointerLine;
    if (ol_WindowName) if (ImGui::Button(u8"窗口名称设置", { 140.0f,0.0f })) Tab = Tab_SC_WindowName;
    if (ol_WindowRECT) if (ImGui::Button(u8"窗口边框设置", { 140.0f,0.0f })) Tab = Tab_SC_WindowRECT;
    if (ol_RainingKey) if (ImGui::Button(u8"键雨设置", { 140.0f,0.0f })) Tab = Tab_SC_RainingKey;
    if (ol_ColorPicker) if (ImGui::Button(u8"取色器设置", { 140.0f,0.0f })) Tab = Tab_SC_ColorPicker;
    if (ol_WindowTopMost) if (ImGui::Button(u8"置顶窗口设置", { 140.0f,0.0f })) Tab = Tab_SC_WindowTopMost;
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
    if (!Com_Col_urbc[3]) {
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
    if (!Com_Col_urbc[1]) {
        ImGui::ColorEdit4(u8"颜色", (float*)&Com_Col[1]);
    }
    ImGui::InputFloat(u8"线宽", &lw_WindowRECT);
    ImGui::InputFloat(u8"向外偏移量", &off_WindowRECT);
    static const char* items[] = { u8"二分插值",u8"线性插值",u8"无" };
    ImGui::ListBox(u8"插值类型", &itype_WindowRECT, items, 3);
    if (itype_WindowRECT == 0) {
        ImGui::InputFloat(u8"阈值", &ibt_WindowRECT);
    }
    else if (itype_WindowRECT == 1) {
        ImGui::InputFloat(u8"阈值", &ict_WindowRECT);
        ImGui::InputFloat(u8"步长", &ics_WindowRECT);
    }

    ImGui::EndChild();
}

void MainWindow::DrawTab_SC_RainingKeyMain() {
    ImGui::SetCursorPos({ 160.0f,10.0f });
    ImGui::BeginChild(u8"RainingKeyUI", { 470.0f,330.0f }, true);

    ImGui::PushFont(g_TitleFont);
    ImGui::TextColored(c_show(0), u8"【键雨设置】");
    ImGui::PopFont();

    // Controls: list all keys
    int count = RainingKey::GetKeyCount();
    if (ImGui::Button(u8"添加按键##setting")) {
        // add at default position to the right
        float baseX = 200.0f + count * 120.0f;
        RainingKey::AddKey(baseX, 200.0f, VK_A + (count % 26), 500.0f, std::string("K"));
    }

    ImGui::Separator();

    static int recordingIndex = -1; // which key index is waiting for VK input

    for (int i = 0; i < count; ++i) {
        ImGui::PushID(i);
        KeyBlock kb = RainingKey::GetKeyCopy(i);
        // Use a stable header label (without the editable text) so collapsing state isn't lost when s_key changes
        // Visible label shows index, unique ID ensures stable collapsing state even if kb.s_key changes
        std::string header = std::string(u8"按键 ") + std::to_string(i) + std::string("##rainingkey_") + std::to_string(i);
        if (ImGui::CollapsingHeader(header.c_str())) {
            //ImGui::SameLine(); ImGui::TextUnformatted(kb.s_key.c_str());
            bool dirty = false;

            // operate directly on a local copy and apply immediately when a control changes
            float x = kb.x; float y = kb.y;
            float speed = kb.RainingSpeed;
            float width = kb.width;
            float maxh = kb.max_height;
            float spawn = kb.spawnInterval;
            int vk = kb.vk;
            char label[64]; strncpy(label, kb.s_key.c_str(), 63); label[63] = 0;
            ImVec4 bg = ImGui::ColorConvertU32ToFloat4(kb.bg_col);
            ImVec4 bgp = ImGui::ColorConvertU32ToFloat4(kb.bg_col_pressed);
            ImVec4 rainc = ImGui::ColorConvertU32ToFloat4(kb.raining_col);

            // Allow dragging to change position
            if (ImGui::DragFloat("X坐标##setting", &x, 1.0f, 0.0f, 10000.0f)) { kb.x = x; dirty = true; }
            if (ImGui::DragFloat("Y坐标##setting", &y, 1.0f, 0.0f, 10000.0f)) { kb.y = y; dirty = true; }
            if (ImGui::InputFloat("速度 (px/s)##setting", &speed)) { kb.RainingSpeed = speed; dirty = true; }
            if (ImGui::InputFloat("宽度##setting", &width)) { kb.width = width; dirty = true; }
            if (ImGui::InputFloat("最大长度##setting", &maxh)) { kb.max_height = maxh; dirty = true; }
            if (ImGui::InputFloat("生成间隔(s)##setting", &spawn)) { kb.spawnInterval = spawn; dirty = true; }
            if (ImGui::InputInt("VK码##setting", &vk)) { kb.vk = vk; dirty = true; }
            ImGui::SameLine();
            if (recordingIndex == i) {
                ImGui::TextColored(ImVec4(1,0.5f,0,1), "正在录入... 按下任意键##setting");
            } else {
                if (ImGui::SmallButton("录入VK##setting")) {
                    recordingIndex = i;
                }
            }
            if (ImGui::InputText("显示文本##setting", label, 64)) { kb.s_key = std::string(label); dirty = true; }

            if (ImGui::ColorEdit4("背景未按下颜色##setting", (float*)&bg)) { kb.bg_col = ImGui::ColorConvertFloat4ToU32(bg); dirty = true; }
            if (ImGui::ColorEdit4("背景按下颜色##setting", (float*)&bgp)) { kb.bg_col_pressed = ImGui::ColorConvertFloat4ToU32(bgp); dirty = true; }
            if (ImGui::ColorEdit4("下落颜色##setting", (float*)&rainc)) { kb.raining_col = ImGui::ColorConvertFloat4ToU32(rainc); dirty = true; }

            if (dirty) {
                RainingKey::UpdateKey(i, kb);
            }

            // If this key is in recording mode, scan for first key down
            if (recordingIndex == i) {
                for (int vkCandidate = 0x08; vkCandidate <= 0xFE; ++vkCandidate) {
                    if (KEY_DOWN(vkCandidate)) {
                        kb.vk = vkCandidate;
                        RainingKey::UpdateKey(i, kb);
                        recordingIndex = -1;
                        break;
                    }
                }
            }

            ImGui::SameLine();
            if (ImGui::Button("删除##setting")) {
                RainingKey::RemoveKey(i);
            }
        }
        ImGui::PopID();
    }

    ImGui::EndChild();
}

void MainWindow::DrawTab_SC_ColorPickerMain() {
    ImGui::SetCursorPos({ 160.0f,10.0f });
    ImGui::BeginChild(u8"UI", { 470.0f,330.0f }, true);

    ImGui::PushFont(g_TitleFont);
    ImGui::TextColored(c_show(0), u8"【取色器设置】");
    ImGui::PopFont();

    ImGui::Checkbox(u8"显示颜色预览正方形", &cp_showblock);
    ImGui::Checkbox(u8"显示RGB值", &cp_showRGBn);

    if (cp_showRGBn) {
        ImGui::Checkbox(u8"启用渐变色", &Com_Col_urbc[4]);
        if (!Com_Col_urbc[4]) {
            ImGui::ColorEdit4(u8"颜色", (float*)&Com_Col[4]);
        }
    }
    ImGui::InputInt(u8"X偏移量(相对于鼠标)", (int*) & off_cpx);
    ImGui::InputInt(u8"Y偏移量(相对于鼠标)", (int*)&off_cpy);
    ImGui::InputFloat(u8"正方形边长", &cp_bwlong);

    ImGui::EndChild();
}

void MainWindow::DrawTab_SC_WindowTopMostMain() {
    ImGui::SetCursorPos({ 160.0f,10.0f });
    ImGui::BeginChild(u8"UI", { 470.0f,330.0f }, true);

    ImGui::PushFont(g_TitleFont);
    ImGui::TextColored(c_show(0), u8"【窗口置顶设置】");
    ImGui::PopFont();

    ImGui::Text(u8"置顶与非置顶仅适用于一般软件,对部分软件无效");

    std::vector<WindowInfo> AllWindow=WindowTopMost::GetAllWindow();
    for (WindowInfo window : AllWindow) {
        window.title=ANSIToUTF8(window.title);
        char idStr[32];
        sprintf_s(idStr, sizeof(idStr), "%p", window.hwnd);
        ImGui::PushID(idStr);
        ImGui::PushStyleColor(ImGuiCol_Button, (window.isTopMost) ? ImVec4(0.0f, 0.7f, 0.0f, 1.0f) : ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
        ImGui::Text(window.title.c_str());
        ImGui::SameLine();
        if (ImGui::Button((window.isTopMost) ? u8"置顶":u8"非置顶", {140.0f,0.0f}))
            WindowTopMost::ToggleTopMost(window.hwnd);
        ImGui::PopStyleColor();
        ImGui::PopID();
    }

    ImGui::EndChild();
}