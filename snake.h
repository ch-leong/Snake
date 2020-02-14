#pragma once
#include <vector>

class Snake final
{
public:
                Snake(int const gameXSize, int const gameYSize);
    inline bool IsRunning() const { return m_Running; }
    // Game Logic.
    void        Input(char input = -1);
    void        Update(int sleepTime = 60);

private:
    void        PlaceFood();
    // Physic.
    bool        TailCollision() const;
    inline bool WallCollision() const { return m_Pos.first == 0 || m_Pos.first == m_GameSize.first - 1 || m_Pos.second == 0 || m_Pos.second == m_GameSize.second - 1; }
    inline bool FoodCollision() const { return m_Pos == m_FoodPos; }
    // Graphic.
    void        PrintBoard() const;
    void        PrintStats() const;
    void        PrintSnake() const;

    std::pair<int, int> const        m_GameSize;
    std::pair<int, int>              m_Pos;
    std::pair<int, int>              m_FoodPos;
    std::pair<int, int>              m_PrevTail;
    std::vector<std::pair<int, int>> m_Tail;
    bool                             m_Running;
    char                             m_Symbol;
    char                             m_FoodSymbol;
    char                             m_Direction;
    int                              m_Length;
    int                              m_Score;
};
