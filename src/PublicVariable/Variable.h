#pragma once
#include <imgui.h>
#include <Windows.h>
#include <string>

// Declare global ImGui fonts. Define them in a single .cpp to avoid multiple-definition linker errors.
extern ImFont* g_MainFont;
extern ImFont* g_TitleFont;

extern HWND hWnd; // Main Window hWnd

extern float rb_R, rb_G, rb_B;

#define rb_COLOR ImVec4(rb_R, rb_G, rb_B, 1.0f)

extern ImVec4 Com_Col[114];
extern bool Com_Col_urbc[114];

#define c_show(x) (Com_Col_urbc[x]?rb_COLOR:Com_Col[x])

extern float lw_WindowRECT, off_WindowRECT, ibt_WindowRECT, ict_WindowRECT, ics_WindowRECT;
extern int itype_WindowRECT;
extern float lw_PointerLine, off_PointerLine, ibt_PointerLine, ict_PointerLine, ics_PointerLine;
extern int itype_PointerLine;
extern std::string str_WindowName;
extern float off_wnX,off_wnY;

extern bool ol_WindowName, ol_WindowRECT, ol_PointerLine;

extern bool ol_RainingKey;

extern bool cp_showblock, cp_showRGBn,ol_ColorPicker;
extern float cp_bwlong;
extern LONG off_cpx,off_cpy;