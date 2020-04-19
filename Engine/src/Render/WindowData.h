#pragma once
#include <string>

constexpr float DEFAULT_WIDTH = 1280.f;
constexpr float DEFAULT_HEIGHT = 720.f;

namespace Engine
{
    struct WindowData
    {
        std::string m_Title{ "UBISOFT MATF KURS - Default Name" };
        int m_Width{ static_cast<int>(DEFAULT_WIDTH) };
        int m_Height{ static_cast<int>(DEFAULT_HEIGHT) };
        bool m_Vsync{ false };
    };
}
