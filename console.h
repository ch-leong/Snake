#pragma once

class Console final
{
public:
    static Console& GetInstance();
    void            Load(int winXSize, int winYSize, int fontScale = 11);
    void            FontResize(int fontScale);
    void            WindowResize(int width, int height);
    inline int      GetCol() const { return m_Width; }
    inline int      GetRow() const { return m_Height; }
    void            SetCursorPos(int x, int y) const;
    static void     Sleep(int milliseconds);
private:
                    Console();
                    Console(Console const&) = delete;
    Console&        operator=(Console const&) = delete;
    void* m_Handle;
    void* m_Hwnd;
    int   m_Width;
    int   m_Height;
    int   m_Scale;
};
