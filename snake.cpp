#include <iostream>
#include <iomanip>
#include <conio.h>
#include "console.h"
#include "snake.h"

#define ToXY(x, y) Console::GetInstance().SetCursorPos(x, y)

Snake::Snake(int const gameXSize, int const gameYSize)
    : m_GameSize(gameXSize, gameYSize)
    , m_Pos(m_GameSize.first >> 1, m_GameSize.second >> 1)
    , m_FoodPos(0, 0)
    , m_PrevTail(0, 0)
    , m_Tail(m_GameSize.first > m_GameSize.second ? m_GameSize.first : m_GameSize.second, { 0, 0 })
    , m_Running(true)
    , m_Symbol('O')
    , m_FoodSymbol('x')
    , m_Direction(0)
    , m_Length(1)
    , m_Score(0)
{
    PrintBoard();
    PlaceFood();
}

void Snake::Input(char input)
{
    if (input == -1 && _kbhit() == 0)
        return;
    else if (input == -1)
        input = (char)_getch();

    // Hit escape.
    if (input == 27) exit(0);

    // Set only if not moving on opposite direction.
    if (input == 'w' and m_Direction != 's') m_Direction = 'w';
    else if (input == 's' and m_Direction != 'w') m_Direction = 's';
    else if (input == 'a' and m_Direction != 'd') m_Direction = 'a';
    else if (input == 'd' and m_Direction != 'a') m_Direction = 'd';
}

void Snake::Update(int sleepTime)
{
    PrintSnake();

    // Update the current x position of the Snake head for the next iteration.
    std::pair<int, int> prevHead = m_Tail[0];
    m_Tail[0] = m_Pos;
    for (int i = 1; i < m_Length; ++i)
    {
        m_PrevTail = m_Tail[i];
        m_Tail[i] = prevHead;
        prevHead = m_PrevTail;
        m_Tail.push_back({ 0, 0 });
    }

    if (m_Direction == 'w') --m_Pos.second;
    else if (m_Direction == 's') ++m_Pos.second;
    else if (m_Direction == 'a') --m_Pos.first;
    else if (m_Direction == 'd') ++m_Pos.first;

    // Tail.
    if (TailCollision() == true)
    {
        ToXY(m_GameSize.first / 2, m_GameSize.second / 2);     std::cout << "GAME OVER!!";
        ToXY(m_GameSize.first / 2, m_GameSize.second / 2 + 1); std::cout << "You ate your own tail.";
        m_Running = false;
    }
    // Wall.
    else if (WallCollision() == true)
    {
        ToXY(m_GameSize.first / 2, m_GameSize.second / 2);     std::cout << "GAME OVER!!";
        ToXY(m_GameSize.first / 2, m_GameSize.second / 2 + 1); std::cout << "You hit the wall.";
        m_Running = false;
    }
    // Food.
    else if (FoodCollision() == true)
    {
        PlaceFood();
        m_Score += 10;
        m_Length += 3;
    }

    PrintStats();

    if (sleepTime > 0)
        Console::GetInstance().Sleep(sleepTime);
}

void Snake::PlaceFood()
{
    static int constant = 0;
    while (true)
    {
        srand((unsigned)time(0) + constant++);
        m_FoodPos.first = rand() % (m_GameSize.first - 2) + 1;
        m_FoodPos.second = rand() % (m_GameSize.second - 2) + 1;
        if (m_FoodPos == m_Pos)
            continue;
        for (auto i = 0; i < (int)m_Tail.size(); ++i)
            if (m_Tail[i] == m_FoodPos)
                continue;
        break;
    }
    ToXY(m_FoodPos.first, m_FoodPos.second);
    std::cout << m_FoodSymbol;
}

bool Snake::TailCollision() const
{
    for (int i = 0; i < m_Length; ++i)
        if (m_Length > 1 && m_Pos == m_Tail[i])
            return true;
    return false;
}

void Snake::PrintBoard() const
{
    for (auto i = 0; i < m_GameSize.second; ++i)
    {
        // Creates the borders on the left and right.
        if (i > 0 and i < m_GameSize.second - 1)
        {
            std::cout << "*";
            for (auto k = 1; k < m_GameSize.first - 1; ++k)
                std::cout << " ";
            std::cout << "*\n";
            continue;
        }

        // Creates top most and bottom most borders.
        for (auto j = 0; j < m_GameSize.first; ++j)
            std::cout << "*";
        std::cout << "\n";
    }
}

void Snake::PrintStats() const
{
    ToXY(1, m_GameSize.second + 1); std::cout << "Esc to end";
    ToXY(1, m_GameSize.second + 2); std::cout << "WASD to move";
    ToXY(1, m_GameSize.second + 3); std::cout << "Score: ";
    std::cout << std::setfill('0') << std::setw(5) << m_Score << "\n";
}

void Snake::PrintSnake() const
{
    // Head.
    (m_Length == 1) ? ToXY(m_Tail[0].first, m_Tail[0].second) : ToXY(m_PrevTail.first, m_PrevTail.second); std::cout << " ";
    ToXY(m_Pos.first, m_Pos.second); std::cout << m_Symbol;

    // Body.
    for (int i = 1; i < m_Length; ++i)
    {
        ToXY(m_Tail[i].first, m_Tail[i].second);
        std::cout << m_Symbol;
    }
}
