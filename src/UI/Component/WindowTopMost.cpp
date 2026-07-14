#include "WindowTopMost.h"
#include <imgui.h>
#include <iostream>
#include <Windows.h>

std::vector<WindowInfo> WindowTopMost::AllWindow;

// 切换窗口置顶状态
void WindowTopMost::ToggleTopMost(HWND hwnd) {
    if (hwnd == nullptr) return;

    bool isTopMost = IsWindowTopMost(hwnd);

    SetWindowPos(hwnd,
        isTopMost ? HWND_NOTOPMOST : HWND_TOPMOST,  // 置顶则取消，非置顶则置顶
        0, 0, 0, 0,
        SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW
    );
}

// 枚举窗口回调（必须是静态函数或全局函数）
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
    // 通过 LPARAM 传递 vector 指针
    std::vector<HWND>* pWindows = reinterpret_cast<std::vector<HWND>*>(lParam);

    // 只显示可见的顶层窗口
    if (IsWindowVisible(hwnd) && GetParent(hwnd) == nullptr) {
        pWindows->push_back(hwnd);
    }
    return TRUE;
}

std::vector<WindowInfo> WindowTopMost::GetAllWindow() {
    AllWindow.clear();

    // 临时存储 HWND
    std::vector<HWND> hwndList;

    // 传递 vector 的指针
    EnumWindows(EnumWindowsProc, reinterpret_cast<LPARAM>(&hwndList));

    int tmpid = 0;
    // 转换为 WindowInfo
    for (HWND hwnd : hwndList) {
        char title[256];
        GetWindowTextA(hwnd, title, sizeof(title));

        // 跳过没有标题的窗口
        if (strlen(title) == 0) continue;

        WindowInfo info;
        info.hwnd = hwnd;
        info.title = title;
        info.isTopMost = IsWindowTopMost(hwnd);
        info.id = tmpid;
        AllWindow.push_back(info);
    }
    return AllWindow;
}

bool WindowTopMost::IsWindowTopMost(HWND hwnd) {
    LONG exStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
    return (exStyle & WS_EX_TOPMOST) != 0;
}
