#include "WindowUtils.h"
#include <dwmapi.h>
#include <iostream>
#include "../PublicVariable/Variable.h"

#pragma comment(lib, "dwmapi.lib")

// 静态成员初始化
WindowUtils::WindowStyleBackup WindowUtils::s_backup;

// ============================================
// 窗口样式操作
// ============================================

void WindowUtils::RemoveTitleBar(HWND hwnd) {
    s_backup.style = GetWindowLongPtr(hwnd, GWL_STYLE);
    s_backup.exStyle = GetWindowLongPtr(hwnd, GWL_EXSTYLE);
    s_backup.hasBackup = true;

    LONG_PTR style = s_backup.style;
    style &= ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU);
    style |= WS_POPUP;
    SetWindowLongPtr(hwnd, GWL_STYLE, style);

    SetWindowPos(hwnd, NULL, 0, 0, 0, 0,
        SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER);
}

void WindowUtils::RestoreTitleBar(HWND hwnd) {
    if (s_backup.hasBackup) {
        SetWindowLongPtr(hwnd, GWL_STYLE, s_backup.style);
        SetWindowLongPtr(hwnd, GWL_EXSTYLE, s_backup.exStyle);
        SetWindowPos(hwnd, NULL, 0, 0, 0, 0,
            SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER);
        s_backup.hasBackup = false;
    }
}

void WindowUtils::SetLayeredWindow(HWND hwnd) {
    LONG_PTR exStyle = GetWindowLongPtr(hwnd, GWL_EXSTYLE);
    SetWindowLongPtr(hwnd, GWL_EXSTYLE, exStyle | WS_EX_LAYERED);
}

void WindowUtils::SetTransparentColor(HWND hwnd, COLORREF color) {
    SetLayeredWindow(hwnd);
    SetLayeredWindowAttributes(hwnd, color, 0, LWA_COLORKEY);
}

void WindowUtils::SetWindowAlpha(HWND hwnd, BYTE alpha) {
    SetLayeredWindow(hwnd);
    SetLayeredWindowAttributes(hwnd, 0, alpha, LWA_ALPHA);
}

void WindowUtils::SetMouseTransparent(HWND hwnd, bool enable) {
    LONG_PTR exStyle = GetWindowLongPtr(hwnd, GWL_EXSTYLE);
    if (enable) {
        SetWindowLongPtr(hwnd, GWL_EXSTYLE, exStyle | WS_EX_TRANSPARENT);
    }
    else {
        SetWindowLongPtr(hwnd, GWL_EXSTYLE, exStyle & ~WS_EX_TRANSPARENT);
    }
}

void WindowUtils::SetTopmost(HWND hwnd, bool topmost) {
    SetWindowPos(hwnd, topmost ? HWND_TOPMOST : HWND_NOTOPMOST,
        0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}

void WindowUtils::SetFullscreen(HWND hwnd) {
    HMONITOR monitor = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
    MONITORINFO mi = { sizeof(MONITORINFO) };
    GetMonitorInfo(monitor, &mi);

    RemoveTitleBar(hwnd);
    SetWindowPos(hwnd, HWND_TOPMOST,
        mi.rcMonitor.left, mi.rcMonitor.top,
        mi.rcMonitor.right - mi.rcMonitor.left,
        mi.rcMonitor.bottom - mi.rcMonitor.top,
        SWP_FRAMECHANGED);
}

void WindowUtils::RestoreFromFullscreen(HWND hwnd) {
    RestoreTitleBar(hwnd);
    SetWindowPos(hwnd, HWND_NOTOPMOST,
        100, 100, 800, 600,
        SWP_FRAMECHANGED);
}

void WindowUtils::SetWindowSize(HWND hwnd, int width, int height) {
    SetWindowPos(hwnd, NULL, 0, 0, width, height,
        SWP_NOMOVE | SWP_NOZORDER);
}

void WindowUtils::SetWindowPosition(HWND hwnd, int x, int y) {
    SetWindowPos(hwnd, NULL, x, y, 0, 0,
        SWP_NOSIZE | SWP_NOZORDER);
}

void WindowUtils::CenterWindow(HWND hwnd) {
    RECT rect;
    ::GetWindowRect(hwnd, &rect);
    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;

    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    int x = (screenWidth - width) / 2;
    int y = (screenHeight - height) / 2;
    SetWindowPosition(hwnd, x, y);
}

void WindowUtils::SetWindowRoundCorners(HWND hwnd, int radius) {
    DWM_WINDOW_CORNER_PREFERENCE cornerPref = DWMWCP_ROUND;
    DwmSetWindowAttribute(hwnd, DWMWA_WINDOW_CORNER_PREFERENCE,
        &cornerPref, sizeof(cornerPref));

    HRGN hRgn = CreateRoundRectRgn(0, 0, 1, 1, radius, radius);
    SetWindowRgn(hwnd, hRgn, TRUE);
}

void WindowUtils::SetWindowShadow(HWND hwnd, bool enable) {
    MARGINS margins;
    if (enable) {
        margins = { -1, -1, -1, -1 };
    }
    else {
        margins = { 0, 0, 0, 0 };
    }
    DwmExtendFrameIntoClientArea(hwnd, &margins);
}

void WindowUtils::MakeWindowClickThrough(HWND hwnd) {
    LONG_PTR exStyle = GetWindowLongPtr(hwnd, GWL_EXSTYLE);
    SetWindowLongPtr(hwnd, GWL_EXSTYLE,
        exStyle | WS_EX_TRANSPARENT | WS_EX_LAYERED);
    SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 0, LWA_COLORKEY);
}

