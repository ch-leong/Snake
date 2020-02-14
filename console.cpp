#include "console.h"
#include <wchar.h>
#include <assert.h>
#include <windows.h>

Console& Console::GetInstance()
{
    static Console instance;
    return instance;
}

void Console::Load(int winXSize, int winYSize, int fontScale)
{
    FontResize(fontScale);
    WindowResize(winXSize, winYSize);
}

void Console::FontResize(int fontScale)
{
    m_Scale = fontScale;

    CONSOLE_FONT_INFOEX font{ (ULONG)sizeof(CONSOLE_FONT_INFOEX) };
    swprintf_s(font.FaceName, L"Terminal");
    assert(SetCurrentConsoleFontEx(m_Handle, FALSE, &font) != 0);
    font.dwFontSize.X = (SHORT)m_Scale;
    font.dwFontSize.Y = (SHORT)m_Scale;
    assert(SetCurrentConsoleFontEx(m_Handle, FALSE, &font) != 0);
}

void Console::WindowResize(int width, int height)
{
    m_Width = width;
    m_Height = height;
    int const area = m_Width * m_Height;

    RECT deskSize;
    GetWindowRect(GetDesktopWindow(), &deskSize);

    MoveWindow((HWND)m_Hwnd
        , (deskSize.right >> 1) - ((m_Height >> 1) * m_Scale)
        , (deskSize.bottom >> 1) - ((m_Height >> 1) * m_Scale)
        , 0, 0, FALSE);
    // Current Console Buffer Info.
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    // New Console Size.
    SMALL_RECT srWindowRect;
    COORD coordScreen;

    assert(GetConsoleScreenBufferInfo(m_Handle, &csbi) != 0);

    // Get the Largest Size we can size the Console Window to.
    coordScreen = GetLargestConsoleWindowSize(m_Handle);

    // Define the New Console Window Size and Scroll Position.
    srWindowRect.Right = (SHORT)(min((SHORT)m_Width, coordScreen.X) - 1);
    srWindowRect.Bottom = (SHORT)(min((SHORT)m_Height, coordScreen.Y) - 1);
    srWindowRect.Left = srWindowRect.Top = (SHORT)0;

    // Define the New Console Buffer Size.
    coordScreen.X = (SHORT)m_Width;
    coordScreen.Y = (SHORT)m_Height;

    // Buffer size & window size adjustment.
    if ((DWORD)csbi.dwSize.X * csbi.dwSize.Y > (DWORD)area)
    {
        assert(SetConsoleWindowInfo(m_Handle, TRUE, &srWindowRect) != 0);
        assert(SetConsoleScreenBufferSize(m_Handle, coordScreen) != 0);
    }
    if ((DWORD)csbi.dwSize.X * csbi.dwSize.Y < (DWORD)area)
    {
        assert(SetConsoleScreenBufferSize(m_Handle, coordScreen) != 0);
        assert(SetConsoleWindowInfo(m_Handle, TRUE, &srWindowRect) != 0);
    }
}

void Console::SetCursorPos(int x, int y) const
{
    COORD coord{ (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(m_Handle, coord);
}

void Console::Sleep(int milliseconds)
{
    ::Sleep(milliseconds);
}

Console::Console()
    : m_Handle(GetStdHandle(STD_OUTPUT_HANDLE))
    , m_Hwnd(GetConsoleWindow())
    , m_Width(0)
    , m_Height(0)
    , m_Scale(0)
{}
