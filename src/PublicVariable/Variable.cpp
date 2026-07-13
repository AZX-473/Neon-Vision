#include "Variable.h"

// Define global ImGui fonts in one translation unit.
ImFont* g_MainFont = nullptr;
ImFont* g_TitleFont = nullptr;

HWND hWnd = nullptr; // Main Window hWnd

float rb_R=0.0f, rb_G=0.0f, rb_B=0.0f;

ImVec4 Com_Col[114] = {};
bool Com_Col_urbc[114] = {true, true, true, true, true};

float lw_WindowRECT=2.0f, off_WindowRECT=5.0f, ibt_WindowRECT = 1.0f, ict_WindowRECT = 1.0f, ics_WindowRECT = 10.0f;
int itype_WindowRECT=0;
float lw_PointerLine=2.0f, off_PointerLine=5.0f, ibt_PointerLine=1.0f, ict_PointerLine = 1.0f, ics_PointerLine = 10.0f;
int itype_PointerLine=0;
std::string str_WindowName="${WindowName}";
float off_wnX=5.0f, off_wnY=5.0f;

bool ol_WindowRECT = true, ol_WindowName = true, ol_PointerLine = true;

bool ol_RainingKey = true;

bool cp_showblock = true, cp_showRGBn = true,ol_ColorPicker=true;
float cp_bwlong = 20.0f;
LONG off_cpx=5.0f,off_cpy=30.0f;