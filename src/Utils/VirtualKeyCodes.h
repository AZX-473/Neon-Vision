// 根据微软官方文档定义所有虚拟键码
// 参考: https://learn.microsoft.com/zh-cn/windows/win32/inputdev/virtual-key-codes

#pragma once

// 鼠标键
#define VK_LBUTTON          0x01
#define VK_RBUTTON          0x02
#define VK_CANCEL           0x03
#define VK_MBUTTON          0x04
#define VK_XBUTTON1         0x05
#define VK_XBUTTON2         0x06
// 0x07 保留

// 键盘主要控制键
#define VK_BACK             0x08
#define VK_TAB              0x09
// 0x0A-0B 保留
#define VK_CLEAR            0x0C
#define VK_RETURN           0x0D
// 0x0E-0F 未分配

// 修饰键与状态切换键
#define VK_SHIFT            0x10
#define VK_CONTROL          0x11
#define VK_MENU             0x12      // Alt键
#define VK_PAUSE            0x13
#define VK_CAPITAL          0x14      // Caps Lock

// IME 相关键 (Kana/Hangul 共用 0x15)
#define VK_KANA             0x15
#define VK_HANGUL           0x15
#define VK_IME_ON           0x16
#define VK_JUNJA            0x17
#define VK_FINAL            0x18
#define VK_HANJA            0x19
#define VK_KANJI            0x19
#define VK_IME_OFF          0x1A

// 其他控制键
#define VK_ESCAPE           0x1B
#define VK_CONVERT          0x1C
#define VK_NONCONVERT       0x1D
#define VK_ACCEPT           0x1E
#define VK_MODECHANGE       0x1F
#define VK_SPACE            0x20
#define VK_PRIOR            0x21      // Page Up
#define VK_NEXT             0x22      // Page Down
#define VK_END              0x23
#define VK_HOME             0x24
#define VK_LEFT             0x25
#define VK_UP               0x26
#define VK_RIGHT            0x27
#define VK_DOWN             0x28
#define VK_SELECT           0x29
#define VK_PRINT            0x2A
#define VK_EXECUTE          0x2B
#define VK_SNAPSHOT         0x2C      // Print Screen
#define VK_INSERT           0x2D
#define VK_DELETE           0x2E
#define VK_HELP             0x2F

// 数字键 0-9 (与ASCII '0'-'9' 值相同)
#define VK_0                0x30
#define VK_1                0x31
#define VK_2                0x32
#define VK_3                0x33
#define VK_4                0x34
#define VK_5                0x35
#define VK_6                0x36
#define VK_7                0x37
#define VK_8                0x38
#define VK_9                0x39
// 0x3A-40 未定义

// 字母键 A-Z (与ASCII 'A'-'Z' 值相同)
#define VK_A                0x41
#define VK_B                0x42
#define VK_C                0x43
#define VK_D                0x44
#define VK_E                0x45
#define VK_F                0x46
#define VK_G                0x47
#define VK_H                0x48
#define VK_I                0x49
#define VK_J                0x4A
#define VK_K                0x4B
#define VK_L                0x4C
#define VK_M                0x4D
#define VK_N                0x4E
#define VK_O                0x4F
#define VK_P                0x50
#define VK_Q                0x51
#define VK_R                0x52
#define VK_S                0x53
#define VK_T                0x54
#define VK_U                0x55
#define VK_V                0x56
#define VK_W                0x57
#define VK_X                0x58
#define VK_Y                0x59
#define VK_Z                0x5A

// Windows 徽标键与应用程序键
#define VK_LWIN             0x5B
#define VK_RWIN             0x5C
#define VK_APPS             0x5D
// 0x5E 保留
#define VK_SLEEP            0x5F

// 数字键盘键
#define VK_NUMPAD0          0x60
#define VK_NUMPAD1          0x61
#define VK_NUMPAD2          0x62
#define VK_NUMPAD3          0x63
#define VK_NUMPAD4          0x64
#define VK_NUMPAD5          0x65
#define VK_NUMPAD6          0x66
#define VK_NUMPAD7          0x67
#define VK_NUMPAD8          0x68
#define VK_NUMPAD9          0x69
#define VK_MULTIPLY         0x6A      // * 键
#define VK_ADD              0x6B      // + 键
#define VK_SEPARATOR        0x6C      // 分隔符键 (通常为 ,)
#define VK_SUBTRACT         0x6D      // - 键
#define VK_DECIMAL          0x6E      // . 键
#define VK_DIVIDE           0x6F      // / 键

// 功能键 F1-F24
#define VK_F1               0x70
#define VK_F2               0x71
#define VK_F3               0x72
#define VK_F4               0x73
#define VK_F5               0x74
#define VK_F6               0x75
#define VK_F7               0x76
#define VK_F8               0x77
#define VK_F9               0x78
#define VK_F10              0x79
#define VK_F11              0x7A
#define VK_F12              0x7B
#define VK_F13              0x7C
#define VK_F14              0x7D
#define VK_F15              0x7E
#define VK_F16              0x7F
#define VK_F17              0x80
#define VK_F18              0x81
#define VK_F19              0x82
#define VK_F20              0x83
#define VK_F21              0x84
#define VK_F22              0x85
#define VK_F23              0x86
#define VK_F24              0x87
// 0x88-8F 保留

