#include "Window/WindowManager.h"
#include "IGM/ImGuiManager.h"
#include "UI/MainWindow.h"
#include "UI/Component/WindowRECT.h"
#include "UI/Component/PointerLine.h"
#include "UI/Component/WindowName.h"
#include "UI/Component/ColorPicker.h"
#include "Utils/WindowUtils.h"
#include "FontManager/FontManager.h"
#include "PublicVariable/Variable.h"
#include "Utils/VirtualKeyCodes.h"
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h> 
#include <iostream>
#include "UI/Component/RainingKey.h"
#include <winsparkle.h>

int main() {

    FontManager::GetInstance().RegisterFont(g_MainFont, "msyh", 16.0f);
    FontManager::GetInstance().RegisterFont(g_TitleFont, "msyh", 24.0f);
    FontManager::GetInstance().RegisterFont(g_SmallFont, "msyh", 8.0f);

    // Initialize GLFW
    if (!WindowManager::InitGLFW()) {
        return 1;
    }

    // Create window
    GLFWwindow* window = WindowManager::CreateWindowN(GetSystemMetrics(SM_CXFULLSCREEN), GetSystemMetrics(SM_CYFULLSCREEN), "NeonVision");
    if (!window) {
        WindowManager::TerminateGLFW();
        return 1;
    }

	hWnd = glfwGetWin32Window(window); // Store the window handle in the global variable

    HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(101)); // 101 是资源 ID
    if (hIcon) {
        SendMessage(hWnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
        SendMessage(hWnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
    }

    // Center window
    WindowManager::CenterWindow(window);

    // Setup transparent
    WindowManager::SetupTransparentWindow(window);

    // Initialize ImGui
    if (!ImGuiManager::Init(window)) {
        WindowManager::DestroyWindow(window);
        WindowManager::TerminateGLFW();
        return 1;
    }

    win_sparkle_set_appcast_url("https://raw.githubusercontent.com/AZX-473/Neon-Vision/master/docs/appcast.xml");
    win_sparkle_set_app_details(
        L"AZX",           // 公司名/作者
        L"NeonVision",        // 应用名
        L"1.1.0"              // 当前版本号（必须和你的程序版本一致）
    );
    win_sparkle_set_automatic_check_for_updates(TRUE);
    win_sparkle_set_update_check_interval(86400);
    win_sparkle_init();

    FontManager::GetInstance().InitFonts();

    // Create main window UI
    MainWindow mainWindow;

    // Register a default row of raining keys (defaults applied when parameters omitted)
    float baseX = 80.0f;
    float baseY = 1000.0f; // y remains same for all keys by default
    float spacing = 50.0f; // x will accumulate per key
    const char* labels[] = { "A","S","D","F","G","H","J","K","L","space" };
    int vks[] = { VK_A, VK_S, VK_D, VK_F, VK_G, VK_H, VK_J, VK_K, VK_L, VK_SPACE };
    for (int i = 0; i < 10; ++i) {
        float x = baseX + i * spacing;
        // Use higher speed so blocks move visibly fast (pixels per second)
        RainingKey::AddKey(x, baseY, vks[i], /*RainingSpeed*/ 500.0f, std::string(labels[i]));
    }

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGuiManager::NewFrame();

        // Draw UI
        mainWindow.Draw();

        // Draw Functional
        if (ol_WindowRECT) {
            RECT w_rect = WindowUtils::GetWindowRect(WindowUtils::GetWindowUnderCursor());
            if (itype_WindowRECT == 0) WindowRECT::i_Bisectio(w_rect, ibt_WindowRECT);
            else if (itype_WindowRECT == 1) WindowRECT::i_ClampedLerp(w_rect, ics_WindowRECT, ict_WindowRECT);
            else if (itype_WindowRECT == 2) WindowRECT::i_None(w_rect);
            WindowRECT::DrawWindowRECT(ImGui::ColorConvertFloat4ToU32(c_show(1)), lw_WindowRECT, off_WindowRECT);
        }

        POINT MousePOS;
        GetCursorPos(&MousePOS);
        if (ol_PointerLine) {
            if (itype_PointerLine == 0) PointerLine::i_Bisectio(MousePOS, ibt_PointerLine);
            else if (itype_PointerLine == 1) PointerLine::i_ClampedLerp(MousePOS, ics_PointerLine, ict_PointerLine);
            else if (itype_PointerLine == 2) PointerLine::i_None(MousePOS);
            PointerLine::DrawPointerLine(MousePOS, ImGui::ColorConvertFloat4ToU32(c_show(2)), lw_PointerLine, off_PointerLine);
        }

        if (ol_WindowName) {
            WindowName::SetShowStr(str_WindowName, WindowUtils::GetWindowTitle(WindowUtils::GetWindowUnderCursor()));
            WindowName::DrawWindowName(MousePOS, ImGui::ColorConvertFloat4ToU32(c_show(3)), off_wnX, off_wnY);
        }

        if(ol_ColorPicker)
            ColorPicker::DrawPickerColor({ MousePOS.x + off_cpx,MousePOS.y + off_cpy }, ColorPicker::GetColorFromPos(MousePOS),cp_showblock,cp_showRGBn, cp_bwlong,c_show(4));

        // Update and draw raining key overlays
        if (ol_RainingKey)
            RainingKey::UpdateRainingBlocks();

        // Render
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGuiManager::Render();
        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGuiManager::Shutdown();
    WindowManager::DestroyWindow(window);
    WindowManager::TerminateGLFW();

    return 0;
}