#include "Window/WindowManager.h"
#include "IGM/ImGuiManager.h"
#include "UI/MainWindow.h"
#include "UI/Component/WindowRECT.h"
#include "Utils/WindowUtils.h"
#include "FontManager/FontManager.h"
#include "PublicVariable/Variable.h"
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h> 
#include <iostream>

int main() {

    FontManager::GetInstance().RegisterFont(g_MainFont, "msyh", 16.0f);
    FontManager::GetInstance().RegisterFont(g_TitleFont, "msyh", 24.0f);

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

    FontManager::GetInstance().InitFonts();

    // Create main window UI
    MainWindow mainWindow;

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGuiManager::NewFrame();

        // Draw UI
        mainWindow.Draw();

        // Draw Functional
        RECT w_rect = WindowUtils::GetWindowRect(WindowUtils::GetWindowUnderCursor());
        WindowRECT::i_Bisectio(w_rect, 1.0f);
        WindowRECT::DrawWindowRECT(ImGui::ColorConvertFloat4ToU32({ rb_R, rb_G, rb_B, 255 }), 2.0f , 5.0f);

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