void WindowUtils::SetToolWindow(HWND hwnd) {
    LONG_PTR exStyle = GetWindowLongPtr(hwnd, GWL_EXSTYLE);
    SetWindowLongPtr(hwnd, GWL_EXSTYLE, exStyle | WS_EX_TOOLWINDOW);
}

// ============================================
// 窗口生命周期操作
// ============================================

void WindowUtils::CloseWindow(HWND hwnd) {
    SendMessage(hwnd, WM_CLOSE, 0, 0);
}

void WindowUtils::ForceCloseWindow(HWND hwnd) {
    TerminateProcess(GetCurrentProcess(), 0);
}

void WindowUtils::MinimizeWindow(HWND hwnd) {
    ::ShowWindow(hwnd, SW_MINIMIZE);
}

void WindowUtils::MaximizeWindow(HWND hwnd) {
    ::ShowWindow(hwnd, SW_MAXIMIZE);
}

void WindowUtils::RestoreWindow(HWND hwnd) {
    ::ShowWindow(hwnd, SW_RESTORE);
}

void WindowUtils::HideWindow(HWND hwnd) {
    ::ShowWindow(hwnd, SW_HIDE);
}

void WindowUtils::ShowWindow(HWND hwnd) {
    ::ShowWindow(hwnd, SW_SHOW);
}

// ============================================
// 获取窗口信息
// ============================================

RECT WindowUtils::GetWindowRect(HWND hwnd) {
    RECT rect;
    ::GetWindowRect(hwnd, &rect);
    return rect;
}

void WindowUtils::GetWindowPosition(HWND hwnd, int& x, int& y) {
    RECT rect;
    ::GetWindowRect(hwnd, &rect);
    x = rect.left;
    y = rect.top;
}

void WindowUtils::GetWindowSize(HWND hwnd, int& width, int& height) {
    RECT rect;
    ::GetWindowRect(hwnd, &rect);
    width = rect.right - rect.left;
    height = rect.bottom - rect.top;
}

void WindowUtils::GetClientSize(HWND hwnd, int& width, int& height) {
    RECT rect;
    ::GetClientRect(hwnd, &rect);
    width = rect.right - rect.left;
    height = rect.bottom - rect.top;
}

std::string WindowUtils::GetWindowTitle(HWND hwnd) {
    // Do not return our own application's window title to avoid self-display
    if (hwnd == NULL) return std::string();
    if (hwnd == hWnd) return std::string();

    wchar_t buffer[256];
    GetWindowTextW(hwnd, buffer, 256);

    int len = WideCharToMultiByte(CP_UTF8, 0, buffer, -1, NULL, 0, NULL, NULL);
    if (len <= 0) return std::string();

    std::string result(len - 1, 0);
    WideCharToMultiByte(CP_UTF8, 0, buffer, -1, &result[0], len, NULL, NULL);
    return result;
}

std::wstring WindowUtils::GetWindowTitleW(HWND hwnd) {
    wchar_t buffer[256];
    GetWindowTextW(hwnd, buffer, 256);
    return std::wstring(buffer);
}

void WindowUtils::SetWindowTitle(HWND hwnd, const std::string& title) {
    int len = MultiByteToWideChar(CP_UTF8, 0, title.c_str(), -1, NULL, 0);
    if (len <= 0) return;

    std::wstring wtitle(len, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, title.c_str(), -1, &wtitle[0], len);
    SetWindowTextW(hwnd, wtitle.c_str());
}

void WindowUtils::SetWindowTitleW(HWND hwnd, const std::wstring& title) {
    SetWindowTextW(hwnd, title.c_str());
}

