#pragma once
#include <string>

constexpr int DEFAULT_WIDTH = 1280;
constexpr int DEFAULT_HEIGHT = 720;

namespace Engine
{
    struct WindowData
    {
        std::string m_Title{ "UBISOFT MATF KURS - Default Name" };
        int m_Width{ DEFAULT_WIDTH };
        int m_Height{ DEFAULT_HEIGHT };
        bool m_Vsync{ false };
    };
}
