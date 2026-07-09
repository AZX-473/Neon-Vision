#include "Variable.h"

// Define global ImGui fonts in one translation unit.
ImFont* g_MainFont = nullptr;
ImFont* g_TitleFont = nullptr;

HWND hWnd = nullptr; // Main Window hWnd

float rb_R=0.0f, rb_G=0.0f, rb_B=0.0f;

ImVec4 Com_Col[114] = {};
bool Com_Col_urbc[114] = {true, true, true};