// 状态锁定键
#define VK_NUMLOCK          0x90
#define VK_SCROLL           0x91      // Scroll Lock
// 0x92-96 OEM 特定
// 0x97-9F 未分配

// 左右修饰键 (用于区分)
#define VK_LSHIFT           0xA0
#define VK_RSHIFT           0xA1
#define VK_LCONTROL         0xA2
#define VK_RCONTROL         0xA3
#define VK_LMENU            0xA4      // 左 Alt
#define VK_RMENU            0xA5      // 右 Alt

// 浏览器与多媒体键
#define VK_BROWSER_BACK     0xA6
#define VK_BROWSER_FORWARD  0xA7
#define VK_BROWSER_REFRESH  0xA8
#define VK_BROWSER_STOP     0xA9
#define VK_BROWSER_SEARCH   0xAA
#define VK_BROWSER_FAVORITES 0xAB
#define VK_BROWSER_HOME     0xAC
#define VK_VOLUME_MUTE      0xAD
#define VK_VOLUME_DOWN      0xAE
#define VK_VOLUME_UP        0xAF
#define VK_MEDIA_NEXT_TRACK 0xB0
#define VK_MEDIA_PREV_TRACK 0xB1
#define VK_MEDIA_STOP       0xB2
#define VK_MEDIA_PLAY_PAUSE 0xB3
#define VK_LAUNCH_MAIL      0xB4
#define VK_LAUNCH_MEDIA_SELECT 0xB5
#define VK_LAUNCH_APP1      0xB6
#define VK_LAUNCH_APP2      0xB7
// 0xB8-B9 保留

// OEM 特定键 (物理键位可能因键盘布局而异)
#define VK_OEM_1            0xBA      // US: ;:
#define VK_OEM_PLUS         0xBB      // US: =+
#define VK_OEM_COMMA        0xBC      // US: ,<
#define VK_OEM_MINUS        0xBD      // US: -_
#define VK_OEM_PERIOD       0xBE      // US: .>
#define VK_OEM_2            0xBF      // US: /?
#define VK_OEM_3            0xC0      // US: `~
// 0xC1-C2 保留

// 游戏手柄按键 (Windows 游戏控制器支持)
#define VK_GAMEPAD_A                0xC3
#define VK_GAMEPAD_B                0xC4
#define VK_GAMEPAD_X                0xC5
#define VK_GAMEPAD_Y                0xC6
#define VK_GAMEPAD_RIGHT_SHOULDER   0xC7
#define VK_GAMEPAD_LEFT_SHOULDER    0xC8
#define VK_GAMEPAD_LEFT_TRIGGER     0xC9
#define VK_GAMEPAD_RIGHT_TRIGGER    0xCA
#define VK_GAMEPAD_DPAD_UP          0xCB
#define VK_GAMEPAD_DPAD_DOWN        0xCC
#define VK_GAMEPAD_DPAD_LEFT        0xCD
#define VK_GAMEPAD_DPAD_RIGHT       0xCE
#define VK_GAMEPAD_MENU             0xCF
#define VK_GAMEPAD_VIEW             0xD0
#define VK_GAMEPAD_LEFT_THUMBSTICK_BUTTON 0xD1
#define VK_GAMEPAD_RIGHT_THUMBSTICK_BUTTON 0xD2
#define VK_GAMEPAD_LEFT_THUMBSTICK_UP     0xD3
#define VK_GAMEPAD_LEFT_THUMBSTICK_DOWN   0xD4
#define VK_GAMEPAD_LEFT_THUMBSTICK_RIGHT  0xD5
#define VK_GAMEPAD_LEFT_THUMBSTICK_LEFT   0xD6
#define VK_GAMEPAD_RIGHT_THUMBSTICK_UP    0xD7
#define VK_GAMEPAD_RIGHT_THUMBSTICK_DOWN  0xD8
#define VK_GAMEPAD_RIGHT_THUMBSTICK_RIGHT 0xD9
#define VK_GAMEPAD_RIGHT_THUMBSTICK_LEFT  0xDA

// OEM 特定键 (续)
#define VK_OEM_4            0xDB      // US: {[
#define VK_OEM_5            0xDC      // US: \|
#define VK_OEM_6            0xDD      // US: }]
#define VK_OEM_7            0xDE      // US: '"
#define VK_OEM_8            0xDF
// 0xE0 保留
// 0xE1 OEM 特定
#define VK_OEM_102          0xE2      // 欧洲 ISO 键盘上的反斜杠键
// 0xE3-E4 OEM 特定
#define VK_PROCESSKEY       0xE5      // IME 处理键
// 0xE6 OEM 特定
#define VK_PACKET           0xE7      // 用于传递 Unicode 字符
// 0xE8 未分配
// 0xE9-F5 OEM 特定

// 更古老或专用的功能键
#define VK_ATTN             0xF6
#define VK_CRSEL            0xF7
#define VK_EXSEL            0xF8
#define VK_EREOF            0xF9
#define VK_PLAY             0xFA
#define VK_ZOOM             0xFB
#define VK_NONAME           0xFC      // 保留
#define VK_PA1              0xFD
#define VK_OEM_CLEAR        0xFE