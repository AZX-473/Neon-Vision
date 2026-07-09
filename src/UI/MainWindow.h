#pragma once
#include <imgui.h>  // 添加这行！
#include <vector>
#include <string>
#include <Windows.h>

class MainWindow {
public:
    MainWindow();
    ~MainWindow() = default;

    void Draw(ImVec2 WindowSize = /*{(float)GetSystemMetrics(SM_CXFULLSCREEN) , (float)GetSystemMetrics(SM_CYFULLSCREEN)}*/{ 640.0f, 360.0f });
    void Reset();

    // Getters
    int GetCounter() const { return m_counter; }
    float GetSliderValue() const { return m_sliderValue; }
    std::string GetStatusMessage() const { return m_statusMessage; }

    double RainbowSpeed = 0.5;
private:
    int m_counter;
    float m_sliderValue;
    ImVec4 m_color;
    int m_selected;
    char m_inputText[128];
    bool m_checkbox;
    std::string m_statusMessage;
    std::vector<float> m_values;
	bool MainWindowShow = true;

    #define Tab_Home 0
    #define Tab_Vision 1
    #define Tab_Mode 2
    #define Tab_Surveillance 3
	int Tab = Tab_Home;  // 当前选中的标签页

    void DrawMENU();
    void DrawTab_HomeMain();
	void DrawTab_VisionMain();
	void DrawTab_ModeMain();
	void DrawTab_SurveillanceMain();

};