HWND WindowUtils::GetWindowUnderCursor() {
    POINT pt;
    GetCursorPos(&pt);
    HWND hwnd = WindowFromPoint(pt);
    // If the topmost window (or its top-level ancestor) is our own overlay, find the next visible window beneath it
    HWND topLevel = hwnd ? GetAncestor(hwnd, GA_ROOT) : NULL;
    // Treat any window that belongs to our process as 'our window' (covers child/controls)
    DWORD wndPid = 0;
    if (hwnd) GetWindowThreadProcessId(hwnd, &wndPid);
    bool isOurWindow = (hwnd == hWnd) || (topLevel == hWnd) || (wndPid == GetCurrentProcessId());
    if (isOurWindow) {
        // Iterate top-level windows in Z order (topmost first)
        for (HWND cur = GetTopWindow(NULL); cur != NULL; cur = GetNextWindow(cur, GW_HWNDNEXT)) {
            if (cur == hWnd) continue;
            if (!IsWindowVisible(cur)) continue;
            // skip windows that belong to our process
            DWORD curPid = 0;
            GetWindowThreadProcessId(cur, &curPid);
            if (curPid == GetCurrentProcessId()) continue;
            RECT r;
            if (!::GetWindowRect(cur, &r)) continue;
            if (PtInRect(&r, pt)) {
                return cur;
            }
        }
        // fallback: return desktop window
        return GetDesktopWindow();
    }
    return hwnd;
}

HWND WindowUtils::GetWindowUnderCursorEx() {
    POINT pt;
    GetCursorPos(&pt);
    return ChildWindowFromPoint(GetDesktopWindow(), pt);
}

HWND WindowUtils::GetWindowAtPoint(int x, int y) {
    POINT pt = { x, y };
    return WindowFromPoint(pt);
}

HWND WindowUtils::GetWindowAtPointEx(int x, int y) {
    POINT pt = { x, y };
    return ChildWindowFromPoint(GetDesktopWindow(), pt);
}

HWND WindowUtils::GetActiveWindow() {
    return ::GetActiveWindow();
}

HWND WindowUtils::GetForegroundWindow() {
    return ::GetForegroundWindow();
}

HWND WindowUtils::GetDesktopWindow() {
    return ::GetDesktopWindow();
}

HWND WindowUtils::GetParentWindow(HWND hwnd) {
    return ::GetParent(hwnd);
}

// 用于枚举子窗口的辅助结构
struct EnumChildData {
    std::vector<HWND> children;
    bool recursive;
};

BOOL CALLBACK WindowUtils::EnumChildProc(HWND hwnd, LPARAM lParam) {
    EnumChildData* data = reinterpret_cast<EnumChildData*>(lParam);
    data->children.push_back(hwnd);

    if (data->recursive) {
        EnumChildData subData = { data->children, true };
        EnumChildWindows(hwnd, EnumChildProc, reinterpret_cast<LPARAM>(&subData));
    }
    return TRUE;
}

std::vector<HWND> WindowUtils::GetChildWindows(HWND hwnd, bool recursive) {
    std::vector<HWND> result;
    EnumChildData data = { result, recursive };
    EnumChildWindows(hwnd, EnumChildProc, reinterpret_cast<LPARAM>(&data));
    return data.children;
}

DWORD WindowUtils::GetWindowProcessId(HWND hwnd) {
    DWORD processId;
    GetWindowThreadProcessId(hwnd, &processId);
    return processId;
}

DWORD WindowUtils::GetWindowThreadId(HWND hwnd) {
    return GetWindowThreadProcessId(hwnd, NULL);
}

std::string WindowUtils::GetWindowClassName(HWND hwnd) {
    wchar_t buffer[256];
    GetClassNameW(hwnd, buffer, 256);

    int len = WideCharToMultiByte(CP_UTF8, 0, buffer, -1, NULL, 0, NULL, NULL);
    if (len <= 0) return "";

    std::string result(len - 1, 0);
    WideCharToMultiByte(CP_UTF8, 0, buffer, -1, &result[0], len, NULL, NULL);
    return result;
}

std::wstring WindowUtils::GetWindowClassNameW(HWND hwnd) {
    wchar_t buffer[256];
    GetClassNameW(hwnd, buffer, 256);
    return std::wstring(buffer);
}

bool WindowUtils::IsWindowVisible(HWND hwnd) {
    return ::IsWindowVisible(hwnd) != FALSE;
}

bool WindowUtils::IsWindowMinimized(HWND hwnd) {
    return IsIconic(hwnd) != FALSE;
}

bool WindowUtils::IsWindowMaximized(HWND hwnd) {
    return IsZoomed(hwnd) != FALSE;
}

bool WindowUtils::IsWindowTopmost(HWND hwnd) {
    LONG_PTR exStyle = GetWindowLongPtr(hwnd, GWL_EXSTYLE);
    return (exStyle & WS_EX_TOPMOST) != 0;
}

LONG_PTR WindowUtils::GetWindowExStyle(HWND hwnd) {
    return GetWindowLongPtr(hwnd, GWL_EXSTYLE);
}

LONG_PTR WindowUtils::GetWindowStyle(HWND hwnd) {
    return GetWindowLongPtr(hwnd, GWL_STYLE);
}